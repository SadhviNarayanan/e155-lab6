// STM32L432KC_SPI.c
// Sadhvi Narayanan
// sanarayanan@g.hmc.edu
// 10/18/2025
// Initializes SPI, and sends and receives data on SPI bus
#include "STM32L432KC_SPI.h"

void initSPI(int br, int cpol, int cpha) {
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br); // BR

    SPI1->CR1 &= ~(1 << 1); // clock polarity
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);

    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha); // clock phase

    SPI1->CR1 &= ~(SPI_CR1_BIDIMODE); // BIDIMODE -> unidirectional
    SPI1->CR1 &= ~(SPI_CR1_RXONLY); // RXONLY --> fully duplex
    SPI1->CR1 &= ~(SPI_CR1_LSBFIRST); // send and recv with MSB first

    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSI, 1); // sl*** select management
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSM, 1);


    SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 1); // set MCU as master

    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); // set data size to 8 bits

    SPI1->CR2 &= ~(SPI_CR2_SSOE); // SSOE disable to control from software
    SPI1->CR2 &= ~(SPI_CR2_FRF); // motorola
    SPI1->CR2 &= ~(SPI_CR2_NSSP); // for manual CS pulse generation

    SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 1); // set FIFO size to 8 bits


    SPI1->CR1 |= _VAL2FLD(SPI_CR1_SPE, 1); // SPI enable
}


char spiSendReceive(char send) {
    while(!(SPI1->SR & SPI_SR_TXE)); // wait until transmit buffer is empty
    *(volatile char *) (&SPI1->DR) = send; // set the data register
    while(!(SPI1->SR & SPI_SR_RXNE)); // wait until receive buffer not empty
    volatile char data = (volatile char) SPI1->DR; // read data
    return(data);
}