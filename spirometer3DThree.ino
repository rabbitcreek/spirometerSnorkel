/*
  Analog input, analog output, serial output
  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.
  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground
  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe
  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#include <SD.h>
#define cardSelect 4
File logfile;
char filename[15];
// These constants won't change. They're used to give names to the pins used:
const int analogInPin = 33;  // Analog input pin that the potentiometer is attached to
const int outPin = 9; // Analog output pin that the LED is attached to
float area_1 = 0.000531;
float area_2 = 0.000201;
float rho = 1.225; //Demsity of air in kg/m3;
float dt = 0;
int logCount = 0;
float timerBreath;
float secondsBreath;
float minuteTotal;
int newBreath = 0;
float TimerNow = 0.0;
int totalBreath = 0;
float massFlow = 0;
float volFlow = 0;
float maxFlow = 0.0;
float volumeTotal = 0;
float sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
float voltage = 0.0;
float Pa = 0.0;
float fev1 = 0.0;
float vol[20];
float volSec[20];
int snatch = 0;
int fundex = 0;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
  minuteTotal = millis();
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, HIGH);
  for(int p = 0;p < 20; p++){
    vol[p] = 0;
    volSec[p] = 0;
  }
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  /*
   if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
   
  }
  strcpy(filename, "ANALOG00.TXT");//this section sets up new files for each new reading
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
   logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    
  }
  
  Serial.print("Writing to "); 
  Serial.println(filename);
  */
}

void loop() {
  if((millis()- minuteTotal) > 60000){
    minuteTotal = millis();
    //Serial.print( "Breaths per minute:");
    //Serial.println(totalBreath);
   // logfile.print("                                        ");
    //logfile.println(totalBreath);
    totalBreath = 0;
  }
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  //sensorValue = sensorValue + 404;
  // map it to the range of the analog out:
//Serial.print("max sens value");
//Serial.println(sensorValue);
 
  voltage =( 3.3 * (( sensorValue )/4095));
  
  Pa = (190 * voltage/3.3) - 31;
     //Serial.print("pa");
     //Serial.println(Pa);
  if( Pa > 1 ) {
    if (newBreath < 1) {
      timerBreath = millis();
      fev1 = 0.0;
      snatch = 0;
      fundex = 0;
      if(volumeTotal > 100){
      totalBreath = totalBreath + 1;
      //Serial.print("VolumeTotal= ");
      //Serial.println(volumeTotal);
      }
      
      volumeTotal = 0;
      newBreath = 1;
      
     
    }
  
    if(!(snatch%5)){
      vol[fundex] = volumeTotal;
      volSec[fundex] = volFlow;
      fundex++;
      
    }
  snatch++;
  // change the analog out value:
  //analogWrite(analogOutPin, outputValue);
  if((millis() - timerBreath >= 1000) && (fev1 == 0)) fev1 = volumeTotal;
massFlow = 1000*sqrt((abs(Pa)*2*rho)/((1/(pow(area_2,2)))-(1/(pow(area_1,2)))));
volFlow = massFlow/rho; //volumetric flow of air
if(volFlow > maxFlow) maxFlow = volFlow;

volumeTotal = volFlow * (millis() - TimerNow) + volumeTotal;
  // print the results to the Serial Monitor:
  //Serial.print("sensor = ");
  //Serial.print(sensorValue);
  Serial.print("VolumeTotal= ");
 Serial.println(volumeTotal);
 
  //Serial.print("\t pressure = ");
  //Serial.println(Pa);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  
  } else if(newBreath){
    newBreath = 0;
    //Serial.print("time for breath");
    secondsBreath = (millis() - timerBreath)/1000;
    Serial.print("FEV1  ");
    Serial.println(fev1);
    Serial.print("FVC  ");
    Serial.print(volumeTotal);
    Serial.print("FV1/FVC");
    Serial.println(fev1/volumeTotal);
    Serial.print("Duration");
    Serial.println(timerBreath/1000);
    Serial.print("MaxFlow");
    Serial.println(maxFlow);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawCentreString("FEV1",32,20,4);
     tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawCentreString("FVC",32,80,4);
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.drawString("888888",70,5,7);
  tft.setTextColor(0xFBE0, TFT_BLACK); // Orange
  tft.drawNumber(fev1,70,5,7);
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.drawString("888888",70,70,7);
  tft.setTextColor(0xFBE0, TFT_BLACK); // Orange
  tft.drawNumber(volumeTotal,70,70,7);
    //Serial.println(secondsBreath);
    printer();
    for(int p = 0;p < 20; p++){
    vol[p] = 0;
    volSec[p] = 0;
  }
    maxFlow = 0;
  
    /*
    logfile.print(volumeTotal);
    logCount = logCount + 1;
    if (logCount%2) {
      digitalWrite(outPin, LOW);
    } else digitalWrite(outPin, HIGH);
    logfile.print("              ");
    logfile.println(secondsBreath);
    logfile.flush();
    */
  }
  
  TimerNow = millis();
  delay(20);
}
void printer(){
  for(int p = 0; p< 20; p++){
    Serial.print("vol");
    Serial.println(vol[p]);
    Serial.print("volspeed");
    Serial.println(volSec[p]);
  }
}
