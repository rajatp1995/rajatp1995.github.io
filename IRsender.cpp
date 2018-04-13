#include <IRremote.h>

IRsend  irsend0;
IRsend1 irsend1;
IRsend2 irsend2;

void setup()
{
  Serial.begin(11500);
}

void loop() 
{
      irsend0.sendSony(0x01, 12); // pin 3
      irsend1.sendSony(0x02, 12); // pin 9
      irsend2.sendSony(0x03, 12); // pin 10
      delay(100);
}