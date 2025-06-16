#include <main.h>


void setup() {
  // tulis setup kode mu di sini, untuk dijalankan sekali saja:  
  Serial.begin(115200);
  cetakIdentitasDeveloper();


  urusanWiFi.konek();
  while(urusanWiFi.apakahKonek() == 0){
    Serial.print(".");
  }


  urusanIoT.konek();
  urusanIoT.penangkapPesan(penangkapPesan);


  if(urusanIoT.apakahKonek() == 1){
    subscribe();
  }


  dht.begin();


  urusanLED.setWarna(100, 100, 100);
  delay(3000);
  urusanLED.setWarna(0, 0, 0);
}


static unsigned long lastUpdateMillis = 0;
void loop() {
  // tulis kode utama mu di sini, untuk dijalankan berulang-ulang :
  urusanIoT.proses();


  if(urusanIoT.apakahKonek() == 0){
    Serial.println("UrusanIoT: Koneksi terputus, mencoba untuk menyambung kembali...");
    urusanIoT.konek();
    if(urusanIoT.apakahKonek() == 1){
      subscribe();
    }
  }


  if (millis() - lastUpdateMillis >= 1000) {
    lastUpdateMillis = millis();


    lastTemp = dht.readTemperature();
    lastHumid = dht.readHumidity();


    String payload;
    JsonDocument doc;
    doc["temp"] = lastTemp;
    doc["humid"] = lastHumid;
    serializeJson(doc, payload);


    urusanIoT.publish("kitkat/murari/status", payload);
  }


  // make the led blink if lastTemp is outside tempBottomThreshold and tempUpperThreshold
  if (lastTemp < tempBottomThreshold || lastTemp > tempUpperThreshold ||
      lastHumid < humidBottomThreshold || lastHumid > humidUpperThreshold) {  
    urusanLED.setWarna(100, 100, 100); // LED nyala
  } else {
    urusanLED.setWarna(0, 0, 0); // LED padam
  }
  delay(10);
}


void subscribe() {
  urusanIoT.subscribe("kitkat/murari/command");
}


void penangkapPesan(String topic, String message){
  Serial.printf("penangkapPesan: topic: %s | message: %s\r\n", topic.c_str(), message.c_str());
  JsonDocument dataMasuk;
  DeserializationError galatParseJson = deserializeJson(dataMasuk, message);
  if(galatParseJson == DeserializationError::Ok){
    if(!dataMasuk["tempBottomThreshold"].isNull() &&
    !dataMasuk["tempUpperThreshold"].isNull() &&
    !dataMasuk["humidBottomThreshold"].isNull() &&
    !dataMasuk["humidUpperThreshold"].isNull()){
      tempBottomThreshold = dataMasuk["tempBottomThreshold"].as<float>();
      tempUpperThreshold = dataMasuk["tempUpperThreshold"].as<float>();
      humidBottomThreshold = dataMasuk["humidBottomThreshold"].as<float>();
      humidUpperThreshold = dataMasuk["humidUpperThreshold"].as<float>();
    }
  }
  else
  {
    Serial.printf("penangkapPesan: Gagal parse JSON: %s\r\n", galatParseJson.c_str());
  }
}


// tulis definisi fungsi mu di sini:
void cetakIdentitasDeveloper() {
  Serial.printf("NIM: %d\r\n", NIM);  
  Serial.printf("Nama: %s\r\n", NAMA_LENGKAP);
  Serial.printf("Grup: %s\r\n", NAMA_KELOMPOK);
  Serial.printf("Firmware: %s %s\r\n", CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
}