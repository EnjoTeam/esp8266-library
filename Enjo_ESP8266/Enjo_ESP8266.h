#ifndef Enjo_ESP8266
#define Enjo_ESP8266
 
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>
 
class Enjo
{
public:
	Enjo();
	String ipToString(IPAddress ip);
	String getApiKeyControl(String nameHub, String type, String password, String token);
	void postLogDataToHTTPServer(String deviceID, String deviceName, String api);
	String getControlDataToHTTPServer(String deviceID, String field, String api);
	String addDeviceToHub(String formData, String api);
	void updateFieldsForDevice(String formData, String deviceID, String api);
	void wifiSetup(const char *SSID, const char *password);
};
 
#endif