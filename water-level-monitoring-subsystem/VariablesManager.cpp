#include "VariablesManager.h"

VariablesManager::VariablesManager() : frequency(30) {
    // Inizializzazioni aggiuntive, se necessario
    WiFiClient espClient;
    this->mqttClient = new PubSubClient(espClient);
}

unsigned long* VariablesManager::getFrequency() {
    // Aggiungi meccanismi di sincronizzazione se necessario
    return &frequency;
}

PubSubClient* VariablesManager::getMQTTClient() {
    return mqttClient;
}