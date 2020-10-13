// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>

const int SW_pin = 2; //digital pin connected to switch output
const int X_pin = 1; //analog pin connected to x output
const int Y_pin = 0; //analog pin connected to y output
const int BTN_pin = 9; //digital pin for button
String status_;

#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//RH_ASK driver;
RH_ASK driver(2000, 4, 12, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85),
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

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

pinMode(BTN_pin, INPUT_PULLUP);
//digitalWrite(BTN_pin, LOW);
}

void loop()
{
  String js_Y = (String)(analogRead(Y_pin) - 500);
  String js_X = (String)(analogRead(X_pin) - 508);

//  Serial.println(analogRead(BTN_pin));

//  if(analogRead(BTN_pin) >= 675 && analogRead(BTN_pin) <= 695) {
  if(digitalRead(BTN_pin)==LOW) {
//    Serial.println("On");
    status_ = "1";
  } else {
//    Serial.println("Off");
    status_ = "0";
  }

//  if (digitalRead(BTN_pin) == HIGH) {
//    // turn LED on:ser
//    Serial.println("on");
//  } else {
//    // turn LED off:
//    Serial.println("off");
//  }

  String s = js_X + ":" + js_Y + ":" + status_;
  Serial.println(s);

  const char *msg = s.c_str();;

  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();

//  Serial.println(s);
}
