//TRANSMITER


#include <VirtualWire.h>
//Arduino pin numbers
const int SW_pin = 2; //digital pin connected to switch output
const int X_pin = 1; //analog pin connected to x output
const int Y_pin = 0; //analog pin connected to y output

int numbers[2];

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(115200);

  pinMode(13, OUTPUT);

  pinMode(12, OUTPUT);
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(12);
  vw_setup(1000);
}

void loop () {

  double Y_pin_ = (double)analogRead(Y_pin) / 710.00000;
  double X_pin_ = (double)analogRead(X_pin) / 710.00000;

  numbers[0] = Y_pin_;
  numbers[1] = X_pin_;

  transmit();

  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(X_pin_, 10);
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.print(Y_pin_, 10);
  Serial.print("\n\n");
  delay(50);
}

void transmit() {
  digitalWrite(13, HIGH);
  vw_send( (uint8_t *)numbers, sizeof(numbers));
  vw_wait_tx();  // Wait for message to finish
  digitalWrite(13, LOW);
}
