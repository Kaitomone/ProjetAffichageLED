/* Copyright (C) 2021 Alain Dube
 * All rights reserved.
 *
 * Projet Allumage LED
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Alain Dubé
    @version  1.2 17/03/23 
    Historique des versions
           Version    Date       Auteur       Description
           1.0        23/02/03   Enzo       Première version du logiciel
           1.1        23/02/23   Enzo       Implémentation du serveur
           1.2        17/03/23   Enzo       Réalisation du publish et subscribe avec le serveur MQTT
           1.3        06/04/23   Enzo       Récupération de la couler et du texte
           

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps =        
            adafruit/Adafruit NeoPixel@^1.11.0
            moononournation/GFX Library for Arduino@^1.3.1
            adafruit/Adafruit GFX Library@^1.11.5
            adafruit/Adafruit NeoMatrix@^1.3.0
            bblanchon/ArduinoJson@^6.20.1
            adafruit/Adafruit SSD1306@^2.5.7
            ottowinter/ESPAsyncWebServer-esphome@^3.0.0

    Autres librairies (à copier dans le répertoire lib)
         WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
    
    Fonctions utiles (utilitaires)
              
*/
#include <Arduino.h>
// #include <ArduinoJson.h>
#include <MyFunctions.cpp>
#include <iostream>
#include <cstring>

using namespace std;

#include <WiFiManager.h>
#include <HTTPClient.h>
WiFiManager wm;
#define WEBSERVER_H

//Variable pour la connection Wifi
const char *SSID = "EcoleDuWeb2.4g";
const char *PASSWORD = "EcoleDuWEB";

#include <PubSubClient.h>

// Adresse IP du server MQTT
const char* mqtt_server = "172.16.5.101";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

String couleur = "";
int r = 0;
int g = 0;
int b = 0;
string texte;

bool etat;

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/Picopixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    22

#ifndef PSTR
#endif

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 256

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);


// On créer notre CallBack qui va recevoir les messages envoyés depuis la page WEB
void callback(char* topic, byte* message, unsigned int length) {
  
  couleur =  "";
  
  if (strcmp(topic, "enzo/led/couleur") == 0) {

    char receivedMessage[length + 1];
    memcpy(receivedMessage, message, length);
    receivedMessage[length] = '\0';
    // Print received message to serial monitor
    Serial.println(receivedMessage);

    couleur = receivedMessage;

    string actionToDo1 = getValue(couleur.c_str(), ' ', 0);
    string actionToDo2 = getValue(couleur.c_str(), ' ', 1);
    string actionToDo3 = getValue(couleur.c_str(), ' ', 2);
    string actionToDo4 = getValue(couleur.c_str(), ' ', 3);

    // Parse RGB color message
    r = stoi(actionToDo1);
    g = stoi(actionToDo2);
    b = stoi(actionToDo3);
    texte = actionToDo4;

    etat = 0;
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  
  void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
  
  void setCursor(int16_t x0, int16_t y0);
  void setTextColor(uint16_t color);
  void setTextColor(uint16_t color, uint16_t backgroundcolor);
  void setTextSize(uint8_t size);
  void setTextWrap(boolean w);
  
  matrix.begin();

  matrix.setBrightness(20);
  matrix.setTextWrap(false);
  
  matrix.setFont(&Picopixel);
  matrix.setCursor(0, 4);
  matrix.setTextSize(1);
  matrix.show();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32_client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("enzo/led/couleur");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    delay(100);

    matrix.clear();

    if(etat == 0) {

      // Fill the Neopixels with red
      for (int i = 0; i < LED_COUNT; i++) {
        matrix.setPixelColor(i, matrix.Color(0, 0, 255));
      }
    }
    
    if(etat == 1) {
      matrix.setCursor(0, 4);
      matrix.setTextSize(1);
      matrix.Color(r,g,b);
      matrix.println(texte.c_str());
    }

    // Update the Neopixels
    matrix.show();

    // Wait for a second
    delay(50);

    char leR[8];
    char leG[8];
    char leB[8];
    char laCouleur[24];
    char leMessage[8];
    char msgEnvoie[30];

    dtostrf(r, 1 ,0, leR);
    dtostrf(g, 1 ,0, leG);
    dtostrf(b, 1 ,0, leB);
    sprintf(laCouleur, "%s %s %s", leR, leG, leB);
    Serial.print("Couleur: ");
    Serial.println(laCouleur);
    sprintf(leMessage, "%s ", texte.c_str());


    Serial.println("Couleur: ");
    Serial.print("R=");
    Serial.print(leR);
    Serial.print(", G=");
    Serial.print(leG);
    Serial.print(", B=");
    Serial.print(leB);
    Serial.print(", message=");
    Serial.println(leMessage);

    sprintf(msgEnvoie, "%s %s %s %s", leR, leG, leB, texte.c_str());

    // client.publish("enzo/led/couleur/R", leR);
    // client.publish("enzo/led/couleur/G", leG);
    // client.publish("enzo/led/couleur/B", leB);
    // client.publish("enzo/led/couleur", laCouleur);

    client.publish("enzo/led/message", msgEnvoie);

  }
}