/*
* IR Sensor Code
* by Adam Müller, 2022
*
* AMG class. Handling the AMG8833 sensor and sending the data.
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

#include "amg_class.h"

// Init the sensor as well as espnow
void amg::init()
{
  amg.begin(105);    // if 105 doesnt work you might need 104 (if solderjumper set)

	// init espnow
  hespnow.connect(broadcastAddress);
}

// Read the sensor and send the data
void amg::read()
{
  //read all the pixels
  amg.readPixels(pixels);

	// send one pixel at the time such as not to overflow the messagebuffer and send even when some pixels are missing
  for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
    out = pixels[i-1];
    sprintf(new_out, "%i:%.2f", i-1, out);

		// send espnow message
    hespnow.send(new_out, strlen(new_out));
  }
}