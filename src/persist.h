
#include <EEPROM.h>

char ssid[32] = "";
char password[32] = "";
int temp = 90;
int pressure = 9;

#define TEMP_ADDR 0
#define PRESS_ADDR 1
#define CRED_ADDR 2
#define CHECK_VAL 128

void load() {
  EEPROM.begin(512);
  temp = EEPROM.read(TEMP_ADDR);
  pressure = EEPROM.read(PRESS_ADDR);
  EEPROM.get(CRED_ADDR, ssid);
  EEPROM.get(CRED_ADDR+sizeof(ssid), password);
  int check = EEPROM.read(CRED_ADDR+sizeof(ssid)+sizeof(password));
  EEPROM.end();
  if (check!=CHECK_VAL) {
    ssid[0] = 0;
    password[0] = 0;
  }
}

/** Store WLAN credentials to EEPROM */
void store() {
  EEPROM.begin(512);
  EEPROM.write(TEMP_ADDR, temp);
  EEPROM.write(PRESS_ADDR, pressure);
  EEPROM.put(CRED_ADDR, ssid);
  EEPROM.put(CRED_ADDR+sizeof(ssid), password);
  EEPROM.write(CRED_ADDR+sizeof(ssid)+sizeof(password), CHECK_VAL);
  EEPROM.commit();
  EEPROM.end();
}