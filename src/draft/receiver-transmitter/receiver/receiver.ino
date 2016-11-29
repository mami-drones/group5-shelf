
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9, 10);
const uint64_t pipe00 = 0xE8E8F0F0E1LL;

void setup() {
  Serial.begin(57600);
  radio.begin();

  radio.openReadingPipe(1, pipe00);
  radio.startListening();
}

void loop() {
  int in;

  if (radio.available()) {
    radio.read(&in, sizeof(in));
    if (in==4444){
      Serial.println("found!");
    }
    else if(in) {
      Serial.println(in);
    }
  }
}

