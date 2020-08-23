//#include <VirtualWire.h>
//
//const int led_pin = 11;
//const int transmit_pin = 12;
////const int receive_pin = 2;
//const int transmit_en_pin = 3;
//
//void setup()
//{
//
//  vw_setup(2000);   // Bits per sec
//  // Initialise the IO and ISR
//  vw_set_tx_pin(transmit_pin);
////  vw_set_rx_pin(receive_pin);
////  vw_set_ptt_pin(transmit_en_pin);
////  vw_set_ptt_inverted(false); // Required for DR3100
//  
//}
//
//byte count = 1;
//
//void loop()
//{
//  for (int i = 0; i <= 255; i++) {
//  char msg[7] = {'h','e','l','l','o',' ','#'};
//
//  msg[6] = count;
//  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
//  vw_send((uint8_t *)msg, 7);
//  vw_wait_tx(); // Wait until the whole message is gone
//  digitalWrite(led_pin, LOW);
//  delay(1000);
//  count = count + 1;
//  }
//}





#include <VirtualWire.h>
void setup()
{
 vw_setup(2000); // Bits per sec
}
void loop()
{
 const char *msg = "hello";
 vw_send((uint8_t *)msg, strlen(msg));
 delay(400);
}




/// 4/26 on com 8
