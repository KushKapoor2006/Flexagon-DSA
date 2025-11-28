// flexpipe_pkg.sv
// Yosys-friendly header: macros + typedefs (included by RTL source files).
// This replaces the previous 'package flexpipe_pkg' approach to avoid import/order issues.

`ifndef FLEXPIPE_PKG_SV
`define FLEXPIPE_PKG_SV

// ----- basic numeric macros -----
`define CONFIG_BUS_WIDTH_BYTES 4
`define CONFIG_BUS_WIDTH_BITS (`CONFIG_BUS_WIDTH_BYTES * 8)
`define ADDR_WIDTH 64
`define DATA_WIDTH 256   // 256-bit DRAM beat = 32 bytes
`define MRN_DRAIN_CYCLES 50

`define MAX_OUTSTANDING_REQS 16
`define REQ_ID_WIDTH 6
`define EPOCH_WIDTH 4

// ----- layer config struct (packed) -----
typedef struct packed {
    logic [31:0] compute_cycles;
    logic [31:0] config_bits;
    logic [31:0] pointer_walks;
    logic [31:0] data_size;
    logic [`ADDR_WIDTH-1:0] base_addr;
    logic                  valid;
    logic [`EPOCH_WIDTH-1:0] epoch;
} layer_config_t;

// ----- request / response enums -----
typedef enum logic [1:0] {
    REQ_TYPE_CONFIG = 2'b00,
    REQ_TYPE_PTR    = 2'b01,
    REQ_TYPE_DATA   = 2'b10
} req_type_e;

typedef enum logic {
    PRIO_LOW  = 1'b0,
    PRIO_HIGH = 1'b1
} req_prio_e;

// ----- mem request and response payloads -----
typedef struct packed {
    logic [`DATA_WIDTH-1:0]   data;
    logic [`REQ_ID_WIDTH-1:0] id;
    logic [`EPOCH_WIDTH-1:0]  epoch;
    logic                    last;
    logic                    error;
} mem_resp_t;

typedef struct packed {
    logic [`ADDR_WIDTH-1:0]   addr;
    logic [31:0]             len;    // bytes
    logic [`REQ_ID_WIDTH-1:0] id;
    logic [`EPOCH_WIDTH-1:0]  epoch;
    req_type_e               rtype;
    req_prio_e               prio;
} mem_req_t;

`endif // FLEXPIPE_PKG_SV
