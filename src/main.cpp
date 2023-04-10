

//---------declaration pour notre libraire-----//
#include <Arduino.h>
#include <Wire.h> // Inclus la bibliothèque Wire.h, afin de pouvoir envoyer des instructions sur le port i2c
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>

// Create an Event Source on /events

Adafruit_BMP280 bmp; // I2C

// Variables to save values from HTML form
String data;

unsigned long previous_time = 0;
unsigned long Delay = 2000;
// Variable pour stocker les valeur de lecture
float temp;
float hum;
float pres;
float alt;

// Json Variable to Hold Sensor Readings
JSONVar readings;

// parametre wifi
const char *ssid = "dibfanappart2";
const char *password = "autorisation2";
// const char *ssid = "UNIFI_IDO1";
// const char *password = "42Bidules!";
//  serveur asynchrone

AsyncWebServer server(80);
// Instanciation de la librairie BME280

//-----------------FUNCTIONS TO HANDLE SENSOR READINGS-----------------//

// Init BME280
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

// Get Sensor Readings
void getSensorReadings()
{

  temp = bmp.readTemperature();
  pres = bmp.readPressure() / 100.0F;
  alt = bmp.readAltitude(1013.25);
}

// Return JSON String from sensor Readings
String getJSONReadings()
{
  readings["temperature"] = String(temp);
  readings["pressure"] = String(pres);
  readings["altitude"] = String(alt);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize SPIFFS
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

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });

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

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      getSensorReadings();
      String json = getJSONReadings();
      request->send(200, "application/data", json);
      json = String(); });

  server.begin();
  Serial.println("Serveur actif!");
}

// ======================================
// Boucle principale (boucle perpétuelle)
// ======================================
void loop()
{

  // Affichage de la TEMPÉRATURE
  Serial.print(F("Température = "));
  Serial.print(bmp.readTemperature());
  Serial.println(F(" °C"));

  // Affichage de la PRESSION ATMOSPHÉRIQUE
  Serial.print(F("Pression atmosphérique = "));
  Serial.print(bmp.readPressure());
  Serial.println(F(" hPa"));

  // Affichage de l'estimation d'ALTITUDE
  Serial.print(F("Altitude estimée = "));
  Serial.print(bmp.readAltitude(1013.25));
  Serial.println(F(" m"));

  // ... et on répète ce cycle à l'infini !
  delay(2000); // Avec x secondes d'attente, avant chaque rebouclage
               // … et un saut de ligne pour faire joli ;)
}
