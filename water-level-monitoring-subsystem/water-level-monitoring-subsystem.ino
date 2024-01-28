#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const int greenLedPin = 6;
const int redLedPin = 7;
const int echoPin = 16;
const int trigPin = 17;
const int emptyDistance = 35;

/* wifi network info */
const char *ssid = "AndroidAP_5664";
const char *password = "ciaociao";

/* MQTT server address */
const char *mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT topic */
const char *topic_receive = "smart-river-monitoring-to-esp-19191";
const char *topic_send = "smart-river-monitoring-from-esp-19191";

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

    Serial.println(String("Connecting to ") + ssid);

    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        WiFi.begin(ssid, password);
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
            client.subscribe(topic_receive);
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
    digitalWrite(redLedPin, HIGH);
    setup_wifi();
    randomSeed(micros());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);

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
                digitalWrite(greenLedPin, LOW);
                digitalWrite(redLedPin, HIGH);
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
            digitalWrite(greenLedPin, HIGH);
            digitalWrite(redLedPin, LOW);
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
    for (;;)
    {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        long duration = pulseIn(echoPin, HIGH);
        double distance = (duration / 2) / 29.1; // Calculate distance in cm
        double waterLevel = emptyDistance - distance;

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
        client.publish(topic_send, jsonMessage.c_str());
        delay(60000 / freq);
    }
}

void setup()
{
    pinMode(greenLedPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(115200);
    xTaskCreatePinnedToCore(checkConnectionTaskCode, "Task1", 10000, NULL, 1, &checkConnectionTask, 0);
    xTaskCreatePinnedToCore(rilevationTaskCode, "Task2", 10000, NULL, 1, &rilevationTask, 1);
}

void loop()
{
    delay(UINT_MAX);
}
