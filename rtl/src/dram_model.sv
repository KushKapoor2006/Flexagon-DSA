// dram_model.sv
// Simple cycle-based DRAM model for Verilator smoke simulation.
// - Accepts mem_req_t requests into a small FIFO
// - Simulates turn-around latency + transfer time (ceil(len / BW))
// - Returns a single response beat with id/epoch echoed back (last=1)

import flexpipe_pkg::*;

module dram_model #(
    parameter int BW_BYTES_PER_CYCLE = (DATA_WIDTH/8), // bytes/beat
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

    // small fifo (depth 16) for incoming requests
    typedef struct packed { mem_req_t req; } req_entry_t;
    req_entry_t fifo [0:15];
    logic [3:0] rd_ptr;
    logic [3:0] wr_ptr;
    logic [4:0]  count; // up to 16

    // active request being serviced
    mem_req_t active_req;
    logic      busy;
    int        cycles_left;

    // small integer helper for transfer compute
    integer xfer_calc;

    // Ready when fifo has space
    assign mem_req_ready = (count < 16);

    // Push incoming requests into fifo
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr <= 0;
            count  <= 0;
        end else begin
            if (mem_req_valid && mem_req_ready) begin
                fifo[wr_ptr].req <= mem_req;
                wr_ptr <= wr_ptr + 1;
                count  <= count + 1;
            end
        end
    end

    // Serve requests and generate response
    // Note: keep sequential logic simple and avoid inline type declarations inside statements
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            rd_ptr <= 0;
            busy <= 1'b0;
            cycles_left <= 0;
            active_req <= '0;
            mem_resp_valid <= 1'b0;
            mem_resp <= '0;
        end else begin
            // default: no response this cycle
            mem_resp_valid <= 1'b0;

            // If not busy and FIFO not empty, dequeue and start service
            if (!busy && (count > 0)) begin
                active_req <= fifo[rd_ptr].req;
                rd_ptr <= rd_ptr + 1;
                count <= count - 1;

                // compute transfer cycles = ceil(len / BW_BYTES_PER_CYCLE)
                xfer_calc = (fifo[rd_ptr].req.len + BW_BYTES_PER_CYCLE - 1) / BW_BYTES_PER_CYCLE;
                if (xfer_calc <= 0) xfer_calc = 1;

                cycles_left <= FIXED_LATENCY + xfer_calc;
                busy <= 1'b1;
            end
            // If busy, count down cycles_left and generate response when done
            else if (busy) begin
                if (cycles_left > 1) begin
                    cycles_left <= cycles_left - 1;
                end else begin
                    // produce a single response beat (last=1)
                    mem_resp.data  <= '0;                    // payload not modelled in detail
                    mem_resp.id    <= active_req.id;
                    mem_resp.epoch <= active_req.epoch;
                    mem_resp.last  <= 1'b1;
                    mem_resp.error <= 1'b0;
                    mem_resp_valid <= 1'b1;

                    busy <= 1'b0;
                    cycles_left <= 0;
                end
            end
        end
    end

endmodule
