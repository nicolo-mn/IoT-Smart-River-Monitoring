#ifndef __VARIABLES_MANAGER_H__
#define __VARIABLES_MANAGER_H__

#include <WiFi.h>
#include <PubSubClient.h>

class VariablesManager {
public:
    VariablesManager();
    unsigned long* getFrequency();
    PubSubClient* getMQTTClient();

private:
    unsigned long frequency;
    PubSubClient* mqttClient;
};

#endif
