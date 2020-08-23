// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12


#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver(2000, 12, 4, 0);

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *l_motor = AFMS.getMotor(1);
Adafruit_DCMotor *r_motor = AFMS.getMotor(2);


void setup()
{
#ifdef RH_HAVE_SERIAL
  Serial.begin(115200);    // Debugging only
#endif
  if (!driver.init())
#ifdef RH_HAVE_SERIAL
    Serial.println("init failed");
#else
    ;
#endif

AFMS.begin();
l_motor->setSpeed(150);
r_motor->setSpeed(150);

}

double left_power;
double right_power;

int multiplier = 2;

String x;
String y;

int ind1;
int ind2;

String readString;

void loop()
{
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Non-blocking
  {

    // Message with a good checksum received, dump it.
    //  driver.printBuffer("Got:", buf, buflen);

    String rec;
    for (int i = 0; i < buflen; i++) {
      rec += (char)buf[i];
    }

    readString = rec;

    String xval = getValue(rec, ':', 0);
    String yval = getValue(rec, ':', 1);

    double xd = xval.toDouble();
    double yd = yval.toDouble();

    left_power = (((xd + yd) / 523) * 50);
    right_power = (((xd - yd) / 515) * 50);

    Serial.println("X:" + xval + " Y:" + yval + " MOTOR POWERS: Left: " + left_power + " Right: " + right_power);

    if(left_power > 0) {
      l_motor->setSpeed(abs(left_power)*multiplier);
      l_motor->run(BACKWARD);
    } else {
      l_motor->setSpeed(abs(left_power)*multiplier);
      l_motor->run(FORWARD);
    }
    
    if(right_power > 0) {
      r_motor->setSpeed(abs(right_power)*multiplier);
      r_motor->run(FORWARD);
    } else {
      r_motor->setSpeed(abs(right_power)*multiplier);
      r_motor->run(BACKWARD);
    }

    //    STEPPER MOTORS
    //    lmotor->setSpeed(1000);
    //
//    if (left_power >= 5) {
//      // forward
//      //      lmotor->step(20,FORWARD,INTERLEAVE);
//      myMotor->run(FORWARD);
//    } else {
//      // backward
//      //      lmotor->step(0,BACKWARD,INTERLEAVE);
//      myMotor->run(BACKWARD);
//    }


    //Serial.print("  X:" + xval);

    //  y = rec.substring(0,rec.indexOf(',',rec.indexOf(',')+1));
    //  Serial.println("X: "+x+" Y: "+y);

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
