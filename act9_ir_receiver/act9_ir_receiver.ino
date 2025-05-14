
/*
   Specify which protocol(s) should be used for decoding.
   If no protocol is defined, all protocols (except Bang&Olufsen) are active.
   This must be done before the #include <IRremote.hpp>
*/
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER
//#define DECODE_FAST

//#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
//#define DECODE_HASH         // special decoder for all protocols

//#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

//#define DEBUG               // Activate this for lots of lovely debug output from the decoders.

//#define RAW_BUFFER_LENGTH  180  // Default is 112 if DECODE_MAGIQUEST is enabled, otherwise 100.

#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 5

#define LED_1 8
#define LED_2 9
#define LED_3 10

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {

    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }

    Serial.println();
    IrReceiver.resume();

    if (IrReceiver.decodedIRData.command == 0xC) {
      const int val = !digitalRead(LED_1);
      digitalWrite(LED_1, val);
      Serial.print("LED 1: ");
      Serial.println(val ? "HIGH" : "LOW");
    } else if (IrReceiver.decodedIRData.command == 0x18) {
      const int val = !digitalRead(LED_2);
      digitalWrite(LED_2, val);
      Serial.print("LED 2: ");
      Serial.println(val ? "HIGH" : "LOW");
    } else if (IrReceiver.decodedIRData.command == 0x5E) {
      const int val = !digitalRead(LED_3);
      digitalWrite(LED_3, val);
      Serial.print("LED 3: ");
      Serial.println(val ? "HIGH" : "LOW");
    }
  }
}
