
  
/*
 * HTTPS Secured Client POST Request
 * Copyright (c) 2019, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */
 #include <DHT.h>
#define DHT11_PIN 2
DHT DHT(DHT11_PIN,DHT11);

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
// GPIO where the DS18B20 is connected to
const int oneWireBus = 13;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
 
/* Set these to your desired credentials. */
const char *ssid = "VIGOYA";  //ENTER YOUR WIFI SETTINGS
const char *password = "Johanna69";
 
//Link to read data from https://jsonplaceholder.typicode.com/comments?postId=7
//Web/Server address to read/write from 
const char *host = "999nmxzw1g.execute-api.us-east-1.amazonaws.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
 
//SHA1 finger print of certificate use web browser to view and copy
const char fingerprint[] PROGMEM = "AD 70 45 19 6C DB 0A A2 77 ED 73 7B 37 F7 A8 11 7F B5 2A C7";
//AD:70:45:19:6C:DB:0A:A2:77:ED:73:7B:37:F7:A8:11:7F:B5:2A:C7
//BA CB 48 CE 60 3E 14 B4 50 0F F2 C3 7E FA 82 05 29 DA FA E8
//=======================================================================
//                    Power on setup
//=======================================================================
 
void setup() {
  delay(1000);
  pinMode(5,INPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  DHT.begin();
  sensors.begin();
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  WiFiClientSecure httpsClient;    //Declare object of class WiFiClient
  
  Serial.println(host);
 
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000); // 15 Seconds
  delay(1000);
  
  Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  
  String getData, Link;
  
  //POST Data
  Link = "/API/rovyle-indexUserData";
 
  Serial.print("requesting URL: ");
  Serial.println(host);
  /*
   POST /post HTTP/1.1
   Host: postman-echo.com
   Content-Type: application/x-www-form-urlencoded
   Content-Length: 13
  
   say=Hi&to=Mom
    
   */


/////////////// BODY ///////////////////
  sensors.requestTemperatures(); 
  float tempC = DHT.readTemperature();
  float humiditys = DHT.readHumidity();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  int sensorValue = analogRead(A0);
      // Temp
String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
String intensidad =String(digitalRead(5));
String humedadSuelo =String(map(analogRead(A0),0,1023,100,0));
String humedadAmbiente=String(humiditys);
String tempSuelo =String(temperatureC);
String tempAmb=String(tempC);
      //   Temp

String userid = "\"id\":\"juanda903\"";
String validation_key = "\"validation_key\":\"uhiSKdB9FX\"";
String user = "\"user\":{"+userid+","+validation_key+"}";

String intencity = "\"intencity\":"+intensidad;
String sun = "\"sun\":{"+intencity+"},";

String groundH = "\"ground\":"+humedadSuelo;
String environmentH = "\"environment\":"+humedadAmbiente;
String humidity = "\"humidity\":{"+groundH+","+environmentH+"},";

String groundT = "\"ground\":"+tempSuelo;
String environmentT= "\"environment\":"+tempAmb;
String temperature = "\"temperature\":{"+groundT+","+environmentT+"}";

String measurements = "\"measurements\":{"+sun+humidity+temperature+"}";

String json = "{"+user+","+measurements+"}";
   
////////////////////////////////////////
 
  httpsClient.print(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "data: " + json + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded"+ "\r\n" +
               "Content-Length: 13" + "\r\n\r\n" +
               "say=Hi&to=Mom" + "\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  Serial.println(json);
  Serial.println("reply was:");
  Serial.println("==========");
  String line;
  while(httpsClient.available()){        
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("closing connection");
    
  delay(600000);  //POST Data at every 2 seconds
}
