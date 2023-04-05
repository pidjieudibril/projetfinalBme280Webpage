

//---------declaration pour notre libraire-----//
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>                         // Inclus la bibliothèque Wire.h, afin de pouvoir envoyer des instructions sur le port i2c
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
// Constantes du programme
#define adresseI2CduBME280                0x77            // Adresse I2C du BME280 (0x76, dans mon cas, ce qui est souvent la valeur par défaut)
#define pressionAuNiveauDeLaMerEnHpa      1024.90         // https://fr.wikipedia.org/wiki/Pression_atmospherique (1013.25 hPa en moyenne, valeur "par défaut")
#define delaiRafraichissementAffichage    1500            // Délai de rafraîchissement de l'affichage (en millisecondes)


// parametre wifi
//const char *ssid = "dibfanappart2";
//const char *password = "autorisation2";
const char *ssid = "UNIFI_IDO1";
const char *password = "42Bidules!";
// serveur asynchrone 

AsyncWebServer server(80);
// Instanciation de la librairie BME280
Adafruit_BME280 bme;


// ========================
// Initialisation programme
// ========================
void setup() {
  
  // Initialisation du port série (pour l'envoi d'infos via le moniteur série de l'IDE Arduino)
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Programme de test du BME280");
  Serial.println("===========================");
  Serial.println();

  // Initialisation du BME280
  Serial.print(F("Initialisation du BME280, à l'adresse [0x"));
  Serial.print(adresseI2CduBME280, HEX);
  Serial.println(F("]"));
  
  if(!bme.begin(adresseI2CduBME280)) {
    Serial.println(F("--> ÉCHEC…"));
  } else {
    Serial.println(F("--> RÉUSSIE !"));
  }
  Serial.println();

  //******************************* vérification des fichiers dans la mémoire **********
  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }
   //************** connexion au wifi **************
  WiFi.begin(ssid, password);
	Serial.print("Tentative de connexion...");
	
	while(WiFi.status() != WL_CONNECTED)
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
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });


//******************** réponses aux requetes du client ************
// Affichage de la TEMPÉRATURE
  server.on("/lireTemperature", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String temp = String(bme.readTemperature());
    request->send(200, "text/plain", temp);
  });
// Affichage du TAUX D'HUMIDITÉ
  server.on("/LireHumidity", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String hum = String(bme.readHumidity());
    request->send(200, "text/plain", hum);
  });
 // Affichage de la PRESSION ATMOSPHÉRIQUE
  server.on("/LirePresure", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String pres = String(bme.readPressure() / 100.0F);
    request->send(200, "text/plain", pres);
  });
  // Affichage de l'estimation d'ALTITUDE
  server.on("/lireAltitude", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    String alt = String(bme.readAltitude(pressionAuNiveauDeLaMerEnHpa));
    request->send(200, "text/plain", alt);
  });

 

  server.begin();
  Serial.println("Serveur actif!");
  
}


// ======================================
// Boucle principale (boucle perpétuelle)
// ======================================
void loop() {

  // Affichage de la TEMPÉRATURE
  Serial.print(F("Température = "));
  Serial.print(bme.readTemperature());
  Serial.println(F(" °C"));

  // Affichage du TAUX D'HUMIDITÉ
  Serial.print(F("Humidité = "));
  Serial.print(bme.readHumidity());
  Serial.println(F(" %"));
  
  // Affichage de la PRESSION ATMOSPHÉRIQUE
  Serial.print(F("Pression atmosphérique = "));
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(F(" hPa"));

  // Affichage de l'estimation d'ALTITUDE
  Serial.print(F("Altitude estimée = "));
  Serial.print(bme.readAltitude(pressionAuNiveauDeLaMerEnHpa));
  Serial.println(F(" m"));


  // ... et on répète ce cycle à l'infini !
  delay(delaiRafraichissementAffichage);                // Avec x secondes d'attente, avant chaque rebouclage
  Serial.println();                                     // … et un saut de ligne pour faire joli ;)

}