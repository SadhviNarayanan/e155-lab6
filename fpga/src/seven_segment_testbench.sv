// E155, Lab 2 - Testbench to test seven segment display

// Name: Sadhvi Narayanan
// Email: sanarayanan@g.hmc.edu
// Date: 09/03/2025

module seven_segment_testbench();

  logic        clk;
  logic		   reset;
  
  logic [3:0] s;
  logic [6:0] seg;
  
  logic [6:0]  actual, expected;
  logic [31:0] vectornum, errors;
  logic [10:0] testvectors[16:0];


  // instantiate device to be tested
  seven_segment dut(s, seg);
  
  // generate clock
  always 
    begin
      clk = 1; #5; clk = 0; #5;
    end

  // at start of test, load vectors
  // and pulse reset
  initial
    begin
      $readmemb("testvectors_seven_seg.tv", testvectors);
      vectornum = 0; errors = 0;
      reset = 1; #22; reset = 0;
    end
	 
  // apply test vectors on rising edge of clk
  always @(posedge clk)
    begin
      #1; {s, expected} = testvectors[vectornum];
    end

  // check results on falling edge of clk
  always @(negedge clk)
    if (~reset) begin // skip cycles during reset
      actual = {seg};
      
      if (actual !== expected) begin  // check result
        $display("Error on vector %d: inputs: s = %b outputs = %b (%b expected)", 
			  vectornum, s, actual, expected);
	      errors = errors + 1;
      end

      vectornum = vectornum + 1;

      if (vectornum == 32'd15) begin 
        $display("%d tests completed with %d errors", 
	         vectornum, errors);
        $stop;
      end
    end

endmodule


