#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Adafruit_BMP085.h>
#include <dht11.h>

#define DHT11PIN 14
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 32  
#define OLED_RESET -1     
#define NUMFLAKES 10   
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiClient wifiClient;
dht11 DHT11;

const char *ssid = "setWifiName";
const char *password = "setWifiPass";
int slide = 0;
int presState = 0;

// set location and API key
String Location = "setCity, setCountry";
String API_Key = "setApiKey";
int netStatus = 1;


void setup() {
  Serial.begin(115200);
  pinMode(13, INPUT);
 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  
  }
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  WiFi.begin(ssid, password);

  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  delay(1000);
}
void outside(float temp, int hum, float pres) {

  getWeather(&temp, &hum, &pres);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("  Weather outside");
  display.setCursor(0, 8);
  display.println("Temperature:" + String(temp));
  display.setCursor(0, 16);
  display.println("Humidity:" + String(hum) + "%");
  display.setCursor(0, 24);
  display.println("Pressure:" + String(pres) + "kPs");
  display.display();
}
void inside() {

  int chk = DHT11.read(DHT11PIN);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("  Weather inside");
  display.setCursor(0, 8);
  display.println("Temperature:" + String(bmp.readTemperature()));
  display.setCursor(0, 16);
  display.println("Humidity:" + String(DHT11.humidity) + "%");
  display.setCursor(0, 24);
  display.println("Pressure:" + String(bmp.readPressure() / 1000) + "kPs");
  display.display();
}
void getWeather(float *temp, int *humidity, float *pressure) {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;  

    http.begin(wifiClient, "http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key);

    int httpCode = http.GET();  // send the request

    if (httpCode > 0)  // check the returning code
    {
      String payload = http.getString();  //Get the request response payload

      DynamicJsonBuffer jsonBuffer(512);

      // Parse JSON object
      JsonObject &root = jsonBuffer.parseObject(payload);
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }

      *temp = (float)(root["main"]["temp"]) - 273.15;        // get temperature in C
      *humidity = root["main"]["humidity"];                  // get humidity in %
      *pressure = (float)(root["main"]["pressure"]) / 1000;  // get pressure in bar
      float wind_speed = root["wind"]["speed"];              // get wind speed in m/s
      int wind_degree = root["wind"]["deg"];                 // get wind degree 

      // print data
      Serial.printf("Temperature = %.2f°C\r\n", temp);
      Serial.printf("Humidity = %d %%\r\n", humidity);
      Serial.printf("Pressure = %.3f bar\r\n", pressure);
      Serial.printf("Wind speed = %.1f m/s\r\n", wind_speed);
      Serial.printf("Wind degree = %d°\r\n\r\n", wind_degree);
    }

    http.end();  //Close connection
  }
}
void loop() {
  float temp = 0;
  int hum = 0;
  float pres = 0;
  
  if (digitalRead(13) == HIGH && presState == 0) {
    slide++;
    presState = 1;
  } else if (digitalRead(13) == LOW) {
    presState = 0;
  }
  if (slide > 2) {
    slide = 1;
  }
  if (slide == 1) {
    inside();
  }
  if (slide == 2) {
    outside(temp, hum, pres);
  }
}
