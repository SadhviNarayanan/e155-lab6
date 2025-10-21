// E155, Lab 2 - Clock divider which toggles at 60Hz

// Name: Sadhvi Narayanan
// Email: sanarayanan@g.hmc.edu
// Date: 09/03/2025

module clock(
	input logic clk,
	input logic reset,
	output logic clk_signal
);

  logic [23:0] counter;
  
  // clock is toggling at 60Hz
  always_ff @(posedge clk, negedge reset) begin
    if(reset == 0) begin
        counter <= 0;
        clk_signal <= 0;
    end else if (counter == 2000000) begin
        counter <= 0;
        clk_signal <= ~clk_signal; // toggles the ouput to create 60Hz frequency
   end else begin
       counter <= counter + 1;
   end
  end
 
endmodule