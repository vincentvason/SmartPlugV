/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "CE-ESL";
const char* password = "ceeslonly";

const char* host = "api.anto.io";
const int httpsPort = 80;

int modes;

void modeset() {
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

 
  String url = "/channel/get/iNpvCwemTctHOjx3F84nL6fUbuSg0zEyGdK2ZP97/SmartPlug/ModeSet";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"result\":\"true\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
//  Serial.println("reply was:");
//  Serial.println("==========");
//  Serial.println(line);
//  Serial.println("==========");
//  Serial.println("closing connection");

  int value = 0;
  char num1 = 0;
  int count = 0;
  int cha = 0;
  while (count < 7)
  {
    if(line[cha] == '\"') count++;
    cha++;
  }

  if(count == 7)
  {

      num1 = line[cha];
      value = num1 - '0';
  }

  cha++;
  if(line[cha] == '\"') count++;

  while(count == 7)
  {

      value = value*10;
      num1 = line[cha];
      value = value + (num1 - '0');
      cha++;
      if(line[cha] == '\"') count++;


  }
  modes = value;
  Serial.print("Mode: "); 
  Serial.println(modes);
}
void setup() {
  //D5 = 14 = RED
  //D6 = 12 = GREEN
  //D7 = 13 = BLUE
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}

void loop() {
  modeset();
  if(modes == 1) {digitalWrite(12, HIGH); digitalWrite(13, LOW); digitalWrite(14, LOW);}
  if(modes == 2) {digitalWrite(12, LOW); digitalWrite(13, HIGH); digitalWrite(14, LOW);}
  if(modes == 3) {digitalWrite(12, LOW); digitalWrite(13, HIGH); digitalWrite(14, HIGH);}
}