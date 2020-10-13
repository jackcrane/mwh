// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <ServoTimer2.h>

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//RH_ASK driver;
RH_ASK driver(2000, 7, 4, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85),
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//Adafruit_StepperMotor *rmotor = AFMS.getStepper(200, 2);
//Adafruit_StepperMotor *lmotor = AFMS.getStepper(200, 1);
Adafruit_DCMotor *rmotor = AFMS.getMotor(2);
Adafruit_DCMotor *lmotor = AFMS.getMotor(1);

ServoTimer2 putter;

void setup()
{
#ifdef RH_HAVE_SERIAL
  Serial.begin(115200);	  // Debugging only
#endif
  if (!driver.init())
#ifdef RH_HAVE_SERIAL
    Serial.println("init failed");
#else
    ;
#endif

AFMS.begin();

putter.attach(8);

pinMode(10,OUTPUT);
}

double left_power;
double right_power;
double servo_power;

String x;
String y;

int ind1;
int ind2;

String readString;



void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
    //Serial.println("loop");
  if (driver.recv(buf, &buflen)) // Non-blocking
  {

    // Message with a good checksum received, dump it.
    //	driver.printBuffer("Got:", buf, buflen);

    String rec;
    for (int i = 0; i < buflen; i++) {
      rec += (char)buf[i];
    }

    //  Serial.println(rec.indexOf(','));

    //  index1 = rec.indexOf(',');
    //  x = rec.substring(0,index1);
    //
    //  index2 = rec.indexOf(',',index1+1);
    //  x = rec.substring(0,index2);

    readString = rec;
    Serial.println(rec);
//    Serial.println("loop");

    //  ind1 = readString.indexOf(',');  //finds location of first ,
    //  x = readString.substring(0, ind1);   //captures first data String
    //  ind2 = readString.indexOf(',', ind1+1 );   //finds location of second ,
    //  y = readString.substring(ind1+1, ind2+1);   //captures second data String

    String xval = getValue(rec, ':', 0);
    String yval = getValue(rec, ':', 1);
    String sval = getValue(rec, ':', 2);

    double xd = xval.toDouble();
    double yd = yval.toDouble();

    left_power = ((((xd + yd)/523)*50));
    right_power = ((((xd - yd)/515)*50));
    servo_power = sval.toDouble();
    
    Serial.println("X:" + (String)xd + " Y:" + (String)yd + " MOTOR POWERS: Left: " + left_power + " Right: " + right_power + " Servo: "+sval);
    
//    lmotor->setSpeed(abs(left_power) * 100);
    lmotor->setSpeed(100);
    rmotor->setSpeed(100);
    //Serial.print("  X:" + xval);

    //  y = rec.substring(0,rec.indexOf(',',rec.indexOf(',')+1));
    //  Serial.println("X: "+x+" Y: "+y);

  }
  //Serial.println("tick");
    if(left_power >= 5) {
      // forward
      lmotor->run(BACKWARD);
    } else if (left_power <= -5) {
      // backward
      lmotor->run(FORWARD);
    } else {
      lmotor->run(RELEASE);
    }
    
    if(right_power >= 5) {
      // forward
      rmotor->run(FORWARD);
    } else if (right_power <= -5) {
      // backward
      rmotor->run(BACKWARD);
    } else {
      rmotor->run(RELEASE);
    }

    if(servo_power == 1) {
      putter.write(2000);
      digitalWrite(10,LOW);
      Serial.println("SHOULD BE TURNING");
    } else {
      putter.write(1500);
      digitalWrite(10,HIGH);
    }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
