/*
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "web.h"
#include <ArduinoJson.h>

const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
const int led_pin = 15;


AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int led_state = 0;

String odesliHodnoty(){
  DynamicJsonDocument doc(200); // fixed size

    JsonObject root = doc.to<JsonObject>();
    root["konstantaP"] = String(Kp);
    root["konstantaI"] = String(Ki);
    root["konstantaD"] = String(Kd);

    char buffer[200]; // create temp buffer
    size_t len = serializeJson(root, buffer);  // serialize to buffer

    webSocket.sendTXT(buffer, len); // send buffer to web socket
}


void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch(type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;

    // Handle text messages from client
    case WStype_TEXT:

      // Print out raw message
      Serial.printf("[%u] Received text: %s\n", client_num, payload);

      // Toggle LED
      /*
      if ( strcmp((char *)payload, "toggleLED") == 0 ) {
        led_state = led_state ? 0 : 1;
        Serial.printf("Toggling LED to %u\n", led_state);
        digitalWrite(led_pin, led_state);}
/////////////////////////////////////////////////////
      // Report the state of the LED
        if ( strcmp((char *)payload, "update") == 0 ) { //predelat update dat v jsonu
        sprintf(msg_buf, "%d", led_state);
        Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);
        

      // Message not recognized
      } else {
        Serial.println("[%u] Message not recognized");
      }
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START: //zjistit co to dělá
    case WStype_FRAGMENT_BIN_START:  //...
    case WStype_FRAGMENT:            // ..
    case WStype_FRAGMENT_FIN:
    default:
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
////////////////////////////////////////////////////////

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
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  
}

void WSloop() {
  webSocket.loop();
}*/