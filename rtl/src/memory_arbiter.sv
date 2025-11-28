// memory_arbiter.sv
import flexpipe_pkg::*;

module memory_arbiter (
    input  logic        clk,
    input  logic        rst_n,

    // Active Core (High priority)
    input  mem_req_t    active_req,
    input  logic        active_req_valid,
    output logic        active_req_ready,
    output mem_resp_t   active_resp,
    output logic        active_resp_valid,

    // Prefetcher (Low priority)
    input  mem_req_t    prefetch_req,
    input  logic        prefetch_req_valid,
    output logic        prefetch_req_ready,
    output mem_resp_t   prefetch_resp,
    output logic        prefetch_resp_valid,

    // Master (DRAM)
    output mem_req_t    mem_master_req,
    output logic        mem_master_valid,
    input  logic        mem_master_ready,
    input  mem_resp_t   mem_master_resp,
    input  logic        mem_master_resp_valid,

    // Status
    output logic        no_outstanding_active
);

    // outstanding counters
    logic [7:0] active_cnt;
    logic [7:0] prefetch_cnt;

    // ID MSB indicates source: 0 = active, 1 = prefetch
    localparam int ID_SRC_BIT = REQ_ID_WIDTH - 1;

    // default outputs
    assign active_resp_valid = 1'b0;
    assign prefetch_resp_valid = 1'b0;
    assign active_resp = mem_master_resp;
    assign prefetch_resp = mem_master_resp;

    // Simple arbitration (strict priority). If active has valid request, forward it, else prefetch.
    always_comb begin
        mem_master_valid = 1'b0;
        mem_master_req = '0;
        active_req_ready = 1'b0;
        prefetch_req_ready = 1'b0;

        if (active_req_valid) begin
            mem_master_valid = 1'b1;
            mem_master_req   = active_req;
            // Force source bit to 0
            mem_master_req.id[ID_SRC_BIT] = 1'b0;
            active_req_ready = mem_master_ready;
        end else if (prefetch_req_valid) begin
            mem_master_valid = 1'b1;
            mem_master_req   = prefetch_req;
            mem_master_req.id[ID_SRC_BIT] = 1'b1;
            prefetch_req_ready = mem_master_ready;
        end
    end

    // Track outstanding counts: increment when grant occurred, decrement on response.last
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            active_cnt <= 0;
            prefetch_cnt <= 0;
        end else begin
            // increment on grant
            if (active_req_valid && active_req_ready) active_cnt <= active_cnt + 1;
            if (prefetch_req_valid && prefetch_req_ready) prefetch_cnt <= prefetch_cnt + 1;

            // decrement on response (last beat)
            if (mem_master_resp_valid && mem_master_resp.last) begin
                if (mem_master_resp.id[ID_SRC_BIT] == 1'b0 && active_cnt > 0) active_cnt <= active_cnt - 1;
                if (mem_master_resp.id[ID_SRC_BIT] == 1'b1 && prefetch_cnt > 0) prefetch_cnt <= prefetch_cnt - 1;
            end
        end
    end

    // route responses to correct client based on id MSB
    always_comb begin
        active_resp_valid = 1'b0;
        prefetch_resp_valid = 1'b0;

        if (mem_master_resp_valid) begin
            if (mem_master_resp.id[ID_SRC_BIT] == 1'b0) begin
                active_resp_valid = 1'b1;
            end else begin
                prefetch_resp_valid = 1'b1;
            end
        end
    end

    assign no_outstanding_active = (active_cnt == 0);

endmodule
