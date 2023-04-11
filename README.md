pour avoir une idee du prjet voici les directive a suivre :
Veuillez faire envoie des données en utilisant un capteur i2c et un esp32 sur une page web via ajax et json. 
la page web doit afficher les données du capteur après chaque deux seconde, utiliser Bootstrap pour la mise en forme de la page web,
parallèlement a cela vous devez pourvoir faire des mises a jour a distance en utilisant OTA (over the air)

le projet est realisé en utilisant platformIO et vs-code 

montage du circuit est le suivant 
projetfinalBme280Webpage![CIRCUIT BMEBMP280](https://user-images.githubusercontent.com/73165862/231042475-1e3e5af0-298e-4781-bb51-4df5487390f1.png)

les bibliotheque a installer sont les suivantes 
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps = 
	adafruit/Adafruit BME280 Library@^2.2.2
	adafruit/Adafruit Unified Sensor@^1.1.9
	adafruit/Adafruit BMP280 Library@^2.6.6
	;me-no-dev/ESP Async WebServer @ ^1.2.3
	me-no-dev/AsyncTCP @ ^1.1.1
	arduino-libraries/Arduino_JSON @ ^0.2.0
	esphome/AsyncTCP-esphome@^2.0.0
	ayushsharma82/AsyncElegantOTA @ ^2.2.7
    ottowinter/ESPAsyncWebServer-esphome @ ^3.0.0
	;ayushsharma82/AsyncElegantOTA@^2.2.7

ici deux branche ont été créer 
firmweare1 

![Capture d’écran (4)](https://user-images.githubusercontent.com/73165862/231043844-5753c04c-9924-4155-8edd-2d3c70d58584.png)

 et firmeare2 avec mise a jour ota 
 
 ![Capture d’écran (10)](https://user-images.githubusercontent.com/73165862/231044000-5ce51568-3016-427c-913e-18336a8fa73f.png)
