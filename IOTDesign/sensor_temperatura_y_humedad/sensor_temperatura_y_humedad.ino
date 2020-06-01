#include <DHT.h>
#define DHT11_PIN 2
DHT DHT(DHT11_PIN,DHT11);

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// GPIO where the DS18B20 is connected to
const int oneWireBus = 13;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
// configuracion pantalla

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void setup()
{
Serial.begin(115200);
DHT.begin();
sensors.begin();
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
}
void loop()
{
float tempC = DHT.readTemperature();
float tempF = DHT.convertCtoF(tempC);
float humidity = DHT.readHumidity();

Serial.print("Temperature (C) = ");
Serial.println(tempC);
Serial.print("Temperature (F) = ");
Serial.println(tempF);
Serial.print("Humidity = ");
Serial.println(humidity);
 sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  int sensorValue = analogRead(A0);
  
  Serial.print("exposicion a la luz:");
  Serial.println(sensorValue);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.print("valor de la foto resistencia:");
  display.println(sensorValue);
  display.display(); 
delay(3000);
}
