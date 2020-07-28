#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>
#include <Enjo_ESP8266.h>
 
// 54:00:04:D1:E8:C8:DF:AA:65:E3:10:85:D2:3C:BC:7B:A1:24:20:11 
Enjo::Enjo(){
}

String Enjo::ipToString(IPAddress ip){
	String s = "";
	for (int i = 0; i < 4; i++)
		s += i  ? "." + String(ip[i]) : String(ip[i]);
	return s;
}

String Enjo::getApiKeyControl(String nameHub, String type, String password, String token){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
			uint8_t fingerprint[20] = {0x54, 0x00, 0x04, 0xD1, 0xE8, 0xC8, 0xDF, 0xAA, 0x65, 0xE3, 0x10, 0x85, 0xD2, 0x3C, 0xBC, 0x7B, 0xA1, 0x24, 0x20, 0x11};

		std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

		client->setFingerprint(fingerprint);

		HTTPClient https;

		Serial.println("\n---------- [Function]: getApiKeyControl Begin ----------\n");

		Serial.print("[HTTPS] begin...\n");
		if (https.begin(*client, "https://enjo-iot.xyz/add/control")) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header

			String formData = "MAC=" + WiFi.macAddress()+ "&"
								+ "name=" + nameHub + "&"
								+ "type=" + type + "&"
								+ "password=" + password + "&"
								+ "token=" + token;
			
			int httpCode = https.POST(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);

					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(2);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);

					const char *key = root["hubID"];					
										
					return String(key);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}

		Serial.println("\n---------- [Function]: getApiKeyControl End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

String Enjo::addDeviceToHub(String formData, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
			uint8_t fingerprint[20] = {0x54, 0x00, 0x04, 0xD1, 0xE8, 0xC8, 0xDF, 0xAA, 0x65, 0xE3, 0x10, 0x85, 0xD2, 0x3C, 0xBC, 0x7B, 0xA1, 0x24, 0x20, 0x11};

		std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

		client->setFingerprint(fingerprint);

		HTTPClient https;

		Serial.println("\n---------- [Function]: addDeviceToHub Begin ----------\n");

		Serial.print("[HTTPS] begin...\n");
		Serial.println("addDeviceToHub api: " + api);
		if (https.begin(*client, "https://www.enjo-iot.xyz/hub/add/device/" + api)) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header
			
			int httpCode = https.POST(formData);
			Serial.println(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);
					
					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(1);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);

					const char *id = root["deviceID"];
										
					return String(id);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: addDeviceToHub End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

void Enjo::postLogDataToHTTPServer(String deviceID, String deviceName, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
			uint8_t fingerprint[20] = {0x54, 0x00, 0x04, 0xD1, 0xE8, 0xC8, 0xDF, 0xAA, 0x65, 0xE3, 0x10, 0x85, 0xD2, 0x3C, 0xBC, 0x7B, 0xA1, 0x24, 0x20, 0x11};

		std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

		client->setFingerprint(fingerprint);

		HTTPClient https;

		Serial.println("\n---------- [Function]: postLogDataToHTTPServer Begin ----------\n");

		Serial.print("[HTTPS] begin...\n");
		if (https.begin(*client, "https://enjo-iot.xyz/update/log/" + api)) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header

			String formData = "deviceID=" + deviceID + "&"
			       + "name=" + deviceName + "&"
                   + "ipAddress=" + ipToString(WiFi.localIP()) + "&"
                   + "MAC=" + WiFi.macAddress() + "&"
                   + "ssidConnected=" + WiFi.SSID() + "&"
                   + "freeHeapSize=" + ESP.getFreeHeap() + "&"
                   + "flashChipSize=" + ESP.getFlashChipSize() + "&"
                   + "flashChipFrequency=" + ESP.getFlashChipSpeed();
			int httpCode = https.POST(formData);
			Serial.println(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: postLogDataToHTTPServer End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

void Enjo::updateFieldsForDevice(String formData, String deviceID, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
			uint8_t fingerprint[20] = {0x54, 0x00, 0x04, 0xD1, 0xE8, 0xC8, 0xDF, 0xAA, 0x65, 0xE3, 0x10, 0x85, 0xD2, 0x3C, 0xBC, 0x7B, 0xA1, 0x24, 0x20, 0x11};

		std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

		client->setFingerprint(fingerprint);

		HTTPClient https;

		Serial.println("\n---------- [Function]: updateFieldsForDevice Begin ----------\n");

		Serial.print("[HTTPS] begin...\n");
		Serial.println("addDeviceToHub api: " + api);
		if (https.begin(*client, "https://www.enjo-iot.xyz/update/control/" + api + "/" + deviceID)) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header
			
			int httpCode = https.POST(formData);
			Serial.println(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);
					
					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(1);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: updateFieldsForDevice End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

String Enjo::getControlDataToHTTPServer(String deviceID, String field, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
			uint8_t fingerprint[20] = {0x54, 0x00, 0x04, 0xD1, 0xE8, 0xC8, 0xDF, 0xAA, 0x65, 0xE3, 0x10, 0x85, 0xD2, 0x3C, 0xBC, 0x7B, 0xA1, 0x24, 0x20, 0x11};

		std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

		client->setFingerprint(fingerprint);

		HTTPClient https;

		Serial.println("\n---------- [Function]: getControlDataToHTTPServer Begin ----------\n");

		Serial.print("[HTTPS] begin...\n");
		
		if (https.begin(*client, "https://enjo-iot.xyz/get/control/" + api + "/" + deviceID + "/" + field)) {  // HTTPS
			https.addHeader("Content-Type", "application/x-www-form-urlencoded");
			Serial.print("[HTTPS] GET...\n");
			int httpCode = https.GET();
			 
			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);

					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(1);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);

					const char *state = root[field];
					Serial.println(field + ": " + String(state));
					return String(state);
			}
			} else {
				Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
			Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: getControlDataToHTTPServer End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

void Enjo::wifiSetup(const char *SSID, const char *password){
	Serial.println("\n---------- [Function]: wifiSetup Begin ----------\n");
	WiFi.begin(SSID, password);
	Serial.print("Connecting ");
	while (WiFi.status() != WL_CONNECTED) { // Wait for connection
		Serial.print(".");
		delay(300);
	}
	Serial.println("");
	Serial.print("WiFi connected to: ");
	Serial.println(WiFi.SSID());  // Print conneccted WiFi SSID
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.print("MAC address: ");
	Serial.println(WiFi.macAddress());
	Serial.println("\n---------- [Function]: wifiSetup Begin ----------\n");
}