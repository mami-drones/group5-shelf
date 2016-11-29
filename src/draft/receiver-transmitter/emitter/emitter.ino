#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

int out1=4444;
int out2=6996;

RF24 radio(9,10);
const uint64_t pipe00 = 0xE8E8F0F0E1LL;
void setup() {
 Serial.begin(57600);
radio.begin();
radio.openWritingPipe(pipe00);
}

void loop() {
  radio.write(&out1,sizeof(out1));
  radio.write(&out2,sizeof(out2));
}

