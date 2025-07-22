// Kode ESP32 untuk Wokwi - Kirim Data pH, EC, Suhu, RH ke ThingSpeak + Kontrol Relay + Notifikasi Telegram (anti spam, encoded, logika kompleks)
#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"
#include <HTTPClient.h>
#include <time.h>

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int pH_pin = 34;
const int EC_pin = 35;
const int RELAY_EC = 13;

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WiFiClient client;
unsigned long channelID = 3001597;
const char* writeAPIKey = "Z2EW9NV06KRRXL6X";

String BOT_TOKEN = "8132236867:AAHZvQps1Sz5mBnoffMUfxZTLVQZ_NBAlKM";
String CHAT_ID = "1379884449";

bool lastPHLow = false;
bool lastECLow = false;

String urlencode(String str) {
  String encoded = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      code0 = ((c >> 4) & 0xf) + '0';
      if (((c >> 4) & 0xf) > 9) code0 = ((c >> 4) & 0xf) - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}

void sendTelegramAlertFormatted(float ph, float ec, float suhu, float rh) {
  String statusPH = (ph < 5.5) ? "⚠️ terlalu rendah" : "✅ Normal";
  String statusEC = (ec < 800) ? "⚠️ terlalu rendah" : "✅ Normal";

  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // waktu Asia/Jakarta
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  char buffer[30];
  snprintf(buffer, sizeof(buffer), "%02d-%02d-%04d %02d:%02d",
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
           t->tm_hour, t->tm_min);

  String msg = "🌱 Monitoring Nutrisi Tanaman (pH & EC)\n";
  msg += "Tanggal : " + String(buffer) + "\n";
  msg += "pH : " + String(ph, 2) + " (" + statusPH + ")\n";
  msg += "EC : " + String(ec, 0) + " µS/cm (" + statusEC + ")\n";
  msg += "Suhu : " + String(suhu, 1) + " °C\n";
  msg += "Kelembaban : " + String(rh, 1) + " %";

  String encodedMsg = urlencode(msg);
  String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + encodedMsg;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    http.GET();
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RELAY_EC, OUTPUT);
  digitalWrite(RELAY_EC, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");
  ThingSpeak.begin(client);
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  int adc_ph = analogRead(pH_pin);
  int adc_ec = analogRead(EC_pin);

  float voltage_ph = adc_ph * (3.3 / 4095.0);
  float ph_value = 3.5 * voltage_ph;

  float voltage_ec = adc_ec * (3.3 / 4095.0);
  float ec_value = voltage_ec * 1000;

  float suhu = dht.readTemperature();
  float rh = dht.readHumidity();

  Serial.print("pH: "); Serial.print(ph_value, 2);
  Serial.print(" | EC: "); Serial.print(ec_value, 0);
  Serial.print(" uS/cm | Suhu: "); Serial.print(suhu);
  Serial.print(" C | RH: "); Serial.println(rh);

  bool currentPHLow = ph_value < 5.5;
  bool currentECLow = ec_value < 800;

  // Kontrol relay
  digitalWrite(RELAY_EC, currentECLow ? HIGH : LOW);
  Serial.println(currentECLow ? "Relay EC AKTIF (EC rendah)" : "Relay EC Mati (EC cukup)");

  // Kirim notifikasi hanya jika status berubah
  if (currentPHLow != lastPHLow || currentECLow != lastECLow) {
    Serial.println("mengirim notif ke telegram");
    sendTelegramAlertFormatted(ph_value, ec_value, suhu, rh);
    lastPHLow = currentPHLow;
    lastECLow = currentECLow;
  }

  ThingSpeak.setField(1, ph_value);
  ThingSpeak.setField(2, ec_value);
  ThingSpeak.setField(3, suhu);
  ThingSpeak.setField(4, rh);

  int res = ThingSpeak.writeFields(channelID, writeAPIKey);
  if (res == 200) {
    Serial.println("Update ThingSpeak sukses\n");
  } else {
    Serial.println("Gagal update: " + String(res));
  }

  delay(15000);
}
