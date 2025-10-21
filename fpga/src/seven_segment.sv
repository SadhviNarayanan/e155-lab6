// E155, Lab 2 Code to output a seven segment display based on hex values

// Name: Sadhvi Narayanan
// Email: sanarayanan@g.hmc.edu
// Date: 09/05/2025


// seven segment display module to convert a 4 bit hex into segments on a seven segment display
module seven_segment(
	input logic [3:0] s,
	output logic [6:0] seg
);
	// combinational cases to trigger seven segment display
	always_comb begin
		case(s)
			4'h0: seg = 7'b1000000;
			4'h1: seg = 7'b1001111;
			4'h2: seg = 7'b0100100;
			4'h3: seg = 7'b0110000;
			4'h4: seg = 7'b0011001;
			4'h5: seg = 7'b0010010;
			4'h6: seg = 7'b0000010;
			4'h7: seg = 7'b1111000;
			4'h8: seg = 7'b0000000;
			4'h9: seg = 7'b0011000;
			4'hA: seg = 7'b0001000;
			4'hB: seg = 7'b0000011;
			4'hC: seg = 7'b1000110;
			4'hD: seg = 7'b0100001;
			4'hE: seg = 7'b0000110;
			4'hF: seg = 7'b0001110;
			default: seg = 7'b1111111;
		endcase
	end
endmodule