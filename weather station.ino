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
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define NUMFLAKES     10 // Number of snowflakes in the animation example
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
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

WiFi.begin(ssid, password);
 
Serial.print("Connecting.");
while ( WiFi.status() != WL_CONNECTED )
{
  delay(500);
  Serial.print(".");
}
  Serial.println("connected");
  delay(1000);


  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

}
void outside(float temp,int hum,float pres)
{
  getWeather(&temp,&hum,&pres);
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println("  Weather outside");
  display.setCursor(0,8);             // Start at top-left corner
  display.println("Temperature:"+String(temp));
  display.setCursor(0,16);             // Start at top-left corner
  display.println("Humidity:"+String(hum)+"%");
  display.setCursor(0,24);             // Start at top-left corner
  display.println("Pressure:"+String(pres)+"kPs");
  display.display();
}
void inside()
{
  int chk = DHT11.read(DHT11PIN);
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println("  Weather inside");
  display.setCursor(0,8);             // Start at top-left corner
  display.println("Temperature:"+String(bmp.readTemperature()));
  //display.println("Temperature:"+String(DHT11.temperature));//DHT11.temperature
  display.setCursor(0,16);
   display.println("Humidity:"+String(DHT11.humidity)+"%");
  display.setCursor(0,24);             // Start at top-left corner
  display.println("Pressure:"+String(bmp.readPressure()/1000)+"kPs");
  display.display();
}
void getWeather(float *temp,int *humidity,float *pressure)
{
 if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
{
HTTPClient http; //Declare an object of class HTTPClient
 
// specify request destination
//http.begin("http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key); // !!
http.begin(wifiClient, "http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key);
 
int httpCode = http.GET(); // send the request
 
if (httpCode > 0) // check the returning code
{
String payload = http.getString(); //Get the request response payload
 
DynamicJsonBuffer jsonBuffer(512);
 
// Parse JSON object
JsonObject& root = jsonBuffer.parseObject(payload);
if (!root.success()) {
Serial.println(F("Parsing failed!"));
return;
}
 
*temp = (float)(root["main"]["temp"]) - 273.15; // get temperature in °C
*humidity = root["main"]["humidity"]; // get humidity in %
*pressure = (float)(root["main"]["pressure"]) / 1000; // get pressure in bar
float wind_speed = root["wind"]["speed"]; // get wind speed in m/s
int wind_degree = root["wind"]["deg"]; // get wind degree in °
 
// print data
Serial.printf("Temperature = %.2f°C\r\n", temp);
Serial.printf("Humidity = %d %%\r\n", humidity);
Serial.printf("Pressure = %.3f bar\r\n", pressure);
Serial.printf("Wind speed = %.1f m/s\r\n", wind_speed);
Serial.printf("Wind degree = %d°\r\n\r\n", wind_degree);


  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
}
 
http.end(); //Close connection

}
}
void loop() {
float temp = 0;
int hum = 0;
float pres = 0;
 if (digitalRead(13) == HIGH && presState == 0) { 
   slide++;
   presState = 1;}
 else if (digitalRead(13) == LOW) {presState = 0;} 
 if (slide > 2) { 
    slide = 1;
    }
  if (slide == 1)
    {
      inside();
    }
  if (slide == 2)
    {
      outside(temp,hum,pres);
    }

 // wait 1 minute
 
}
