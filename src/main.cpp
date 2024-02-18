#include <Arduino.h>
#include "HomeSpan.h"
#include <IotWebConf.h>
#include <IotWebConfOptionalGroup.h>

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "presenceSensorESP32";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
const char wifiInitialApPassword[] = "busheyfields";

#define STRING_LEN 128
#define NUMBER_LEN 32

// -- Configuration specific key. The value should be modified if config structure was changed.
#define CONFIG_VERSION "opt1"

// -- When CONFIG_PIN is pulled to ground on startup, the Thing will use the initial
//      password to buld an AP. (E.g. in case of lost password)
#define CONFIG_PIN D2

// -- Status indicator pin.
//      First it will light up (kept LOW), on Wifi connection it will blink,
//      when connected to the Wifi it will turn off (kept HIGH).
#define STATUS_PIN D10

// -- Method declarations.
void handleRoot();
// -- Callback methods.
void configSaved();

DNSServer dnsServer;
WebServer server(80);

char timeServerIp[STRING_LEN];
char offsetParamValue[NUMBER_LEN];
char dstParamValue[STRING_LEN];

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword, CONFIG_VERSION);
iotwebconf::OptionalParameterGroup timeServerGroup = iotwebconf::OptionalParameterGroup("iotTimeServer", "Time Server", false);
iotwebconf::TextParameter timeServerIpParam = iotwebconf::TextParameter("IP address", "timeServerIp", timeServerIp, STRING_LEN);
iotwebconf::NumberParameter offsetParam = iotwebconf::NumberParameter("Offset", "timeOffset", offsetParamValue, NUMBER_LEN, "0", "-23..23", "min='-23' max='23' step='1'");
iotwebconf::CheckboxParameter dstParam = iotwebconf::CheckboxParameter("DST ON", "timeDst", dstParamValue, STRING_LEN,  false);

// -- An instance must be created from the class defined above.
iotwebconf::OptionalGroupHtmlFormatProvider optionalGroupHtmlFormatProvider;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  timeServerGroup.addItem(&timeServerIpParam);
  timeServerGroup.addItem(&offsetParam);
  timeServerGroup.addItem(&dstParam);

  iotWebConf.setStatusPin(STATUS_PIN);
  iotWebConf.setConfigPin(CONFIG_PIN);
  iotWebConf.setHtmlFormatProvider(&optionalGroupHtmlFormatProvider);
  iotWebConf.addParameterGroup(&timeServerGroup);
  iotWebConf.setConfigSavedCallback(&configSaved);

  // -- Initializing the configuration.
  iotWebConf.init();

  // -- Set up required URL handlers on the web server.
  server.on("/", handleRoot);
  server.on("/config", []{ iotWebConf.handleConfig(); });
  server.onNotFound([](){ iotWebConf.handleNotFound(); });

  // initialise homespan
  //homeSpan.begin("123 456 789", "presenceSensorESP32", "123 456 789");

  Serial.println("Ready.");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  iotWebConf.doLoop();
}

/**
 * Handle web requests to "/" path.
 */
void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>IotWebConf 13 Optional Group</title></head><div>Status page of ";
  s += iotWebConf.getThingName();
  s += ".</div>";

  if (timeServerGroup.isActive())
  {
    s += "<div>Time Server defined</div>";
    s += "<ul>";
    s += "<li>Time server IP: ";
    s += timeServerIp;
    s += "<li>Offset: ";
    s += atoi(offsetParamValue);
    s += "<li>DST: ";
    s += dstParam.isChecked() ? "ON" : "OFF";
    s += "</ul>";
  }
  else
  {
    s += "<div>Time Server not defined</div>";
  }
  s += "Go to <a href='config'>configure page</a> to change values.";
  s += "</body></html>\n";

  server.send(200, "text/html", s);
}

void configSaved()
{
  Serial.println("Configuration was updated.");
}