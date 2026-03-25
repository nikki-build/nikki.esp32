# nikki.esp32

**nikki.esp32** is an Arduino library for **ESP32** that connects to the [nikki.build Playground](https://nikki.build) — a fun platform for ** rapid prototyping and applied learning ** using real IoT devices.

This library makes it easy for students, makers, and educators to connect their ESP32 boards to the nikki Playground over **WebSocket (WSS)** and exchange data in real time.

---
## Overview

The nikki.esp32 library provides:

- Connect ESP32 to **nikki Playground** securely via `wss://`
- Send and receive real-time data using JSON
- Auto-reconnect and status event callbacks
- Great for **IoT learning**, **STEM education**, and **rapid prototypes**
- Lightweight integration with standard Arduino projects
-----
## 🚀 Features

- 🔐 Secure WSS communication
- 🔄 Automatic reconnect handling
- 📡 Real-time JSON messaging
- 📊 Event-based status callbacks
- ⚡ Designed for ESP32 performance
----

## Requirements

- ESP32 development board
- Arduino IDE 1.8.x or later (or Arduino CLI)
- WiFi network access
- Valid serviceDef.json and serviceToken.json from the nikki Playground dashboard
-----

## 📦 Installation

### Option 1 — From Arduino Library Manager  (Recommended)
1. Open Arduino IDE  
2. Go to **Tools → Manage Libraries**  
3. Search for **"nikki.esp32"**  
4. Click **Install**

### Option 2 — From GitHub  
1. Download this repo as ZIP  
2. In Arduino IDE, go to **Sketch → Include Library → Add .ZIP Library...**  
3. Select the downloaded ZIP file

---

## 🧩 Basic Example

```cpp

#include <WiFi.h>
#include <NikkiClientESP32.h>

// --- WiFi Credentials ---
const char *WIFI_SSID = "wifiSSD";
const char *WIFI_PASS = "wifiPassword";

// --- nikki Playground Credentials ---
const char *SERVICE_DEF_JSON = R"(

  // replace with your serviceDef.json content

)";

const char *SERVICE_TOKEN_JSON = R"(

  // replace with your serviceToken.json content

)";

// --- Custom Service Class ---
class MyNikkiService : public nikkiServiceBase
{
public:
  MyNikkiService(const char *serviceDefJson, const char *serviceTokenJson)
      : nikkiServiceBase(serviceDefJson, serviceTokenJson) {}

protected:
  void onStatus(const String &status, const String &details) override
  {
    Serial.println();
    Serial.print("[Status] ");
    Serial.print(status);
    Serial.print(" -> ");
    Serial.println(details);
  }

  void onMessage(const JsonVariant &message) override
  {
    Serial.println("[nikki Playground] Received:");
    serializeJsonPretty(message, Serial);
    Serial.println();
  }
};

// --- Create instance (no pointer now) ---
MyNikkiService nikki(SERVICE_DEF_JSON, SERVICE_TOKEN_JSON);

static unsigned long lastSent = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // Start nikki connection
  nikki.begin();
  delay(1000);
  Serial.println("Connecting to ws server...");
}

void loop()
{
  nikki.loop();

  // Send data every 2 seconds
  if (millis() - lastSent > 2000 && nikki.isConnected())
  {
    lastSent = millis();

    DynamicJsonDocument doc(256);
    doc["temp"] = 27.3;
    doc["humidity"] = 62;
    doc["unit"] = "C";
    doc["timeStamp"] = millis();

    nikki.send(doc);

    Serial.println("[Sent to nikki Playground]");
  }
}

```

### Connection Workflow

- Connect ESP32 to WiFi
- Initialize nikkiServiceBase with your service definition and token
- Register message and status callbacks
- Call begin() to establish the WSS connection
- Call loop() continuously inside loop()