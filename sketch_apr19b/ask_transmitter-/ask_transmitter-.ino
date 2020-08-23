#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif


RH_ASK driver(2000, 4, 12, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

const int SW_pin = 11; //digital pin connected to switch output
const int X_pin = 1; //analog pin connected to x output
const int Y_pin = 0; //analog pin connected to y output

void setup()
{
pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  
#ifdef RH_HAVE_SERIAL
    Serial.begin(115200);	  // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
	;
#endif
}

void loop()
{
double Y_pin_ = (double)analogRead(Y_pin) / 710.00000;
  double X_pin_ = (double)analogRead(X_pin) / 710.00000;

    Serial.println(analogRead(Y_pin));

    int X_pin_0 = analogRead(X_pin) - 500;
    int Y_pin_0 = analogRead(Y_pin) - 500;

    const String str_y = "(" + (String)X_pin_0 + "," + (String)Y_pin_0 + ")";

//    str_char_y = str_y.c_str();
  
    const char *msg = str_y.c_str();

//    const char* msg[] = {"/test", "/say", "/reset", "/sleep"};

//    Serial.println(msg);

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(2);
}
