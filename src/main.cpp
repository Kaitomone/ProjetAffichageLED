/* Copyright (C) 2021 Alain Dube
 * All rights reserved.
 *
 * Projet Allumage LED
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 23/.02/23 
    Historique des versions
           Version    Date       Auteur       Description
           1.0        23/02/03   Enzo       Première version du logiciel
           1.1        23/02/23   Enzo       Implémentation du serveur
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

    Classes du système
         
        MyServer                        V1.0    Pour la gestion des routes pour le site WEB
            /data                               Répertoire qui contient les fichiers du site WEB 
                index.html              V1.0    Page index du site WEB
                index.css               V1.0    CSS
                script.js               V1.0    JS (fonctions JavaScript)
              
 * */
#include <Arduino.h>
// #include <ArduinoJson.h>
#include <MyFunctions.cpp>
#include <iostream>
#include <cstring>

using namespace std;

// Gestion écran OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#include <MyOled.h>
// Gestion écran OLED
MyOled *myOled = NULL;

#include <MyOledView.h>
MyOledView *myOledView = NULL;

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

// Declare our NeoPixel strip object:
//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// On créer notre CallBack qui va recevoir les messages envoyés depuis la page WEB
void callback(char* topic, byte* message, unsigned int length) {

  couleur = "";
  for (int i = 0; i < length; i++)
  {
    couleur.concat(String((message[i] - '0')));
  }

  string actionToDo1 = getValue(couleur.c_str(), '-16', 0);
  string actionToDo2 = getValue(couleur.c_str(), '-16', 1);
  string actionToDo3 = getValue(couleur.c_str(), '-16', 2);

  // Print message to serial monitor
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print(", ");
  Serial.print(actionToDo1.c_str());
  Serial.print(", ");
  Serial.print(actionToDo2.c_str());
  Serial.print(", ");
  Serial.print(actionToDo3.c_str());
  Serial.print("] ");

  if (strcmp(topic, "enzo/led/couleur") == 0) {
    // Parse RGB color message
    int r = stoi(actionToDo1);
    int g = stoi(actionToDo2);
    int b = stoi(actionToDo3);

    // Print RGB values to serial monitor
    Serial.print("RGB color received: ");
    Serial.print("R=");
    Serial.print(r);
    Serial.print(", G=");
    Serial.print(g);
    Serial.print(", B=");
    Serial.println(b);
  }

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
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

  myOled = new MyOled(&Wire, -1, 64, 128);
  myOled->init();

  // put your setup code here, to run once:
  // void drawPixel(uint16_t x, uint16_t y, uint16_t color);
  // void drawFastVLine(uint16_t x0, uint16_t y0, uint16_t length, uint16_t color);
  // void drawFastHLine(uint8_t x0, uint8_t y0, uint8_t length, uint16_t color);
  
  // void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  // void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);

  // void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  // void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

  // void drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
  // void fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);

  // void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  // void fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  
  void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
  
  void setCursor(int16_t x0, int16_t y0);
  void setTextColor(uint16_t color);
  void setTextColor(uint16_t color, uint16_t backgroundcolor);
  void setTextSize(uint8_t size);
  void setTextWrap(boolean w);
  
  matrix.begin();

  matrix.setBrightness(5);
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
  
  // matrix.clear();

  // matrix.setCursor(0, 4);
  // matrix.setTextSize(1);
  // matrix.setTextColor(WHITE);
  // matrix.print("ENZO");
  // matrix.show();

  // matrix.print('\n');

  // matrix.setTextColor(RED);
  // matrix.print("SAMY");
  // matrix.show();

  // delay(500);

  // matrix.clear();
  // matrix.setCursor(0, 4);
  // matrix.drawCircle(8, 8, 7, RED);
  // matrix.show();

  // delay(500);

  // matrix.clear();
  // for (int i = 0; i < LED_COUNT; i++) {
  //   matrix.setPixelColor(i, matrix.Color(255, 255, 255));
  // }
  // matrix.show();
  
  // delay(500);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    char leR[8];
    char leG[8];
    char leB[8];

    float R = 100;
    float G = 200;
    float B = 255;

    dtostrf(R, 1 ,0, leR);
    dtostrf(G, 1 ,0, leG);
    dtostrf(B, 1 ,0, leB);

    Serial.print("Couleur: ");
    Serial.print("R=");
    Serial.print(leR);
    Serial.print(", G=");
    Serial.print(leG);
    Serial.print(", B=");
    Serial.println(leB);

    client.publish("enzo/led/couleur/R", leR);
    client.publish("enzo/led/couleur/G", leG);
    client.publish("enzo/led/couleur/B", leB);
  }
}