#include "web.h"
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
const int led_pin = 15;


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
char msg_buf[10];
int led_state = 0;

void odesliHodnoty(){
    const uint8_t size = JSON_OBJECT_SIZE(4);
    StaticJsonDocument<size> json; //nutno definovat velikost, ta se zmeni podle obsahu

    json["kP"] = String(Kp);  //nacteni hodnot do jsonu
    json["kI"] = String(Ki);
    json["kD"] = String(Kd);
    char data[200]; //snad by slo zmensit = vypsat velikost dat odeslanych pres json
    size_t len = serializeJson(json, data); //prevedeni dat na json (ulozen v data) a jeho velikost len

    ws.textAll(data, len); // odeslani dat vsem klientum
    Serial.print("[ESP]odesilam data klientum: ");
    Serial.println(data);
}

void zpracujZpravu(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
   

        const uint8_t size = JSON_OBJECT_SIZE(4);
        StaticJsonDocument<size> json;
        DeserializationError err = deserializeJson(json, data);
        if (err) {
            Serial.print(F("nelze dekodovat JSON"));
            Serial.println(err.c_str());
            return;
        }
        

        const char *akce = json["akce"];
        const char *konstantaP = json["kP"];
        const char *konstantaI = json["kI"];
        const char *konstantaD = json["kD"];  
        Serial.println("[ESP]obdrzeny hodnoty: ");
        Serial.print((char*)akce);       Serial.print("\t");
        Serial.print((char*)konstantaP); Serial.print("\t"); 
        Serial.print((char*)konstantaI); Serial.print("\t"); 
        Serial.print((char*)konstantaD); Serial.print("\t"); Serial.println(); 

        if (strcmp(akce, "update") == 0) {  //pokud je hodnota akce:hodnota dojde k odeslani aktualnich hodnot, pokud ale bude neco jineho (0), pak se nactou PID data 
            odesliHodnoty();
        }
        else{
        if(isAlphaNumeric(atof(konstantaP))){
            Kp = atof(konstantaP);
        }
        if(isAlphaNumeric(atof(konstantaI))){
            Ki = atof(konstantaI);
        }
        if(isAlphaNumeric(atof(konstantaD))){
            Kd = atof(konstantaD);
        }
        }
  }
}
void udalost(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("[SERVER]WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("[SERVER]WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      zpracujZpravu(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}
/*
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}*/

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}


void setupWS(){
  server.on("/", HTTP_GET, onIndexRequest);
  //server.on("/style.css", HTTP_GET, onCSSRequest);
  server.onNotFound(onPageNotFound);
  server.begin();
  ws.onEvent(udalost);
  server.addHandler(&ws);
  
}

void WSloop() {
  ws.cleanupClients();
}