
// E155, Lab 2 - Testbench to test top level module - time multiplexing and led addition

// Name: Sadhvi Narayanan
// Email: sanarayanan@g.hmc.edu
// Date: 09/03/2025

`timescale 1ns / 1ps
module testbench_lab2_sn();

  logic        clk;
  logic		   reset;
  
  logic [7:0] counter;
  logic [3:0] s1, s2, s, s_bar1;
  logic [4:0] led;
  logic [1:0] enable_seg;
  logic [6:0] seg, exp_seg;
  logic start;
  
  initial counter = 8'b0;
  //initial s1 = 4'b0;
  //initial s2 = 4'b0;
  //initial led = 5'b10000;
  initial start = 0;
  


  // instantiate device to be tested
  lab2_sn dut(clk, reset, s1, s2, led, enable_seg, seg);
  
  // generate clock

  always 
    begin
      clk = 0; #10; clk = 1; #10;
    end

  initial
    begin
      reset = 0; #22; reset = 1; 
    end

  always
    begin
      #40000000;
      assert (enable_seg === 2'b10) else $display("ERROR on enbable, should be 10 but actuall %b", enable_seg);
      #40000000;
      assert (enable_seg === 2'b01) else $display("ERROR on enbable, should be 01 but actuall %b", enable_seg);
    end

    


  always @(*) begin #2
	  assert (led == (s_bar1 + s2)) else $display("LED is not correct %b, %b, %b, %b", led, s1, ~s1, s2);
	  assert (~reset || (seg == exp_seg)) else $display("expected segment is not correct %b, %b, %b, %b, %b, %b", enable_seg, s, s_bar1, s2, seg, exp_seg);
  end
  
  always @(posedge clk, negedge reset) begin
	  if (reset == 0) begin
		counter <= 8'b00000000;
		start <= 0;
          end else begin
	  	counter <= counter + 1;
	  	start <= 1;
	  end
  end
  
  assign {s1, s2} = counter;
  assign s_bar1 = ~s1;
  
  always_comb begin
	  if (enable_seg == 2'b10) s = s_bar1;
	  else if (enable_seg == 2'b01) s = s2;
	  else s = 4'bxxxx;
  end
  
  always_comb begin
	  case (s)
		4'b0000: exp_seg = 7'b1000000;
		4'b0001: exp_seg = 7'b1001111;
		4'b0010: exp_seg = 7'b0100100;
		4'b0011: exp_seg = 7'b0110000;
		4'b0100: exp_seg = 7'b0011001;
		4'b0101: exp_seg = 7'b0010010;
		4'b0110: exp_seg = 7'b0000010;
		4'b0111: exp_seg = 7'b1111000;
		4'b1000: exp_seg = 7'b0000000;
		4'b1001: exp_seg = 7'b0011000;
		4'b1010: exp_seg = 7'b0001000;
		4'b1011: exp_seg = 7'b0000011;
		4'b1100: exp_seg = 7'b1000110;
		4'b1101: exp_seg = 7'b0100001;
		4'b1110: exp_seg = 7'b0000110;
		4'b1111: exp_seg = 7'b0001110;
		default: exp_seg = 7'b1111111;
	  endcase
  end

  
		


endmodule




