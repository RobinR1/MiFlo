# Arduino code voor de MiFlo

Deze code is geschreven voor een [Adafruit Feather HUZZAH ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266) of [Adafruit Feather HUZZAH32](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather) in combinatie met een [gameduino 3 touchscreen](https://www.watterott.com/en/Gameduino-3). 

Bekijk in de [hardware](../hardware) folder van dit project de volledige hardware lijst.

## Configuratie en setup

Alle details over de Adafruit Feather [HUZZAH ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/overview) of [HUZZAH32 ESP32](https://learn.adafruit.com/adafruit-feather-huzzah32-esp32/overview) staan op [de site van adafruit](https://learn.adafruit.com/search?q=HUZZAH).

Kopiëer het bestand `settings_example.h` naar `settings.h` en vul alle parameters in.

Je hebt ook de volgende libraries nodig:

* [gd2-lib](https://github.com/jamesbowman/gd2-lib) Gameduino 2/3 library (v1.2.1)
* arduino EEPROM library
* [RTClib](https://github.com/adafruit/RTClib) RTC library.
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson) JSON library
* [PubSubClient](https://github.com/knolleary/pubsubclient) MQTT client library

Indien je PlatformIO gebruikt voeg je hiervoor volgende library dependencies toe aan je project:
```
lib_deps = 
	PubSubClient
	RTClib
	ArduinoJson
```
Van de gameduino library is er geen recente, werkende versie voorhanden in de PlatformIO library manager. Download deze library manueel en plaats deze als subdirectory in de `libs/` directory van je project.

In de gameduino 2/3 library moet je dan nog enkele aanpassingen doen zodat we de correcte pins van de HUZZAH of HUZZAH32 aanspreken:

* Voor de ESP8266 chip: Voer in het bestand `transports/wiring.h`
	```
	#define D8  0
	#define D9  16
	```
	toe net voor de lijn `#define CS D8`.
* Voor de ESP32 chip: 
  * Plaats in het bestand `GD2.h`:
  	```
	#elif defined(ESP32)
	#define SD_PIN       32
	```
	net achter de lijnen
	```
	#if defined(ESP8266)
	#define SD_PIN        D9    // pin used for the microSD enable signal
	```
	en plaats
	```
	#elif defined(ESP32)
	#define DEFAULT_CS 15
	```
	net achter de lijnen
	```
	#ifndef DEFAULT_CS
	#if defined(ESP8266)
	#define DEFAULT_CS D8
	```
	
* (dit lijkt niet nodig: Zoek in bestand `GD2.h` naar `#define L2` en `#define L4` en comment deze lijnen uit.)

Indien je de Arduino IDE gebruikt, moet je in de PubSubClient library ook nog een aanpassing uitvoeren om de maximum MQTT packet grootte in te stellen 1MB:
* Wijzig in het bestand `src/PubSubClient.h`:
	```
	#define MQTT_MAX_PACKET_SIZE 128
	```
	in
	```
	#define MQTT_MAX_PACKET_SIZE 1024
	```
Indien je echter PlatformIO gebruikt dien je in `platformio.ini` van je project volgende build option toe te voegen:
```
build_flags = -DMQTT_MAX_PACKET_SIZE=1024
```

De arduino code gaat ervanuit dat je de geconverteerde [assets](../assets) mee op de sd kaart gezet hebt.

In de code wordt bv de header-file `m_cool.h` geladen.

```
#include "m_cool.h"
```

Daarnaast worden ook alle pictogrammen geladen:

```
void load_jpgs() 
{
	GD.BitmapHandle(BRIL_JPG);
	GD.cmd_loadimage(ASSETS_END, 0);
	GD.load("BRIL.jpg");

	...
	
}
```
