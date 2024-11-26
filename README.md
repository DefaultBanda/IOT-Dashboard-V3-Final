# ESP32 IoT Dashboard

This is the FINAL version of this project which is a fully integrated IoT dashboard built with an ESP32 and a 2.8" ILI9341 TFT display. It shows real-time data, including weather updates, stock prices, and 3D printer progress. The system has reached it's final form as it does everything I need it to do

---
![IMG_4049](https://github.com/user-attachments/assets/746f28ed-8bc3-4b99-937a-605f3de25f05)


https://github.com/user-attachments/assets/34504fbd-8bdf-4e7c-8c74-3c2241fb5b3f



## Features

### **Real-Time Weather Updates**
- Utilizes OpenWeatherMap API to display:
  - Current temperature
  - Weather conditions (e.g., sunny, cloudy, or rainy)
  - Wind speed
  - Humidity levels
- Displays dynamically updated data with clear formatting and spacing for enhanced readability.

### **Stock Tracker**
- Fetches live stock prices for selected symbols (AAPL, TSLA, and GOOG) using the Alpha Vantage API.
- Stocks are updated at regular intervals to keep the data fresh and relevant.
- Displays pricing trends with a visually appealing design.

### **3D Printer Monitor**
- Monitors active print jobs using the OctoPrint API.
- Displays the print job name, percentage completion, and an animated progress bar.
- Offers a practical glance at your print job status without needing to access the OctoPrint web interface.

### **Optimized for Performance**
- Code refactored for better readability, modularity, and reduced memory footprint.
- Efficient API calls reduce latency and improve response times.
- Debugging utilities built into the firmware to assist with troubleshooting connectivity issues.

### **Seamless Page Transitions**
- Automatically cycles through pages.
- Designed for clarity with well-organized data presentation across pages.

---

## Components

- **ESP32 Development Board**: Acts as the brains of the dashboard, handling API calls, data processing, and display updates.
- **2.8" ILI9341 TFT Display**: Provides a vibrant and interactive interface for data visualization.
- **XPT2046 Touchscreen Controller**: Optional but was included with my screen (Was disabled as it caused some touch problems)

---

## Wiring Diagram

| **ILI9341 Pin** | **ESP32 Pin** |
|------------------|---------------|
| VCC              | 3.3V         |
| GND              | GND          |
| CS               | GPIO 17      |
| RESET            | GPIO 5       |
| D/C              | GPIO 16      |
| SD_MOSI          | GPIO 23      |
| SD_SCK           | GPIO 18      |
| LED              | GPIO 32      |
| SD_MISO          | GPIO 19      |
| T_CLK            | GPIO 18      |
| T_CS             | GPIO 21      |
| T_DI             | GPIO 23      |
| T_DO             | GPIO 19      |

---

## Enhancements and Optimizations

### **Improved WiFi Handling**
- Optimized the WiFi connection process to ensure faster and more reliable connectivity.
- Reconnection logic implemented to recover gracefully from network disruptions.

### **Efficient API Integration**
- Streamlined the process of fetching and parsing data from multiple APIs.
- Utilized non-blocking functions to prevent system freezes during data retrieval.

### **Dynamic Data Handling**
- Added fallback mechanisms to handle API failures gracefully without disrupting the display.
- Integrated error logging for easier debugging and future development.

---

## Setup Instructions

1. **Open the Project in Arduino IDE**
   - Place the code in the `src/main.ino` file.

2. **Install Required Libraries**
   - `Adafruit GFX Library`
   - `Adafruit ILI9341`
   - `ArduinoJson`
   - `NTPClient`

3. **Update Configuration**
   - Update the WiFi credentials and API keys in the code:

     ssid = "YOUR_WIFI_SSID";
     password = "YOUR_WIFI_PASSWORD";
     weatherApiKey = "YOUR_OPENWEATHERMAP_API_KEY";
     stockApiKey = "YOUR_ALPHA_VANTAGE_API_KEY";
  

4. **Wire the Components**
   - Use the wiring diagram above to connect the ESP32 and TFT display.

5. **Upload the Code**
   - Upload the code to your ESP32 using Arduino IDE.

6. **Power On**
   - Watch your dashboard come to life with real-time updates!

---

## Future Features

- **Touch Interaction**: Plan to enable touchscreen functionality for navigation and additional controls.
- **Smart Home Integration**: Potential to expand with smart home sensors and devices.
- **Customizable Layouts**: Allow users to define their preferred screen layout via an online interface.

---

## APIs Used

1. **OpenWeatherMap API**
   - Fetches weather details for your specified location.
2. **Alpha Vantage API**
   - Retrieves live stock prices for AAPL, TSLA, and GOOG.
3. **OctoPrint API**
   - Monitors the progress of your 3D print jobs.

---

## Lessons Learned

This project was an opportunity to:
- Work with multiple APIs and manage real-time data.
- Develop a modular codebase that balances functionality with readability.
