#include <Arduino.h>

#include "en_class.h"

//String espnow::request_temperatures(float* result, int numSensors, DallasTemperature sensors)
//{
//  sensors.requestTemperatures(); 
//  if(numSensors == 3)
//  {
//    result[0] = sensors.getTempCByIndex(0);
//    result[1] = sensors.getTempCByIndex(1);
//    result[2] = sensors.getTempCByIndex(2);
//  }
//  else if(numSensors == 2)
//  {
//    result[0] = sensors.getTempCByIndex(3);
//    result[1] = sensors.getTempCByIndex(4);
//  }
//
//  return espnow::concat_result(result, numSensors);
//}
//
//String espnow::concat_result(float* result, int numSensors)
//{
//  String resultString;
//  for(int i=0; i<numSensors; i++)
//  {
//    resultString = resultString + ' ' + result[i];
//  }
//
//  return resultString;
//}