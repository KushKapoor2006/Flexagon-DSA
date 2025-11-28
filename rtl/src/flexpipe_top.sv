// flexpipe_top.sv
import flexpipe_pkg::*;

module flexpipe_top (
    input  logic                   clk,
    input  logic                   rst_n,

    // ----- Debug ports (added) -----
    input  logic dbg_mode,
    output logic [31:0] dbg_cycle_count,

    // AXI-lite config interface
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

    // Core control signals (from harness or dummy core)
    input  logic                   core_idle,
    input  logic                   core_pipeline_drained,
    input  logic                   flip_ack,
    output logic                   request_flip,
    output layer_config_t          active_config,
    output logic                   layer_start_pulse,

    // Active core memory interface (driven by harness)
    input  mem_req_t               core_mem_req,
    input  logic                   core_mem_req_valid,
    output logic                   core_mem_req_ready,
    output mem_resp_t              core_mem_resp,
    output logic                   core_mem_resp_valid,

    // Master DRAM interface (to dram_model)
    output mem_req_t               mem_master_req,
    output logic                   mem_master_valid,
    input  logic                   mem_master_ready,
    input  mem_resp_t              mem_master_resp,
    input  logic                   mem_master_resp_valid
);

    // ------------------------------------------------------------------
    // Internal signals and debug instrumentation
    // ------------------------------------------------------------------

    // existing internal wires
    layer_config_t shadow_config;
    logic [EPOCH_WIDTH-1:0] current_epoch;
    logic prefetch_busy;
    logic [31:0] bytes_fetched_w;

    // prefetch -> arbiter
    mem_req_t prefetch_req;
    logic     prefetch_req_valid;
    logic     prefetch_req_ready;
    mem_resp_t prefetch_resp;
    logic     prefetch_resp_valid;

    // arbiter -> dram
    mem_req_t arb_to_dram_req;
    logic     arb_to_dram_valid;

    logic no_outstanding_active;

    // -------------------------
    // Debug counter (instrumentation)
    // -------------------------
    // A simple RTL-side cycle counter which increments on every rising
    // edge while dbg_mode is asserted. Non-invasive: does not change
    // functional behavior unless dbg_mode is asserted by the harness.
    logic [31:0] dbg_cycle_count_r;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            dbg_cycle_count_r <= 32'd0;
        end else begin
            if (dbg_mode)
                dbg_cycle_count_r <= dbg_cycle_count_r + 32'd1;
            // else hold current value so harness can read after stopping dbg_mode
        end
    end

    // Drive the top-level output port
    assign dbg_cycle_count = dbg_cycle_count_r;

    // ------------------------------------------------------------------
    // Instantiations (unchanged)
    // ------------------------------------------------------------------

    // instantiate config_manager
    config_manager u_config_manager (
        .clk                   (clk),
        .rst_n                 (rst_n),
        .s_axi_awaddr          (s_axi_awaddr),
        .s_axi_awvalid         (s_axi_awvalid),
        .s_axi_awready         (s_axi_awready),
        .s_axi_wdata           (s_axi_wdata),
        .s_axi_wstrb           (s_axi_wstrb),
        .s_axi_wvalid          (s_axi_wvalid),
        .s_axi_wready          (s_axi_wready),
        .s_axi_bresp           (s_axi_bresp),
        .s_axi_bvalid          (s_axi_bvalid),
        .s_axi_bready          (s_axi_bready),
        .core_safe_to_flip     (core_idle && core_pipeline_drained),
        .no_outstanding_active (no_outstanding_active),
        .request_flip          (request_flip),
        .flip_ack              (flip_ack),
        .active_config         (active_config),
        .shadow_config         (shadow_config),
        .layer_start_pulse     (layer_start_pulse),
        .current_epoch         (current_epoch)
    );

    // prefetch start trigger: when shadow_config.valid and not already busy
    logic prefetch_start = shadow_config.valid && !prefetch_busy;

    prefetch_dma u_prefetch_dma (
        .clk               (clk),
        .rst_n             (rst_n),
        .shadow_config     (shadow_config),
        .current_epoch     (current_epoch),
        .start_prefetch    (prefetch_start),
        .busy              (prefetch_busy),
        .bytes_fetched     (bytes_fetched_w),
        .mem_req           (prefetch_req),
        .mem_req_valid     (prefetch_req_valid),
        .mem_req_ready     (prefetch_req_ready),
        .mem_resp          (prefetch_resp),
        .mem_resp_valid    (prefetch_resp_valid)
    );

    // memory arbiter
    memory_arbiter u_memory_arbiter (
        .clk                   (clk),
        .rst_n                 (rst_n),
        .active_req            (core_mem_req),
        .active_req_valid      (core_mem_req_valid),
        .active_req_ready      (core_mem_req_ready),
        .active_resp           (core_mem_resp),
        .active_resp_valid     (core_mem_resp_valid),
        .prefetch_req          (prefetch_req),
        .prefetch_req_valid    (prefetch_req_valid),
        .prefetch_req_ready    (prefetch_req_ready),
        .prefetch_resp         (prefetch_resp),
        .prefetch_resp_valid   (prefetch_resp_valid),
        .mem_master_req        (arb_to_dram_req),
        .mem_master_valid      (arb_to_dram_valid),
        .mem_master_ready      (mem_master_ready),
        .mem_master_resp       (mem_master_resp),
        .mem_master_resp_valid (mem_master_resp_valid),
        .no_outstanding_active (no_outstanding_active)
    );

    // connect arbiter master outputs to top-level DRAM outputs
    assign mem_master_req = arb_to_dram_req;
    assign mem_master_valid = arb_to_dram_valid;

endmodule
