#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "WebApp.h"
#include "Strip.h"
#include "Vesc.h"

using namespace std;

#define APSSID "VESC_LEDS (IP=4.4.4.4)"  // The SSID of the Wifi access point

IPAddress local_IP(4, 4, 4, 4);
IPAddress gateway(4, 4, 4, 4);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
Strip frontStrip(D1, 1);
Strip backStrip(D2, 2);
WebApp webApp;
Vesc vesc(40.5, 63);

/**
  Display the webapp  root
**/
void displayIndexPage() {
    server.send(200, "text/html",
                webApp.getHtml());  // TODO : Use LittleFS instead of a class : https://byfeel.info/esp8266-systeme-de-fichier-littlefs/
}

/**
  Starts the http server
**/
void startServer() {
    server.on("/", displayIndexPage);

    /******
     POWER
    ******/
    server.on("/power", HTTP_GET, []() {
        // TODO : multiple switches
        server.send(200, "text/plain", frontStrip.isPoweredOn() && backStrip.isPoweredOn() ? "1" : "0");
    });
    server.on("/power/on", HTTP_POST, []() {
        frontStrip.powerOn(true);
        backStrip.powerOn(true);
        Serial.println("Powered On");
        server.send(200);
    });
    server.on("/power/off", HTTP_POST, []() {
        frontStrip.powerOn(false);
        backStrip.powerOn(false);
        Serial.println("Powered Off");
        server.send(200);
    });

    /**********
     Mocks to be removed
    ***********/
    server.on("/rpm", HTTP_GET, []() {
        String rpm(vesc.rpm);
        server.send(200, "text/plain", rpm);
    });
    server.on("/rpm", HTTP_POST, []() {
        float rpm = server.arg("plain").toFloat();
        vesc.prevRpm = vesc.rpm;
        vesc.rpm = rpm;
        server.send(200);
    });
    server.on("/dutyCycle", HTTP_GET, []() {
        String dutyCycle(vesc.dutyCycle);
        server.send(200, "text/plain", dutyCycle);
    });
    server.on("/dutyCycle", HTTP_POST, []() {
        float dutyCycle = server.arg("plain").toFloat();
        vesc.dutyCycle = dutyCycle;
        server.send(200);
    });
    httpUpdater.setup(&server);
    server.begin();
}

/******
 SETUP
******/
void setup() {

    Serial.begin(9600);

    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(APSSID);

    IPAddress myIP = WiFi.softAPIP();

    Serial.print("AP IP address: ");
    Serial.println(myIP);

    startServer();

    Serial.println("HTTP server started");

    frontStrip.setColor(255, 255, 255);
    backStrip.setColor(255, 0, 0);
}

/**
  Main loop
**/
void loop() {
    vesc.refresh();
    if (vesc.isStopped()) {
        const int voltagePercent = vesc.getVoltagePercent();
        frontStrip.displayPercentage(voltagePercent);
        backStrip.displayPercentage(voltagePercent);
    } else if (vesc.isBreaking()) {
        frontStrip.fixedLights();
        backStrip.fixedLights();
    } else {
        frontStrip.fixedLights();
        backStrip.animateAccordion();
    }
    server.handleClient();
    delay(5);
}
