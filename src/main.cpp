#include <Arduino.h>
#include <BleKeyboard.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ezButton.h>
#include <jled.h>

#include "config.h"
#include "hardware.h"
#include "ota.h"

ezButton button1(KEY_1);
ezButton button2(KEY_2);
ezButton button3(KEY_3);
ezButton button4(KEY_4);
ezButton button5(KEY_5);
ezButton button6(KEY_6);

auto led1 = JLed(jled::Esp32Hal(LED_BUILTIN, 6));
auto led2 = JLed(jled::Esp32Hal(LED_EXTERNAL, 7));

OTA ota;
BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);

  // IO
  led1.FadeOn(250);
  led2.Candle().Forever();
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  button4.setDebounceTime(50);
  button5.setDebounceTime(50);
  button6.setDebounceTime(50);

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

  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();
  button5.loop();
  button6.loop();

  if (button1.isPressed()) {
    Config::sendPW1(bleKeyboard);
  }
  if (button2.isPressed()) {
    Config::sendPW2(bleKeyboard);
  }
  if (button3.isPressed()) {
    Config::sendPW3(bleKeyboard);
  }
  if (button4.isPressed()) {
    Config::sendPW4(bleKeyboard);
  }
  if (button5.isPressed()) {
    Config::sendPW5(bleKeyboard);
  }
  if (button6.isPressed()) {
    Config::sendPW6(bleKeyboard);
  }
  if (button2.isPressed()) {
    Config::sendPW2(bleKeyboard);
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
