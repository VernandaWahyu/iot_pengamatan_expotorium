// Library pada Arduino Framework IDE
#include <ESP8266WiFi.h>         //Menambahkan Library untuk menghubungkan ESP8266 ke jaringan WiFi
#include <SPI.h>                 //Menambahkan Library untuk komunikasi melalui protokol SPI.
#include <Wire.h>                //Menambahkan Library untuk komunikasi melalui I2C
#include <ThingSpeak.h>          //Menambahkan Library untuk mengirim data ke ThingSpeak        
#include <DHT.h>                 //Menambahkan Library Untuk Sensor DHT11/22
#include <OneWire.h>             //Menambahkan Library Untuk Sensor  DS18B20
#include <DallasTemperature.h>   //Menambahkan Library Tambahan  untuk Sensor DS18B20
#include <FirebaseArduino.h>     //Menambahkan Library untuk Firebase

// Mendefinisikan PinSensor yang digunakan
#define DHTPIN D4 // Pin pada ESP8266 yang terhubung dengan sensor DHT
DHT dht(DHTPIN, DHT11);  // Inisialisasi sensor  yang digunakan yaitu sensor DHT11
#define FIREBASE_HOST "data-pengamatan-sistem-otomasi-default-rtdb.asia-southeast1.firebasedatabase.app"//URL yang terdapat pada Firebase
#define FIREBASE_AUTH "A5B2aPJaeCzIXh5PRZP3GetBNDW4RX1LA7UHx4u4"//API KEY yang terdapat pada Firebase

const int oneWireBus = D3;             //Pin pada ESP8266 yang terhubung dengan sensor Akuator(DS18B20 )
OneWire oneWire(oneWireBus);          //Untuk kebutuhan sensor DS18B20 
DallasTemperature sensors(&oneWire);  //Untuk kebutuhan sensor DS18B20 

String apiKey = "FXWW7UCEH39S8UBM"; //Api key yang terdapat pada thingspeak
const char* ssid = "KHUSUS GAME"; //Nama jaringan WiFi yang akan di hubungkan pada ESP8266
const char* pass = "Simbadda#22"; //Password jaringan wifi (jika tidak ada password bisa di kosongi)
const char* server = "api.thingspeak.com";  // Alamat server Thingspeak

const int AirValue = 790;  // Nilai bacaan sensor ketika di udara
const int WaterValue = 390;  // Nilai bacaan sensor ketika di air
const int SensorPin = A0;  // Pin pada ESP8266 yang terhubung dengan sensor Soilmosture
int soilMoistureValue = 0;  // Nilai bacaan sensor tanah
int soilMoisturePercent = 0;  // Nilai kelembaban tanah dalam persen

float temperatureWater = 0;  // Variabel untuk menyimpan nilai suhu air

WiFiClient client;  // Objek WiFi Client untuk koneksi ke Thingspeak

void setup() {
  Serial.begin(115200);//Serial monitor untuk menjalankan progra  m
  sensors.begin();            //Inisialisasi untuk Sensor DS18B20
  dht.begin();  // Memulai sensor DHT11
  WiFi.begin(ssid, pass);  // Memulai terhubung ke jaringan WiFi
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);//Menghubungkan URL dan API key pada firebase
  while (WiFi.status() != WL_CONNECTED) {     // Proses ESP8266 terhubung ke WiFi
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Terhubung Ke WiFi");   // Terhubung ke jaringan WiFi
  delay(4000);
}

void loop() {
  float h = dht.readHumidity();  // Membaca kelembaban udara
  float t = dht.readTemperature();  // Membaca suhu

  sensors.requestTemperatures();                    //--Cek sensor Akuator(DS18B20)
  float temperatureC = sensors.getTempCByIndex(0);  //--Ambil Data sensor DS18B20 dalam satuan Celcius

  temperatureWater = temperatureC;  // Menyimpan nilai suhu air ke variabel temperatureWater

  soilMoistureValue = analogRead(SensorPin);  // Membaca nilai sensor tanah dari pin yang telah terdapat pada variabel SensorPin

  soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);  // Mengkonversi nilai sensor menjadi persentase kelembaban tanah

  if (soilMoisturePercent > 100) { // Jika soil mosture di atas 100% 
    soilMoisturePercent = 100;  // Display pada serial monitor yaitu 100%
    // Tunggu delay 250 milliseconds
    delay(250);
  }
  else if (soilMoisturePercent < 0) {   // Jika soil mosture di bawah 0% 
    soilMoisturePercent = 0;  // Display pada serial monitor yaitu 0%
    // Tunggu delay 250 milliseconds
    delay(250);
  }
  else if (soilMoisturePercent <100 && soilMoisturePercent >0) { //Lainya jika sensor di bawah 100% dan Diatas 0%
    // Tunggu delay 250 milliseconds
    delay(250);
  }
  //Memulai display pada serial monitor
  Serial.print("Suhu Udara: ");
  Serial.print(t);
  Serial.print(" C ");
  Serial.print("Kelembaban Udara: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.print("Kelembaban Tanah: ");
  Serial.print(soilMoisturePercent);
  Serial.print(" % ");
  Serial.print("Suhu Air: ");
  Serial.print(temperatureWater);
  Serial.println("C");
  //Selesai  display pada serial monitor
   //Memulai display pada firebase
  Firebase.setFloat("Suhu Udara", t);
  Firebase.setFloat("Kelembapan Udara", h);
  Firebase.setFloat("Kelembapan Tanah", soilMoisturePercent);
  Firebase.setFloat("Suhu Air", temperatureWater);
   // Tunggu beberapa saat agar data terinput ke Firebase
  Serial.println("Input Data Ke Firebase"); //
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:"); //Display pada serial monitor jika firebase salah
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  //Selesai  display pada Firebase dengan delay 1000 millisecond
  if (client.connect(server, 80)) { //Mengconnect kan ke server
    String postStr = apiKey; //Mengambil data API Key untuk thingspeak
    //Memulai display pada Thingspeak
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "&field3=";
    postStr += String(soilMoisturePercent);
    postStr += "&field4=";
    postStr += String(temperatureWater);
    postStr += "\r\n\r\n\r\n\r\n";
    //Selesai  display pada Thingspeak

    // Mengirim data ke Thingspeak
    client.print("POST /update HTTP/1.1\n");      // Mengirim permintaan POST ke alamat URL "/update" menggunakan protokol HTTP versi 1.1.
    client.print("Host: api.thingspeak.com\n");   // Menentukan tujuan permintaan adalah "api.thingspeak.com".
    client.print("Connection: close\n");          // Menentukan bahwa koneksi akan ditutup setelah permintaan selesai.
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");  // Mengirim API key sebagai header permintaan.
    client.print("Content-Type: application/x-www-form-urlencoded\n");   // Menentukan tipe konten permintaan adalah "application/x-www-form-urlencoded".
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(2000);                   // Tunggu beberapa saat agar data terinput ke thingspeak
    Serial.println("Input Data Ke Thingspeak");
  }
  client.stop();
  // Delay 3 detik sebelum mengulangi loop
  delay(3000);
}
