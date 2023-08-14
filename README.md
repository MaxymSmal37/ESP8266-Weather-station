# Weather-station
nodemcu weather station
# Weather Station Code Readme

This repository contains Arduino code for a weather station using an ESP8266 microcontroller, an OLED display, a BMP085 sensor for pressure and temperature, and a DHT11 sensor for humidity and temperature. The weather station displays both indoor and outdoor weather information obtained from the OpenWeatherMap API.

## Components Used

- ESP8266 microcontroller
- SSD1306 OLED display
- BMP085 sensor
- DHT11 sensor
- WiFi connection for data retrieval

## Dependencies

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [ESP8266WiFi Library](https://github.com/esp8266/Arduino)
- [ESP8266HTTPClient Library](https://github.com/esp8266/Arduino)
- [ArduinoJson Library](https://github.com/bblanchon/ArduinoJson)
- [Adafruit BMP085 Library](https://github.com/adafruit/Adafruit_BMP085_Unified)
- [DHT11 Library](https://github.com/adafruit/DHT-sensor-library) (Assuming DHT11 sensor)

## Setup

1. Install the above libraries in your Arduino IDE if you haven't already.
2. Connect the components as per the wiring instructions in the code comments.
3. Replace placeholders like `setWifiName`, `setWifiPass`, `setCity`, `setCountry`, and `setApiKey` with your actual WiFi credentials, location information, and OpenWeatherMap API key.

## Functionality

The code provides the following functionality:

- Initializes the display and checks the BMP085 sensor.
- Connects to WiFi using provided credentials.
- Fetches weather data from the OpenWeatherMap API using the provided location and API key.
- Displays indoor and outdoor weather information alternately on the OLED display.
- The indoor data is read from the BMP085 sensor and DHT11 sensor.
- The outdoor data is obtained from the API response.

## Usage

1. Flash the code onto your ESP8266 device.
2. Observe the OLED display showing indoor and outdoor weather information alternately.
3. You can press a button connected to pin 13 to switch between indoor and outdoor data.

## Note

- The provided code assumes that you are using the DHT11 sensor for indoor temperature and humidity measurements. If you're using a different sensor, modify the code accordingly.
- Ensure that your ESP8266 is connected to the same WiFi network as your OpenWeatherMap API can be accessed.

