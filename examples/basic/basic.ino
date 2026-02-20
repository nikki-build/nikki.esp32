#include <WiFi.h>
#include <NikkiClientESP.h>

// --- WiFi Credentials ---
const char *WIFI_SSID = "wifiSSD";
const char *WIFI_PASS = "wifiPassword";

// --- nikki Playground Credentials ---
// (replace with values from NikkiBuild Playground dashboard)
const char *SERVICE_DEF_JSON = R"(
  
   // replace this line with  your serviceDef.json content here

   
)";

const char *SERVICE_TOKEN_JSON = R"(
 
// replace this line with your serviceToken.json content here
 
 )";

// --- Create nikki client instance ---..
nikkiServiceBase *nikki;

void onMessageReceived(const JsonVariant &msg)
{
  Serial.println("[nikki Playground] Received:");
  serializeJsonPretty(msg, Serial);
  Serial.println();
}

void onStatusChanged(const String &status, const String &details)
{
  Serial.println("");
  Serial.print("[Status] ");
  Serial.print(status);
  Serial.print(" -> ");
  Serial.println(details);
}

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

  // Setup nikki client

  nikki = new nikkiServiceBase(SERVICE_DEF_JSON, SERVICE_TOKEN_JSON);

  nikki->setOnMessage(onMessageReceived);
  nikki->setOnStatus(onStatusChanged);

  // Start nikki playground connection
  nikki->begin();
  delay(1000);
  Serial.println("Connecting to ws server ");
}

static unsigned long lastSent = 0;

void loop()
{

  nikki->loop();

  // Example: Send temperature reading periodically
  if (millis() - lastSent > 2000 && nikki->isConnected())
  {
    lastSent = millis();

    // Send a JSON object
    DynamicJsonDocument doc(256);
    doc["temp"] = 27.3;
    doc["humidity"] = 62;
    doc["unit"] = "C";
    doc["timeStamp"] = millis();
    nikki->send(doc);
    Serial.println("[Sent to nikki Playground] ");
  }
}