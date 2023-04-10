
//---------inclusion des librairies -----//
#include <Arduino.h>           //inclusion de la bibliotheque arduino
#include <Wire.h>              // Inclus la bibliothèque Wire.h, afin de pouvoir envoyer des instructions sur le port i2c
#include <Adafruit_Sensor.h>   //utilisation de  la bibliothèque BMP280
#include <Adafruit_BME280.h>   //lectures du module de capteur BME280
#include <Adafruit_BMP280.h>   //lectures du module de capteur BMp280
#include <Arduino_JSON.h>      // gestion des chaine json
#include <AsyncTCP.h>          //dependance de  ESPAsyncWebServer
#include <ESPAsyncWebServer.h> //construire notre serveur Web et mettre en place un serveur HTTP asynchrone
#include <SPIFFS.h>            //gestionnaire des systeme de fichier
#include <AsyncElegantOTA.h>   //gestion des mise a jour distant

// creation d'objet bmp
Adafruit_BMP280 bmp; // I2C
// Variables pour enregistrer les valeur  de capteur
String data;
// Variable pour stocker les valeur de lecture
float temp;
float hum;
float pres;
float alt;

// Variable Json pour conserver les lectures du capteur
JSONVar readings;

// parametre wifi
const char *ssid = "dibfanappart2";
const char *password = "autorisation2";
// const char *ssid = "UNIFI_IDO1";
// const char *password = "42Bidules!";
//  serveur asynchrone
// Créé unAsyncWebServerobjet sur le port 80 :
AsyncWebServer server(80);

//-----------------FONCTIONS POUR GÉRER LES LECTURES DES CAPTEURS-----------------//

// Init BME280
// La fonction initBME() initialise le capteur BME280 sur les broches I2C par défaut ESP32 :
void initBMP()
{
  if (!bmp.begin(0x76))
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    while (1)
      delay(10);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

// Les fonctions getSensorReadings() obtiennent la température, pression et l'altitude du capteur BMp280 et enregistrent les valeurs des variables temp, pres et alt.
void getSensorReadings()
{

  temp = bmp.readTemperature();
  pres = bmp.readPressure() / 100.0F;
  alt = bmp.readAltitude(1013.25);
}

// La fonction getJSONReadings() renvoie une chaîne JSON à partir des valeurs actuelles de température, de pression et altitude
String getJSONReadings()
{
  readings["temperature"] = String(temp);
  readings["pressure"] = String(pres);
  readings["altitude"] = String(alt);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Cette fonction initialise le système de fichiers ESP32 SPIFFS. Dans ce projet, nous sauvegardons les fichiers HTML, CSS et JavaScript pour créer les pages du serveur Web sur le système de fichiers
void initSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
  }
}

// ========================
// Initialisation programme
// ========================
void setup()
{

  // Initialisation du port série (pour l'envoi d'infos via le moniteur série de l'IDE Arduino)
  Serial.begin(115200);
  // Init BME280 senspr
  initBMP();

  // Init SPIFFS
  initSPIFFS();

  //************** connexion au wifi **************
  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\n");
  Serial.println("Connexion etablie!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  //************************  chargement des fichiers ******************
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.serveStatic("/", SPIFFS, "/");
  /*
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/style.css", "text/css"); });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/script.js", "text/javascript"); });
  */
  //******************** réponses aux requetes du client ************
  // Affichage de la TEMPÉRATURE
  server.on("/lireTemperature", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String temps = String(bmp.readTemperature());
    request->send(200, "text/plain", temps); });

  // Affichage de la PRESSION ATMOSPHÉRIQUE
  server.on("/LirePresure", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String press = String(bmp.readPressure() / 100.0F);
    request->send(200, "text/plain", press); });
  // Affichage de l'estimation d'ALTITUDE
  server.on("/lireAltitude", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String alts = String(bmp.readAltitude(1013.25));
    request->send(200, "text/plain", alts); });

  // requette pour obtenir les dernier valeur de lecture
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      getSensorReadings();
      String json = getJSONReadings();
      request->send(200, "application/data", json);
      json = String(); });

  // Start ElegantOTA
  AsyncElegantOTA.begin(&server);

  // Start server
  server.begin();
  Serial.println("Serveur actif!");
}

// ======================================
// Boucle principale (boucle perpétuelle)
// ======================================
void loop()
{
  AsyncElegantOTA.loop();

  // Affichage de la TEMPÉRATURE dans le moniteur serie
  Serial.print(F("Température = "));
  Serial.print(bmp.readTemperature());
  Serial.println(F(" °C"));

  // Affichage de la PRESSION ATMOSPHÉRIQUE dans le moniteur serie
  Serial.print(F("Pression atmosphérique = "));
  Serial.print(bmp.readPressure());
  Serial.println(F(" hPa"));

  // Affichage de l'estimation d'ALTITUDE dans le moniter serie
  Serial.print(F("Altitude estimée = "));
  Serial.print(bmp.readAltitude(1013.25));
  Serial.println(F(" m"));

  // ... et on répète ce cycle à l'infini !
  delay(2000); // Avec x secondes d'attente, avant chaque rebouclage
               // … et un saut de ligne pour faire joli ;)
}
