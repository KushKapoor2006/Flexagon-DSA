`include "flexpipe_pkg.sv"

// A tiny dummy core stub that can be used by the harness for simple tests.
// It does not drive external memory ports by default; harness can override signals.
module dummy_core (
    input  logic             clk,
    input  logic             rst_n,
    output logic [63:0]      compute_cycles_done,
    output logic             request_flip_ack
);
    // Produce simple deterministic outputs so lint doesn't complain
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            compute_cycles_done <= 64'd0;
            request_flip_ack <= 1'b0;
        end else begin
            compute_cycles_done <= compute_cycles_done + 1;
            request_flip_ack <= 1'b0;
        end
    end
endmodule
