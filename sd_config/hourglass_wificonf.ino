#include <SPI.h>
#include <SD.h>
#include <ESP8266WiFi.h>

String ssid;
String password;

File log_file;

void process_wifi_config(File f) {
  for(int ch=f.read();ch !=-1 && ch!='\n'; ch=f.read()) {
    ssid += (char)ch;
  }
  for(int ch=f.read();ch!=-1&&ch!='\n';ch=f.read()) {
    password += (char)ch;
  }
}
void setup() {
  Serial.begin(9600);
  Serial.println("INIT");
  if(!SD.begin(4)) {
    Serial.println("unable to initialize SD card");
    return;
  }
  log_file = SD.open("hgl_log.txt", FILE_WRITE);
  if(!log_file) {
    Serial.println("UNABLE TO OPEN LOG FILE!");
  }
  log_file.println("Booting up");
  process_wifi_config(SD.open("WIFI.TXT"));
  Serial.println("SSID:");
  Serial.println(ssid);
  Serial.println("PASSWORD:");
  Serial.println(password);
  log_file.print("attempting to connect to ");
  log_file.println(ssid);
  log_file.flush();

  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  log_file.println("Connection successful");
  log_file.flush();
  Serial.println();
  Serial.print("Connected, IP: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  log_file.print("Connected, IP: ");
  log_file.println(WiFi.localIP());
  log_file.flush();
  delay(5000);
}
