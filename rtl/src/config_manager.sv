// config_manager.sv
import flexpipe_pkg::*;

module config_manager (
    input  logic                   clk,
    input  logic                   rst_n,

    // Host AXI-lite style (address width = ADDR_WIDTH to match top)
    input  logic [ADDR_WIDTH-1:0]  s_axi_awaddr,
    input  logic                   s_axi_awvalid,
    output logic                   s_axi_awready,
    input  logic [31:0]            s_axi_wdata,
    input  logic [3:0]             s_axi_wstrb,
    input  logic                   s_axi_wvalid,
    output logic                   s_axi_wready,
    output logic [1:0]             s_axi_bresp,
    output logic                   s_axi_bvalid,
    input  logic                   s_axi_bready,

    // Core control interface
    input  logic                   core_safe_to_flip,
    input  logic                   no_outstanding_active,
    output logic                   request_flip,
    input  logic                   flip_ack,

    // Outputs
    output layer_config_t          active_config,
    output layer_config_t          shadow_config,
    output logic                   layer_start_pulse,
    output logic [EPOCH_WIDTH-1:0] current_epoch
);

    // Internal registers
    layer_config_t active_config_r, shadow_config_r;
    logic          shadow_full;
    logic [EPOCH_WIDTH-1:0] epoch_counter;
    logic          request_flip_r;
    logic [2:0]    write_idx;

    // AXI-Lite write FSM
    typedef enum logic [1:0] { AXI_IDLE, AXI_WRITE, AXI_RESP } axi_state_e;
    axi_state_e axi_state;

    // Defaults
    assign s_axi_awready = (axi_state == AXI_IDLE);
    assign s_axi_wready  = (axi_state == AXI_WRITE);
    assign s_axi_bresp   = 2'b00;

    // Reset/init
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            axi_state        <= AXI_IDLE;
            write_idx        <= 0;
            shadow_full      <= 1'b0;
            shadow_config_r  <= '0;
            s_axi_bvalid     <= 1'b0;
            epoch_counter    <= {EPOCH_WIDTH{1'b0}};
            request_flip_r   <= 1'b0;
            active_config_r  <= '0;
            layer_start_pulse <= 1'b0;
        end else begin
            // AXI write flow (simple single-word-per-cycle model)
            case (axi_state)
                AXI_IDLE: begin
                    s_axi_bvalid <= 1'b0;
                    if (s_axi_awvalid) begin
                        axi_state <= AXI_WRITE;
                    end
                end
                AXI_WRITE: begin
                    if (s_axi_wvalid) begin
                        // Map sequential writes to fields (simple)
                        unique case (write_idx)
                            0: shadow_config_r.compute_cycles <= s_axi_wdata;
                            1: shadow_config_r.config_bits    <= s_axi_wdata;
                            2: shadow_config_r.pointer_walks  <= s_axi_wdata;
                            3: shadow_config_r.data_size      <= s_axi_wdata;
                            default: ; // ignore
                        endcase
                        write_idx <= write_idx + 1;
                        s_axi_bvalid <= 1'b1;
                        axi_state <= AXI_RESP;

                        // mark shadow valid when we've loaded the basic fields
                        if (write_idx == 3) begin
                            shadow_full <= 1'b1;
                            shadow_config_r.valid <= 1'b1;
                            write_idx <= 0;
                        end
                    end
                end
                AXI_RESP: begin
                    if (s_axi_bready) begin
                        s_axi_bvalid <= 1'b0;
                        axi_state <= AXI_IDLE;
                    end
                end
                default: axi_state <= AXI_IDLE;
            endcase

            // Flip FSM: safe handshake
            layer_start_pulse <= 1'b0;
            case ({request_flip_r, flip_ack})
                default: ; // handled below
            endcase

            // Simple two-stage flip: wait for shadow_full, then for core safe, then request flip -> wait_ack -> flip
            // Implemented in a lightweight way
            if (shadow_full && core_safe_to_flip && no_outstanding_active && !request_flip_r) begin
                request_flip_r <= 1'b1;
            end

            if (request_flip_r && flip_ack) begin
                // Perform atomic flip
                active_config_r <= shadow_config_r;
                epoch_counter <= epoch_counter + 1;
                active_config_r.epoch <= epoch_counter + 1;
                active_config_r.valid <= 1'b1;
                shadow_full <= 1'b0;
                shadow_config_r.valid <= 1'b0;
                request_flip_r <= 1'b0;
                layer_start_pulse <= 1'b1;
            end
        end
    end

    assign request_flip = request_flip_r;
    assign active_config = active_config_r;
    assign shadow_config = shadow_config_r;
    assign current_epoch = epoch_counter;

endmodule
