/*
File: Lab_6_JHB.c
Author: Josh Brake
Email: jbrake@hmc.edu
Date: 9/14/19
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Lab6 - SPI Communication between MCU and DS1722 Temp Sensor</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Lab6 - SPI Communication between MCU and DS1722 Temp Sensor</h1>";

char* ledStr = "<p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";

char* resStr = "<p>Resolution Control:</p><form action=\"12bit\"><input type=\"submit\" value=\"12-bit resolution\"></form>\
	<form action=\"11bit\"><input type=\"submit\" value=\"11-bit resolution\"></form>\
        <form action=\"10bit\"><input type=\"submit\" value=\"10-bit resolution\"></form>\
        <form action=\"9bit\"><input type=\"submit\" value=\"9-bit resolution\"></form>\
        <form action=\"8bit\"><input type=\"submit\" value=\"8-bit resolution\"></form>";

char* webpageEnd   = "</body></html>";

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

int updateLEDStatus(char request[])
{
	int led_status = 0;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		led_status = 0;
                return led_status;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		led_status = 1;
                return led_status;
	}

	// return led_status;
}

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////
char updateResStatus(char request[])
{
    // set resolution config register based on form input
    if (inString(request, "12bit")==1) {
            return(0xE8);
    }
    else if (inString(request, "11bit")==1) {
            return(0xE6); // 0b0110;
    }
    else if (inString(request, "10bit")==1) {
		return(0xE4); // 0b0100;
	}
    else if (inString(request, "9bit")==1) {
		return(0xE2); // 0b0010;
	}
    else if (inString(request, "8bit")==1) {
		return(0xE0); // 0b0000;
	}

    // return res_status;
}

void configurePins()
{
    pinMode(PB3, GPIO_ALT); //SLCK B3
    pinMode(PB4, GPIO_ALT); // MISO/CIPO B4
    pinMode(PB5, GPIO_ALT); // MOSI/COPI B5
    pinMode(PB1, GPIO_OUTPUT); // manual CS
    digitalWrite(PB1, 0); // set CS to low

    // now set AFRL channel connections for each pin to AF5 since that enables the SPI functionality we want
    GPIOB->AFR[0] &= ~((0xF << 12) | (0xF << 16) | (0xF << 20)); // using AFRL
    GPIOB->AFR[0] |=  ((5 << 12) | (5 << 16) | (5 << 20));

    // OSPEED for SPI
    GPIOB->OSPEEDR &= ~((3 << 6) | (3 << 8) | (3 << 10));
    GPIOB->OSPEEDR |=  ((3 << 6) | (3 << 8) | (3 << 10));
}


int main(void) {
  configureFlash();
  configureClock();

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(PA6, GPIO_OUTPUT);
  
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);

  RCC->APB2ENR |= (1 << 12); // ENABLE SPI1

  configurePins();
  
  USART_TypeDef * USART = initUSART(USART1_ID, 125000);

  // TODO: Add SPI initialization code
  initSPI(0b111, 0, 1); // TODO what polarity and phase?

  initTempSensor();

  while(1) {
    /* Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */

    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;
  
    // Keep going until you get end of line character
    while(inString(request, "\n") == -1) {
      // Wait for a complete request to be transmitted before processing
      while(!(USART->ISR & USART_ISR_RXNE));
      request[charIndex++] = readChar(USART);
    }

    // SPI code here for reading temperature
    char resStatus = updateResStatus(request);
    writeToTempSensor(resStatus);  // change resolution
  
    double temp = readFromTempSensor();

    // add temperature to a print buffer
    char tempStatusStr[32];
    sprintf(tempStatusStr,"Temperature is %0.4f C", (temp));

  
    // Update string with current LED state
    int led_status = updateLEDStatus(request);

    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");
    


    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code
    sendString(USART, ledStr); // button for controlling LED
    sendString(USART, resStr); // button for controlling Temp resolution

    sendString(USART, "<h2>LED Status</h2>");
    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");

    sendString(USART, "<h2>Temperature</h2>");
    sendString(USART, "<p>");
    sendString(USART, tempStatusStr);
    sendString(USART, "</p>");

    sendString(USART, webpageEnd);
  }
}



