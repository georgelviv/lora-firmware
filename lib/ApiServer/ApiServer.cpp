#include "ApiServer.h"

String ssid;
const String password = "2233445566";

AsyncWebServer server(80);
AsyncWebSocket ApiServer::ws("/ws");

void ApiServer::setup() {
  this->logger = new Logger("API Server", true);

  this->deviceId = getDeviceId();
  this->setupWiFi();
  this->setupServer();
}

void ApiServer::setupWiFi() {
  WiFi.mode(WIFI_AP);
  ssid = "LoraTunning-" + this->deviceId;
  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  this->logger->log("IP:", ip.toString());
}

void ApiServer::setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response =
      request->beginResponse(200, "text/plain", "LoraTunning Hotspot OK");

    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "*");

    request->send(response);
  });

  server.on("/id", HTTP_GET, [this](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response =
      request->beginResponse(200, "text/plain", this->deviceId);

    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "*");

    request->send(response);
  });

  ws.onEvent([this](AsyncWebSocket *server,
    AsyncWebSocketClient *client,
    AwsEventType type,
    void *arg,
    uint8_t *data,
    size_t len
  ) {
    if (type == WS_EVT_CONNECT) {
      this->logger->log("WS client connected", (int)client->id());
    }

    if (type == WS_EVT_DISCONNECT) {
      this->logger->log("WS client disconnected", (int)client->id());
    }

    if (type == WS_EVT_DATA) {
      AwsFrameInfo *info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 &&
        info->len == len && info->opcode == WS_TEXT) {

        String msg;
        msg.reserve(len);
        for (size_t i = 0; i < len; i++) {
          msg += (char)data[i];
        }

        if (this->dataCallback) {
          this->dataCallback(msg);
        }
      }
    }
  });

  server.addHandler(&ws);

  server.begin();
}

void ApiServer::send(String msg) {
  ws.textAll(msg);
}

void ApiServer::setDataCallback(DataCallback cb) {
  this->dataCallback = cb;
}