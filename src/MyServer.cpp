/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
using namespace std;

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
    }

String processor(const String& var){
    float currentTemperature = 2.2f;
    if (var == "DATA_TEMP"){
    return String(currentTemperature);
    }
    return String();
}

String processor2(const String& var){
    int currentCompteurFour;
    if (var == "DATA_COMPTEUR"){
    return String(currentCompteurFour);
    }
    return String();
}

void MyServer::initAllRoutes() { 
    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
        }

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
        });

    this->on("/sac.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/sac.png", "image/png");
        });

    //Route du script JavaScript
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
        });
   
    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
        });

    // // Récupère la température cible et le temps de séchage
    // this->on("/setAdressIP", HTTP_GET, [](AsyncWebServerRequest *request){
    //     AsyncResponseStream *response = request->beginResponseStream("text/html");
    //     AsyncWebParameter* p1 = request->getParam(0);
    //     char buffer[1024];

    //     sprintf(buffer, "%s %s", "adresseIP", p1->value().c_str());

    //     if (ptrToCallBackFunction) (*ptrToCallBackFunction) (buffer);
        
    // });

    // Permet de savoir si on appuyer sur le bouton de démarrage
    this->on("/getAdresseIP", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        AsyncWebParameter* p1 = request->getParam(0);
        char buffer[1024];

        sprintf(buffer, "%s %s", "changement getAdresseIP", p1->value().c_str());

        if (ptrToCallBackFunction) (*ptrToCallBackFunction) (buffer);
        
    });

    this->begin();

    
};