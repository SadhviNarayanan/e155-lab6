// DS1722.c
// Sadhvi Narayanan
// sanarayanan@g.hmc.edu
// 10/18/2025
// Configures the temperature sensor to calculate at a resolution and read the msb and lsb of the temperature continuously
#include "DS1722.h"

void initTempSensor() {
    digitalWrite(PB1, 1); // CS high
    spiSendReceive(0x80); // tell temp sensor we are writing to it for setup
    spiSendReceive(0xE8); // continuous calculation with 12-bit resolution (0xE8)
    digitalWrite(PB1, 0); // CS low
}

void writeToTempSensor(char resStatus) {
    digitalWrite(PB1, 1);
    if (resStatus == 0xE8) {
     spiSendReceive(0x80); // tell temp sensor we are writing to it for setup
     spiSendReceive(resStatus); // setup resolution
      delay_millis(TIM15, 900);
    }
    else if (resStatus == 0xE6) {
     spiSendReceive(0x80); // tell temp sensor we are writing to it for setup
     spiSendReceive(resStatus); // setup resolution
      delay_millis(TIM15, 600);
    }
    else if (resStatus == 0xE4) {
     spiSendReceive(0x80); // tell temp sensor we are writing to it for setup
     spiSendReceive(resStatus); // setup resolution
      delay_millis(TIM15, 300);
    }
    else if (resStatus == 0xE2) {
     spiSendReceive(0x80); // tell temp sensor we are writing to it for setup
     spiSendReceive(resStatus); // setup resolution
      delay_millis(TIM15, 150);
    }
    else if (resStatus == 0xE0) {
     spiSendReceive(0x80); // tell temp sensor we are writing to it for setup
     spiSendReceive(resStatus); // setup resolution
      delay_millis(TIM15, 75);
    }
    digitalWrite(PB1, 0); // CS low
}

double readFromTempSensor() {

    digitalWrite(PB1, 1); 
    spiSendReceive(0x00); // read from config register for testing purposes
    volatile char config_readback = spiSendReceive(0x00);
    digitalWrite(PB1, 0);


    digitalWrite(PB1, 1);
    spiSendReceive(0x02); // tell sensor we want to read MSB
    uint8_t msb = spiSendReceive(0x00); // get MSB out with 8 clk pulses
    digitalWrite(PB1, 0);


    digitalWrite(PB1, 1);
    spiSendReceive(0x01); // tell sensor we want to read LSB
    uint8_t lsb = spiSendReceive(0x00); // get LSB out with another 8 clk pulses
    digitalWrite(PB1, 0); // set CS to high
    
    // double temp2 = (double) msb + ((double) lsb / 256.0);
    // calculate temperature
    double temp2 = calc_temp(msb, lsb);

    return temp2;
}

double calc_temp(uint8_t msb, uint8_t lsb) {
  int sign = (msb >> 7) & 0x1; // extract sign
  double temp2;

  // for positive temperatures
  if (sign == 0) {
    temp2 = (double) msb + ((double) lsb / 256.0);
  }
  // calculation for negative temperatures
  else {
    uint16_t combined = (msb << 8) | lsb; // {msb, lsb}
    uint16_t twoscomp = ~(combined) + 1; // take 2's complement
    uint8_t msb_new = twoscomp >> 8;
    uint8_t lsb_new = twoscomp & 0x00FF;
    temp2 = (double) msb_new + ((double) lsb_new / 256.0);
    temp2 = temp2 * -1; // make negative
  }
  return temp2;

}