# IoT Monitoring Lingkungan dengan ESP8266, Firebase, dan ThingSpeak
Proyek ini menggunakan ESP8266 yang dihubungkan dengan beberapa sensor (DHT11, DS18B20, dan sensor kelembaban tanah) untuk memantau kondisi lingkungan. Data yang dikumpulkan akan dikirim ke Firebase dan ThingSpeak untuk monitoring dan analisis real-time.

# Komponen yang Digunakan

- ESP8266 (NodeMCU): Microcontroller WiFi.

- DHT11: Sensor untuk suhu dan kelembaban udara.

- DS18B20: Sensor suhu air.

- Soil Moisture Sensor: Sensor kelembaban tanah.

# Library yang Digunakan

- ESP8266WiFi: Koneksi WiFi.
  
- DHT: Membaca data dari sensor DHT11.

- ThingSpeak: Mengirim data ke ThingSpeak.

- OneWire & DallasTemperature: Untuk sensor DS18B20.

- FirebaseArduino: Menghubungkan ke Firebase Realtime Database.

# Data yang Diukur

- Suhu udara dan kelembaban dari DHT11.
  
- Suhu air dari DS18B20.

- Kelembaban tanah dari Soil Moisture Sensor.

# Langkah Instalasi

1. Persiapan Hardware
ESP8266: Sambungkan DHT11 ke pin D4, DS18B20 ke pin D3, dan sensor kelembaban tanah ke pin A0.

2. Setup Software
   
# Arduino IDE

Install ESP8266 melalui Boards Manager di Arduino IDE.

Install library yang dibutuhkan: ESP8266WiFi, DHT, ThingSpeak, OneWire, DallasTemperature, dan FirebaseArduino.

# Konfigurasi Kode

Masukkan URL Firebase dan API key:
#define FIREBASE_HOST "firebase-url-anda"
#define FIREBASE_AUTH "api-key-anda"

Masukkan API key dari ThingSpeak:
String apiKey = "api-key-thingspeak-anda";

Tambahkan SSID dan password WiFi:
const char* ssid = "nama-wifi-anda";
const char* pass = "password-wifi-anda";

Upload Kode:
Upload kode ke ESP8266 melalui Arduino IDE.

3. Monitoring
Lihat hasil di Serial Monitor Arduino IDE.
Data akan otomatis dikirim ke Firebase dan ThingSpeak.
