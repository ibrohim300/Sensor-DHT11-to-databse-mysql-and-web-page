
 
#include <ESP8266WiFi.h>
 
//Konfigurasi WiFi
const char *ssid = "MAKERINDO2";
const char *password = "makerindo2019";
 
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.1.23";
 
void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //Jika koneksi berhasil, maka akan muncul address di serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
int value = 0;
 
void loop() {
  // Proses Pengiriman -----------------------------------------------------------
  delay(1000);
  ++value;
 
  // Membaca Sensor Analog -------------------------------------------------------
  int datasensor=analogRead(A0); //Read Analog value of LDR
  Serial.println(datasensor);
 
  Serial.print("connecting to ");
  Serial.println(host);
 
// Mengirimkan ke alamat host dengan port 80 -----------------------------------
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 // DARI Program pak oscar
 String ADCData, station, getData, Link;
 ADCData = String(datasensor); //String to interger conversion
 station = "B";
 //GET Data
 getData = "?status=" + ADCData + "&station=" + station ; //Note "?" added at front
 
 
// Isi Konten yang dikirim adalah alamat ip si esp -----------------------------
  String url = "/etandur/getdemo.php";
  url += getData;
 
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
// Mengirimkan Request ke Server -----------------------------------------------
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 
// Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("closing connection");
}
