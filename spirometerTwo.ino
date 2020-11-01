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

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
float area_1 = 0.000531;
float area_2 = 0.000201;
float rho = 1.225; //Demsity of air in kg/m3;
float dt = 0;
int newBreath = 0;
float TimerNow = 0.0;
int totalBreath = 0;
float massFlow = 0;
float volFlow = 0;
float volumeTotal = 0;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
float voltage = 0.0;
float Pa = 0.0;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  voltage = 3.3 * (( sensorValue + 0.5)/1024);
  Pa = (190 * voltage/3.3) - 38;
  if( Pa > 1 ) {
    if (newBreath < 1) {
      volumeTotal = 0;
      newBreath = 1;
      totalBreath = totalBreath + 1;
    }
  
    
  
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);
massFlow = 1000*sqrt((abs(Pa)*2*rho)/((1/(pow(area_2,2)))-(1/(pow(area_1,2)))));
volFlow = massFlow/rho; //volumetric flow of air
volumeTotal = volFlow * (millis() - TimerNow) + volumeTotal;
  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  //Serial.print(sensorValue);
  Serial.print("VolumeTotal= ");
  Serial.println(volumeTotal);
  Serial.print("\t pressure = ");
  Serial.println(Pa);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  
  } else if(newBreath) newBreath = 0;
  TimerNow = millis();
  delay(20);
}
