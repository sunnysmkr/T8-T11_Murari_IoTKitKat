#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <params.h>
#include <secret.h>
#include <UrusanWiFi.h>
#include <UrusanBuzzer.h>
#include <UrusanIoT.h>
#include <ArduinoJson.h>
#include <UrusanLED.h>
#include <DHT.h>


// Tulis parameter lokal mu di sini
const uint8_t pinLED = 27;
const uint8_t pinDHT = 4;
String clientId = String(CURRENT_FIRMWARE_TITLE) + String(NIM);
unsigned long lastSent = 0;
bool flagUpdate = false;
float lastTemp = 0;
float lastHumid = 0;


float tempBottomThreshold = 17;
float tempUpperThreshold = 40;
float humidBottomThreshold = 20;
float humidUpperThreshold = 95;


// Tulis instansiasi objek global mu di sini
UrusanWiFi urusanWiFi(wssid, wpass);
UrusanLED urusanLED(pinLED, pinLED, pinLED); // LED RGB dengan warna putih
UrusanIoT urusanIoT(tbAddr, tbPort, clientId.c_str(), "latihan", "bukanbudakai");
DHT dht(pinDHT, DHT22);


// Tulis deklarasi fungsi mu di sini:
void cetakIdentitasDeveloper();
void penangkapPesan(String topic, String message);
void subscribe();


#endif