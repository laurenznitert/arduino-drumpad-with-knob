#include <EEPROM.h>

#define CONFIG_VERSION "ls1"
#define CONFIG_START 32

struct Settings {
  char version[4];
  int selectedColorIndex;
  //int colors[MAXCOLORS][3];
  int brightness;
};

Settings settings;

void setupSettings() {
}

void loopSettings() {
}

void loadDefaults() {
  #if debug
  Serial.println("LOADING DEFAULTS ....");
  #endif
  //
  // settings.version[0] = CONFIG_VERSION[0];
  // settings.version[1] = CONFIG_VERSION[1];
  // settings.version[2] = CONFIG_VERSION[2];
  // settings.version[3] = CONFIG_VERSION[3];
  //
  // settings.colors[0][0] = 0x11;
  // settings.colors[0][1] = 0x00;
  // settings.colors[0][2] = 0x00;
  //
  // settings.colors[1][0] = 0x00;
  // settings.colors[1][1] = 0x11;
  // settings.colors[1][2] = 0x00;
  //
  // settings.colors[2][0] = 0x00;
  // settings.colors[2][1] = 0x00;
  // settings.colors[2][2] = 0x11;
  //
  // settings.colors[3][0] = 0x22;
  // settings.colors[3][1] = 0x00;
  // settings.colors[3][2] = 0x00;
  //
  // settings.colors[4][0] = 0x00;
  // settings.colors[4][1] = 0x22;
  // settings.colors[4][2] = 0x00;
  //
  // settings.colors[5][0] = 0x00;
  // settings.colors[5][1] = 0x00;
  // settings.colors[5][2] = 0x22;
  //
  // settings.colors[6][0] = 0x33;
  // settings.colors[6][1] = 0x00;
  // settings.colors[6][2] = 0x00;
  //
  // settings.colors[7][0] = 0x00;
  // settings.colors[7][1] = 0x33;
  // settings.colors[7][2] = 0x00;
  //
  // settings.colors[8][0] = 0x00;
  // settings.colors[8][1] = 0x00;
  // settings.colors[8][2] = 0x33;
  //
  // settings.colors[9][0] = 0x11;
  // settings.colors[9][1] = 0x11;
  // settings.colors[9][2] = 0x11;
  //
  // settings.brightness = 100;
  // settings.selectedColorIndex = 0;
}

void loadConfig() {
  #if debug
  Serial.println("LOADING CONFIG ....");
  #endif

  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2])
    for (unsigned int t=0; t<sizeof(settings); t++)
      *((char*)&settings + t) = EEPROM.read(CONFIG_START + t);
}

void saveConfig() {
  #if debug
  Serial.println("SAVING CONFIG ....");
  #endif

  for (unsigned int t=0; t<sizeof(settings); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&settings + t));
}
