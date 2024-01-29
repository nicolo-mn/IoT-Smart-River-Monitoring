#ifndef __CONFIG_H__
#define __CONFIG_H__

#define GREEN_LED_PIN 6
#define RED_LED_PIN 7
#define ECHO_PIN 16
#define TRIG_PIN 17
#define EMPTY_DISTANCE 35

/* wifi network info */
#define SSID "AndroidAP_5664"
#define PASSWORD "ciaociao"

/* MQTT server address */
#define MQTT_SERVER "broker.mqtt-dashboard.com"

/* MQTT topic */
#define TOPIC_RECEIVE "smart-river-monitoring-to-esp-19191"
#define TOPIC_SEND "smart-river-monitoring-from-esp-19191"

#endif

