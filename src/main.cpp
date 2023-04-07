#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;    //create http client

// credentials of my wifi hotspot
const char* SSID = "T";
const char* PASSWORD = "25569556";
const int DELAY = 5000;

// the ip adress of the server with the endpoint
const char* serverUrl = "http://192.168.2.91:8080/temperature";
const String DEVICE = "E1P1";   //device name 

//variables declaration
float temperature = 0, humidite = 0;

//get temperature with random
float getTemperature() {
  return random(2000, 3000) / 100.0;
}

//get humudity with random
float getHumidite() {
  return random(3000, 5000) / 100.0;
}


void setup() {
  Serial.begin(9600);

  // Connect to the wifi network with SSID and PASSWORD
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Attempting to connect to the wifi");
  }
  Serial.println("Connected to WiFi...OK!");
}

void loop() {
  temperature = getTemperature(); //get humity and temperature data
  humidite    = getHumidite();

  //send the data   
  http.begin(serverUrl);      //start connecting to the server
  http.addHeader("Content-Type", "application/json");     //specify the message type json

  //build the json message
  String requestBody = String("{\"temperature\": ") + temperature + String(", \"humidite\": ") + humidite + String(", \"device\": \"") + DEVICE + String("\"}");  
  int httpResponseCode = http.POST(requestBody);    //send the post request
  
  //print the http response
  if (httpResponseCode > 0) {
    Serial.printf("HTTP POST request succeeded with status %d\n", httpResponseCode);
  } else {
    Serial.printf("HTTP POST request failed with status %d\n", httpResponseCode);
  }
  http.end();   //close the http connection

  delay(DELAY); // wait 5 seconds before sending the next temperature data
}


