#include "serverController.h"
#include "variables.h"
#include "env.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "leds/states/fullColorState.h"
#include "leds/states/rainbowState.h"
#include "leds/states/pulseState.h"
#include "leds/states/blackState.h"
#include "leds/states/starState.h"

AsyncWebServer server(80);

ServerController* ServerController::controller_ = nullptr;
ServerController::ServerController(){
  WiFi.mode(WIFI_STA);
  WiFi.config(IPAddress(192, 168, 1, 190), IPAddress(192, 168, 1, 100), IPAddress(255, 255, 255, 0));
  WiFi.begin(env::SSID, env::PASSWORD);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    ledController_->setState(new BlackState());
    request->send(200, "text/plain", "Turned off the leds");  
  });
  server.on("/fill", HTTP_GET, [](AsyncWebServerRequest *request){
    int h = 0, s = 255;
    if(!(request->params() < 2 || request->getParam(0) == NULL || request->getParam(1) == NULL)) {
      h = std::stoi(request->getParam(0)->value().c_str());
      s = std::stoi(request->getParam(1)->value().c_str());
    }
    ledController_->setState(new FullColorState(h, s));
    request->send(200, "text/plain", "Changed to color");  
  });
  server.on("/rainbow", HTTP_GET, [](AsyncWebServerRequest *request){
    int s = 255, f = 2;
    if(!(request->params() < 2 || request->getParam(0) == NULL || request->getParam(1) == NULL)) {
      s = std::stoi(request->getParam(0)->value().c_str());
      f = std::stoi(request->getParam(1)->value().c_str());
    }
    ledController_->setState(new RainbowState(s, f));
    request->send(200, "text/plain", "Changed to rainbow"); 
  });
  server.on("/pulse", HTTP_GET, [](AsyncWebServerRequest *request){
    int s = 255;
    if(!(request->params() < 1 || request->getParam(0) == NULL)) {
      s = std::stoi(request->getParam(0)->value().c_str());
    }
    ledController_->setState(new PulseState(s));
    request->send(200, "text/plain", "Changed to pulse"); 
  });
  server.on("/star", HTTP_GET, [](AsyncWebServerRequest *request){
    int p = 0, s = 255;
    if(!(request->params() < 2 || request->getParam(0) == NULL || request->getParam(1) == NULL)) {
      p = std::stoi(request->getParam(0)->value().c_str());
      s = std::stoi(request->getParam(1)->value().c_str());
    }
    ledController_->setState(new StarState(s, p));
    request->send(200, "text/plain", "Changed to star"); 
  });
  server.begin();
}

ServerController *ServerController::getInstance() {
  if(controller_ == nullptr) controller_ = new ServerController();
  return controller_;
};