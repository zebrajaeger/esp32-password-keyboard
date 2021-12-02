#pragma once

#include <Arduino.h>
#include <BleKeyboard.h>

class Config {
 public:
  static const char *getWifiSSID();
  static const char *getWifiPassword();
  static void sendPW1(BleKeyboard &bleKeyboard);
  static void sendPW2(BleKeyboard &bleKeyboard);
  static void sendPW3(BleKeyboard &bleKeyboard);
  static void sendPW4(BleKeyboard &bleKeyboard);
  static void sendPW5(BleKeyboard &bleKeyboard);
  static void sendPW6(BleKeyboard &bleKeyboard);

 private:
  static void sendKeycode(BleKeyboard &bleKeyboard, uint8_t keyCode) {
    KeyReport keyReport;
    
    keyReport.modifiers = 0;
    keyReport.reserved = 0;
    keyReport.keys[0] = keyCode;
    keyReport.keys[1] = 0;
    keyReport.keys[2] = 0;
    keyReport.keys[3] = 0;
    keyReport.keys[4] = 0;
    keyReport.keys[5] = 0;
    bleKeyboard.sendReport(&keyReport);

    keyReport.modifiers = 0;
    keyReport.reserved = 0;
    keyReport.keys[0] = 0;
    keyReport.keys[1] = 0;
    keyReport.keys[2] = 0;
    keyReport.keys[3] = 0;
    keyReport.keys[4] = 0;
    keyReport.keys[5] = 0;
    keyReport.keys[0] = 0;
    bleKeyboard.sendReport(&keyReport);
  }
};
