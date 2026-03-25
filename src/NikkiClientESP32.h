#ifndef NIKKI_CLIENT_ESP_H
#define NIKKI_CLIENT_ESP_H

#include <WiFi.h>                // ← ESP32 version
#include <Arduino.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <ctype.h>

#define OUT_DATA_SIZE_SEGMENT_MAX_LIMIT 2048
#define OUT_DATA_SIZE_MAX_LIMIT 4096
#define QUERY_STRING_KEY "token"

// --- Structs ---
struct NikkiServiceDetails
{
  String GuID;
  String servID;
  String instID;
  String name;
  String dispName;
  String servType;
};

struct NikkiTokenDetails
{
  String sessionID;
  String secrete;
  String wsAddr;
  float rateLimit;
};

// --- Main Class ---
class nikkiServiceBase
{
public:
  nikkiServiceBase(const char *serviceDefJson, const char *serviceTokenJson);

  void begin();
  void loop();
  bool isConnected();
  bool send(JsonVariant data);

protected:
  // ← User overrides these (no setOnXXX anymore)
  virtual void onStatus(const String &status, const String &details);
  virtual void onMessage(const JsonVariant &message);

private:
  NikkiServiceDetails _serviceDetails;
  NikkiTokenDetails _tokenDetails;
  unsigned long _rateLimitMs;
  unsigned long _lastSendMs;
  WebSocketsClient _webSocket;

  // Internal helper methods (all private)
  void _fireStatus(const String &status, const String &details);
  String _urlEncode(const String &str);
  bool _parseConfigs(const char *serviceDefJson, const char *serviceTokenJson);
  String getWssHost(const String &fullWssUrl);
  String getWssPathSegment(const String &fullWssUrl);
  String _buildWsUrl();
  void _webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
};

#endif // NIKKI_CLIENT_ESP_H