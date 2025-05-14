#include "logos.h"

#ifndef FPSTR
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif

void printArduinoFetch() {
#if defined(ESP8266)
  Serial.println(FPSTR(LOGO_ESPRESSIF));
  Serial.println("Board     : ESP8266");
  Serial.print("Chip ID   : ");
  Serial.println(ESP.getChipId());
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024);
  Serial.println(" KB");
  Serial.print("SDK       : ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("Core Ver  : ");
  Serial.println(ESP.getCoreVersion());
#elif defined(ESP32)
  Serial.println(FPSTR(LOGO_ESPRESSIF));
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  Serial.println("Board     : ESP32");
  Serial.print("Cores     : ");
  Serial.println(chip_info.cores);
  Serial.print("Revision  : ");
  Serial.println(chip_info.revision);
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024);
  Serial.println(" KB");
  Serial.print("SDK       : ");
  Serial.println(esp_get_idf_version());
#elif defined(ARDUINO_AVR_UNO)
  Serial.println(FPSTR(LOGO_ARDUINO));
  Serial.println("Board     : Arduino Uno");
  Serial.println("MCU       : ATmega328P");
#elif defined(ARDUINO_AVR_MEGA2560)
  Serial.println(FPSTR(LOGO_ARDUINO));
  Serial.println("Board     : Arduino Mega 2560");
  Serial.println("MCU       : ATmega2560");
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
  Serial.println("Board     : Raspberry Pi Pico W");
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
  Serial.println("Board     : Raspberry Pi Pico");
  Serial.println("MCU       : RP2040 (Dual Core Arm Cortex-M0+)");
  Serial.print("Cores     : ");
  Serial.println(2);
  Serial.print("Flash Size: ");
  Serial.print(PICO_FLASH_SIZE_BYTES / 1024);
  Serial.println(" KB");

  uint8_t chip_id[8];
  getUniqueID(chip_id);
  Serial.print("Flash ID  : ");
  for (int i = 1; i < 9; ++i) {
    if (chip_id[i] < 0x10) Serial.print("0");
    Serial.print(chip_id[i], HEX);
  }
  Serial.println();
#elif defined(ARDUINO_ARCH_RP2040)
  Serial.println("Board     : RP2040-based (unknown variant)");
  Serial.println("MCU       : RP2040 (Dual Core Arm Cortex-M0+)");
  Serial.print("Cores     : 2");
  Serial.print("Flash Size: ");
  Serial.print(PICO_FLASH_SIZE_BYTES / 1024);
  Serial.println(" KB");

  uint8_t chip_id[8];
  getUniqueID(chip_id);
  Serial.print("Flash ID  : ");
  for (int i = 1; i < 9; ++i) {
    if (chip_id[i] < 0x10) Serial.print("0");
    Serial.print(chip_id[i], HEX);
  }
  Serial.println();
#else
  Serial.println("Board     : Unknown");
#endif

  Serial.print("Sketch    : ");
  Serial.println(__FILE__);
  Serial.print("Compiled  : ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  printArduinoFetch();
}

void loop() {}