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