#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "Sonar.h"

/* MQTT client management */
WiFiClient espClient;
PubSubClient client(espClient);

/* Tasks variables */
TaskHandle_t checkConnectionTask;
TaskHandle_t rilevationTask;
enum
{
    WORKING,
    CHECK_WIFI,
    CHECK_MQTT
} checkState;
unsigned long freq = 30;

void setup_wifi()
{

    delay(10);

    Serial.println(String("Connecting to ") + SSID);

    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        WiFi.begin(SSID, PASSWORD);
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

/* MQTT subscribing callback */
void callback(char *topic, byte *payload, unsigned int length)
{
    JsonDocument doc;
    deserializeJson(doc, payload, length);
    if (doc.containsKey("frequency"))
    {
        freq = doc["frequency"];
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.println("Attempting MQTT connection...");

        // Create a random client ID
        String clientId = String("esp-client-") + String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            client.subscribe(TOPIC_RECEIVE);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

// Checks if the connection is still alive, modifies leds accordingly
void checkConnectionTaskCode(void *parameter)
{
    digitalWrite(RED_LED_PIN, HIGH);
    setup_wifi();
    randomSeed(micros());
    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);

    for (;;)
    {
        switch (checkState)
        {
        case WORKING:
            if (WiFi.status() == WL_CONNECTED && client.connected())
            {
                client.loop();
                delay(10);
            }
            else
            {
                digitalWrite(GREEN_LED_PIN, LOW);
                digitalWrite(RED_LED_PIN, HIGH);
                checkState = CHECK_WIFI;
            }
            break;
        case CHECK_WIFI:
            if (WiFi.status() != WL_CONNECTED)
            {
                setup_wifi();
            }
            checkState = CHECK_MQTT;
            break;
        case CHECK_MQTT:
            if (!client.connected())
            {
                reconnect();
            }
            digitalWrite(GREEN_LED_PIN, HIGH);
            digitalWrite(RED_LED_PIN, LOW);
            checkState = WORKING;
            break;
        default:
            break;
        }
    }
}

// Sends data to the main Java application
void rilevationTaskCode(void *parameter)
{
    Sonar sonar(ECHO_PIN, TRIG_PIN, 10000);
    for (;;)
    {
        // digitalWrite(TRIG_PIN, LOW);
        // delayMicroseconds(2);
        // digitalWrite(TRIG_PIN, HIGH);
        // delayMicroseconds(10);
        // digitalWrite(TRIG_PIN, LOW);
        // long duration = pulseIn(ECHO_PIN, HIGH);
        // double distance = (duration / 2) / 29.1; // Calculate distance in cm
        float waterLevel = sonar.getDistance();

        if (waterLevel < 0)
        {
            waterLevel = 0;
        }

        Serial.print("Distanza: ");
        Serial.print(waterLevel);
        Serial.print(" cm ");
        Serial.println(millis());

        JsonDocument doc;
        doc["waterLevel"] = waterLevel;
        doc["time"] = millis();
        String jsonMessage;
        serializeJson(doc, jsonMessage);
        client.publish(TOPIC_SEND, jsonMessage.c_str());
        delay(60000 / freq);
    }
}

void setup()
{
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(115200);
    xTaskCreatePinnedToCore(checkConnectionTaskCode, "Task1", 10000, NULL, 1, &checkConnectionTask, 0);
    xTaskCreatePinnedToCore(rilevationTaskCode, "Task2", 10000, NULL, 1, &rilevationTask, 1);
}

void loop()
{
    delay(UINT_MAX);
}
