#include <Arduino.h>
#include <BleKeyboard.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
// #include <ezButton.h>
// #include <Button.h>
#include <JC_Button.h>
#include <jled.h>

#include "config.h"
#include "hardware.h"
#include "ota.h"

#define DEBOUNCE_TIME_MS 50
Button button1(KEY_1, DEBOUNCE_TIME_MS);
Button button2(KEY_2, DEBOUNCE_TIME_MS);
Button button3(KEY_3, DEBOUNCE_TIME_MS);
Button button4(KEY_4, DEBOUNCE_TIME_MS);
Button button5(KEY_5, DEBOUNCE_TIME_MS);
Button button6(KEY_6, DEBOUNCE_TIME_MS);

auto led1 = JLed(jled::Esp32Hal(LED_BUILTIN, 4));
auto led2 = JLed(jled::Esp32Hal(LED_EXTERNAL, 5));

OTA ota;
BleKeyboard bleKeyboard("PWkeyboard");

void setup() {
  Serial.begin(115200);

  // IO
  button1.begin();
  button2.begin();
  button3.begin();
  button4.begin();
  button5.begin();
  button6.begin();

  led1.FadeOn(250);
  led2.Candle().Forever();

  // BT
  bleKeyboard.begin();

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(Config::getWifiSSID(), Config::getWifiPassword());
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    led1.Blink(200, 300).Forever();
    delay(5000);
    led1.Off();
    ESP.restart();
  }

  led1.Candle().Forever();
  ota.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  led1.FadeOff(250);
}

void loop() {
  ota.loop();

  button1.read();
  button2.read();
  button3.read();
  button4.read();
  button5.read();
  button6.read();

  if (button1.wasPressed()) {
    Config::sendPW1(bleKeyboard);
  }
  if (button2.wasPressed()) {
    Config::sendPW2(bleKeyboard);
  }
  if (button3.wasPressed()) {
    Config::sendPW3(bleKeyboard);
  }
  if (button4.wasPressed()) {
    Config::sendPW4(bleKeyboard);
  }
  if (button5.wasPressed()) {
    Config::sendPW5(bleKeyboard);
  }
  if (button6.wasPressed()) {
    Config::sendPW6(bleKeyboard);
  }

  led1.Update();
  led2.Update();
  bool ledOn = led1.IsRunning();
  bool bleOn = bleKeyboard.isConnected();
  if (ledOn && !bleOn) {
    led1.Stop();
  } else if (!ledOn && bleOn) {
    led1.Breathe(2000).Forever().DelayAfter(2000);
  }
}
