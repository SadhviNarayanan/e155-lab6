// E155, Lab 2 - Code to display two numbers on a time-multiplexed dual segment display

// Name: Sadhvi Narayanan
// Email: sanarayanan@g.hmc.edu
// Date: 09/03/2025


module lab2_sn(
    input logic reset,
    input logic [3:0] s1,
    input logic [3:0] s2,
    output logic [4:0] led,
    output logic [1:0] enable_seg,
    output logic [6:0] seg
);


  logic int_osc;
  logic [23:0] counter;
  logic clk_signal;
  logic [3:0] s, s_bar1;
  logic [6:0] seg1, seg2;
 
  assign s_bar1 = ~s1; // need to flip this due to the inverted nature of the DIP switches
  
   // Internal high-speed oscillator
  HSOSC #(.CLKHF_DIV(2'b01))
        hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
		
   // toggles the clock at 60Hz - frequency where humans cannot see flickers
  clock clock_divider(int_osc, reset, clk_signal);


  assign led = s_bar1 + s2; // led has the sum of s1 and s2
 
  seven_segment seven_segment_decoder2(s, seg);


  always_comb begin
       if (clk_signal == 0) begin
       // determines which pnp transistor to provide a load to
	    enable_seg = 2'b10; // enable first 7-seg
	    s = s_bar1; // chooses first DIP switch
            
       end else begin
        // determines which pnp transistor to provide a load to
	    enable_seg = 2'b01; // enable second 7-seg
	    s = s2; // chooses second DIP switch
       end
   end
 endmodule



