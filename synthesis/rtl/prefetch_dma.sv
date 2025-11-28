`include "flexpipe_pkg.sv"

module prefetch_dma (
    input  logic                   clk,
    input  logic                   rst_n,

    // Configuration & control
    input  layer_config_t          shadow_config,
    input  logic [`EPOCH_WIDTH-1:0] current_epoch,
    input  logic                   start_prefetch,
    output logic                   busy,
    output logic [31:0]            bytes_fetched, // exported for top-level instrumentation

    // Memory interface (to arbiter)
    output mem_req_t               mem_req,
    output logic                   mem_req_valid,
    input  logic                   mem_req_ready,

    // Response from arbiter/DRAM
    input  mem_resp_t              mem_resp,
    input  logic                   mem_resp_valid
);

    typedef enum logic [2:0] { IDLE, PTR_REQ, PTR_WAIT, DATA_REQ, DATA_WAIT, DONE } state_e;
    state_e state, next_state;

    logic [`ADDR_WIDTH-1:0] current_ptr_addr;
    logic [`ADDR_WIDTH-1:0] fetched_data_addr;
    logic [31:0]           walks_remaining;
    logic [`EPOCH_WIDTH-1:0] my_epoch;
    logic [31:0]           bytes_accum;

    // Defaults
    assign mem_req = '0;
    assign mem_req_valid = 1'b0;
    assign busy = (state != IDLE && state != DONE);
    assign bytes_fetched = bytes_accum;

    // FSM combinational
    always @(*) begin
        next_state = state;
        mem_req_valid = 1'b0;
        // mem_req default zero
        case (state)
            IDLE: begin
                if (start_prefetch && shadow_config.valid) begin
                    next_state = PTR_REQ;
                end
            end
            PTR_REQ: begin
                // issue pointer read (64-bit pointer)
                mem_req.addr  = current_ptr_addr;
                mem_req.len   = 8;
                mem_req.rtype = REQ_TYPE_PTR;
                mem_req.prio  = PRIO_LOW;
                mem_req.epoch = my_epoch;
                mem_req_valid = 1'b1;
                if (mem_req_ready) next_state = PTR_WAIT;
            end
            PTR_WAIT: begin
                if (mem_resp_valid && (mem_resp.epoch == my_epoch) && (mem_resp.id == mem_req.id)) begin
                    next_state = DATA_REQ;
                end
            end
            DATA_REQ: begin
                mem_req.addr  = fetched_data_addr;
                mem_req.len   = (`DATA_WIDTH/8); // request one DRAM beat (32 bytes)
                mem_req.rtype = REQ_TYPE_DATA;
                mem_req.prio  = PRIO_LOW;
                mem_req.epoch = my_epoch;
                mem_req_valid = 1'b1;
                if (mem_req_ready) next_state = DATA_WAIT;
            end
            DATA_WAIT: begin
                if (mem_resp_valid && (mem_resp.epoch == my_epoch) && (mem_resp.id == mem_req.id) && mem_resp.last) begin
                    if (walks_remaining > 0) next_state = PTR_REQ;
                    else next_state = DONE;
                end
            end
            DONE: begin
                // stay until reset or a new shadow_config appears
                if (!shadow_config.valid) next_state = IDLE;
            end
            default: next_state = IDLE;
        endcase
    end

    // FSM sequential / datapath
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= IDLE;
            current_ptr_addr <= '0;
            fetched_data_addr <= '0;
            walks_remaining <= 0;
            my_epoch <= '0;
            bytes_accum <= 0;
        end else begin
            state <= next_state;

            if (state == IDLE && next_state == PTR_REQ) begin
                current_ptr_addr <= shadow_config.base_addr;
                walks_remaining  <= shadow_config.pointer_walks;
                my_epoch         <= current_epoch;
                bytes_accum      <= 0;
            end

            // capture pointer value from response (mocked in simulation by harness/dram)
            if (state == PTR_WAIT && mem_resp_valid && (mem_resp.epoch == my_epoch)) begin
                // take low bits as address
                fetched_data_addr <= mem_resp.data[`ADDR_WIDTH-1:0];
            end

            if (state == DATA_WAIT && mem_resp_valid && (mem_resp.epoch == my_epoch) && mem_resp.last) begin
                // count bytes returned (DATA_WIDTH/8)
                bytes_accum <= bytes_accum + (`DATA_WIDTH/8);
                if (walks_remaining > 0) begin
                    walks_remaining <= walks_remaining - 1;
                    current_ptr_addr <= current_ptr_addr + 8; // next pointer
                end
            end

            // abort if epoch changed (stale)
            if (state != IDLE && my_epoch != current_epoch) begin
                // abort prefetch
                state <= IDLE;
            end
        end
    end

endmodule
