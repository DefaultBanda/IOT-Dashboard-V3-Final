#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS 17
#define TFT_DC 16
#define TFT_RST 5
#define TFT_LED 32
#define TOUCH_CS 21
#define TOUCH_IRQ 4

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

const char* ssid = "Hello_WIFI_SSID";
const char* password = "Hello_WIFI_PASSWORD";
const String weatherApiKey = "Hello_WEATHER_API_KEY";
const String stockApiKey = "Hello_STOCK_API_KEY";
const String city = "Hello_CITY_NAME";
const String countryCode = "Hello_COUNTRY_CODE";

String weatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&appid=" + weatherApiKey + "&units=metric";
String stockBaseUrl = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&apikey=" + stockApiKey + "&symbol=";

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -18000;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, 60000);

int currentPage = 0;
String currentWeather = "clear";
float currentTemp = 0;
float humidity = 50;
float windSpeed = 5;
unsigned long lastWeatherUpdate = 0;
const long updateInterval = 15 * 60000;

unsigned long lastPageSwitch = 0;
const unsigned long pageSwitchInterval = 5000;

String printJobName = "None";
float printProgress = 0;

float aaplPrice = 0.0;
float tslaPrice = 0.0;
float googPrice = 0.0;

void setup() {
    Serial.begin(115200);
    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, HIGH);
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
    ts.begin();
    ts.setRotation(1);
    connectWiFi();
    timeClient.begin();
    updateWeather();
    updateStockPrices();
    updateOctoPrintStatus();
}

void loop() {
    timeClient.update();
    if (millis() - lastWeatherUpdate > updateInterval) {
        updateWeather();
        updateStockPrices();
        updateOctoPrintStatus();
        lastWeatherUpdate = millis();
    }
    unsigned long currentMillis = millis();
    if (currentMillis - lastPageSwitch >= pageSwitchInterval) {
        lastPageSwitch = currentMillis;
        currentPage = (currentPage + 1) % 5;
        tft.fillScreen(ILI9341_BLACK);
    }
    displayPage();
}

void connectWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
}

void displayPage() {
    switch (currentPage) {
        case 0:
            displayMainInfo();
            break;
        case 1:
            displayHealthAndOutfit();
            break;
        case 2:
            displayHumidityAndWind();
            break;
        case 3:
            displayStockInfo();
            break;
        case 4:
            displayOctoPrintInfo();
            break;
    }
}

void displayMainInfo() {
    String timeStr = formatTime();
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.setTextSize(3);
    tft.setCursor(10, 10);
    tft.print("Time: ");
    tft.print(timeStr);
    String dateStr = formatDate();
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.print("Date: ");
    tft.print(dateStr);
    tft.setCursor(10, 90);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.print("Weather: ");
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.print(currentWeather);
    tft.setCursor(10, 120);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.print("Temp: ");
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.print(String(currentTemp) + " C");
}

void displayHealthAndOutfit() {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print("Health & Outfit");
    String outfitRec = currentTemp > 25 ? "T-shirt & shorts" : currentTemp > 15 ? "Light jacket" : "Coat & warm layer";
    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
    tft.print("Outfit: ");
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.print(outfitRec);
}

void displayHumidityAndWind() {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print("Humidity & Wind");
    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.print("Humidity: ");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.print(String(humidity) + "%");
    tft.setCursor(10, 90);
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.print("Wind Speed: ");
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.print(String(windSpeed) + " m/s");
}

void displayStockInfo() {
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print("Stock Tracker");
    tft.setCursor(10, 50);
    tft.print("AAPL: $");
    tft.print(String(aaplPrice));
    tft.setCursor(10, 90);
    tft.print("TSLA: $");
    tft.print(String(tslaPrice));
    tft.setCursor(10, 130);
    tft.print("GOOG: $");
    tft.print(String(googPrice));
}

void displayOctoPrintInfo() {
    tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print("3D Printer Status");
    tft.setCursor(10, 50);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.print("Job: ");
    tft.print(printJobName);
    tft.setCursor(10, 90);
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.print("Progress: ");
    tft.print(String(printProgress) + "%");
    int barWidth = 200;
    int barHeight = 20;
    int barX = 10;
    int barY = 120;
    tft.drawRect(barX, barY, barWidth, barHeight, ILI9341_WHITE);
    int filledWidth = (printProgress / 100.0) * barWidth;
    tft.fillRect(barX, barY, filledWidth, barHeight, ILI9341_GREEN);
}

void updateWeather() {
    HTTPClient http;
    http.begin(weatherUrl);
    int httpCode = http.GET();
    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        currentWeather = doc["weather"][0]["description"].as<String>();
        currentTemp = doc["main"]["temp"].as<float>();
        humidity = doc["main"]["humidity"].as<float>();
        windSpeed = doc["wind"]["speed"].as<float>();
    }
    http.end();
}

void updateStockPrices() {
    aaplPrice = fetchStockPrice("AAPL");
    tslaPrice = fetchStockPrice("TSLA");
    googPrice = fetchStockPrice("GOOG");
}

float fetchStockPrice(String symbol) {
    HTTPClient http;
    http.begin(stockBaseUrl + symbol);
    int httpCode = http.GET();
    float price = 0.0;
    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        price = doc["Global Quote"]["05. price"].as<float>();
    }
    http.end();
    return price;
}

void updateOctoPrintStatus() {
    HTTPClient http;
    http.begin("http://Hello_OCTOPRINT_IP:5000/api/job");
    http.addHeader("X-Api-Key", "Hello_OCTOPRINT_API_KEY");
    int httpCode = http.GET();
    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, payload);
        printJobName = doc["job"]["file"]["name"].as<String>();
        printProgress = doc["progress"]["completion"].as<float>();
    }
    http.end();
}

String formatTime() {
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    return (currentHour < 10 ? "0" : "") + String(currentHour) + ":" + (currentMinute < 10 ? "0" : "") + String(currentMinute);
}

String formatDate() {
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);
    int day = ptm->tm_mday;
    int month = ptm->tm_mon + 1;
    int year = ptm->tm_year + 1900;
    return String(day) + "/" + String(month) + "/" + String(year);
}
