`include "flexpipe_pkg.sv"

// Simple cycle-based DRAM model for Verilator smoke simulation.
// - Accepts mem_req_t requests into a small FIFO
// - Simulates turn-around latency + transfer time (ceil(len / BW))
// - Returns a single response beat with id/epoch echoed back (last=1)

module dram_model #(
    parameter int BW_BYTES_PER_CYCLE = (`DATA_WIDTH/8), // bytes/beat
    parameter int FIXED_LATENCY      = 120
)(
    input  logic        clk,
    input  logic        rst_n,

    input  mem_req_t    mem_req,
    input  logic        mem_req_valid,
    output logic        mem_req_ready,

    output mem_resp_t   mem_resp,
    output logic        mem_resp_valid
);

    // ----------------------------------------------------------------
    // FIFO of incoming requests
    // NOTE: use separate arrays for each field instead of mem_req_t
    //       to keep Yosys happy (no array-of-struct with variable index).
    // ----------------------------------------------------------------
    logic [`ADDR_WIDTH-1:0]   fifo_addr  [0:15];
    logic [31:0]              fifo_len   [0:15];
    logic [`REQ_ID_WIDTH-1:0] fifo_id    [0:15];
    logic [`EPOCH_WIDTH-1:0]  fifo_epoch [0:15];
    req_type_e                fifo_rtype [0:15];
    req_prio_e                fifo_prio  [0:15];

    logic [3:0] rd_ptr;
    logic [3:0] wr_ptr;
    logic [4:0] count; // up to 16 entries

    // Active request being serviced
    logic [`ADDR_WIDTH-1:0]   active_addr;
    logic [31:0]              active_len;
    logic [`REQ_ID_WIDTH-1:0] active_id;
    logic [`EPOCH_WIDTH-1:0]  active_epoch;
    req_type_e                active_rtype;
    req_prio_e                active_prio;

    logic      busy;
    int        cycles_left;

    // small integer helper for transfer compute
    integer xfer_calc;

    // Ready when fifo has space
    assign mem_req_ready = (count < 16);

    // ----------------------------------------------------------------
    // Push incoming requests into FIFO
    // ----------------------------------------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr <= 0;
            count  <= 0;
        end else begin
            if (mem_req_valid && mem_req_ready) begin
                fifo_addr[wr_ptr]   <= mem_req.addr;
                fifo_len[wr_ptr]    <= mem_req.len;
                fifo_id[wr_ptr]     <= mem_req.id;
                fifo_epoch[wr_ptr]  <= mem_req.epoch;
                fifo_rtype[wr_ptr]  <= mem_req.rtype;
                fifo_prio[wr_ptr]   <= mem_req.prio;
                wr_ptr              <= wr_ptr + 1;
                count               <= count + 1;
            end
        end
    end

    // ----------------------------------------------------------------
    // Serve requests and generate response
    // ----------------------------------------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            rd_ptr         <= 0;
            busy           <= 1'b0;
            cycles_left    <= 0;
            active_addr    <= '0;
            active_len     <= 32'd0;
            active_id      <= '0;
            active_epoch   <= '0;
            active_rtype   <= REQ_TYPE_DATA;
            active_prio    <= PRIO_LOW;
            mem_resp_valid <= 1'b0;
            mem_resp       <= '0;
        end else begin
            // default: no response this cycle
            mem_resp_valid <= 1'b0;

            // If not busy and FIFO not empty, dequeue and start service
            if (!busy && (count > 0)) begin
                active_addr   <= fifo_addr[rd_ptr];
                active_len    <= fifo_len[rd_ptr];
                active_id     <= fifo_id[rd_ptr];
                active_epoch  <= fifo_epoch[rd_ptr];
                active_rtype  <= fifo_rtype[rd_ptr];
                active_prio   <= fifo_prio[rd_ptr];

                // compute transfer cycles = ceil(len / BW_BYTES_PER_CYCLE)
                xfer_calc = (fifo_len[rd_ptr] + BW_BYTES_PER_CYCLE - 1) / BW_BYTES_PER_CYCLE;
                if (xfer_calc <= 0) xfer_calc = 1;

                rd_ptr      <= rd_ptr + 1;
                count       <= count - 1;
                cycles_left <= FIXED_LATENCY + xfer_calc;
                busy        <= 1'b1;
            end
            // If busy, count down cycles_left and generate response when done
            else if (busy) begin
                if (cycles_left > 1) begin
                    cycles_left <= cycles_left - 1;
                end else begin
                    // produce a single response beat (last=1)
                    mem_resp.data  <= '0;              // payload not modelled in detail
                    mem_resp.id    <= active_id;
                    mem_resp.epoch <= active_epoch;
                    mem_resp.last  <= 1'b1;
                    mem_resp.error <= 1'b0;
                    mem_resp_valid <= 1'b1;

                    busy        <= 1'b0;
                    cycles_left <= 0;
                end
            end
        end
    end

endmodule
