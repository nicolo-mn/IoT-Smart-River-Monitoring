package rivermonitoringservice.comm;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.mqtt.MqttClient;
import io.vertx.core.buffer.Buffer;

public class MQTTVerticle extends AbstractVerticle {
    private static final String BROKER_ADDRESS = "broker.mqtt-dashboard.com";
    private static final String TOPIC_SEND = "smart-river-monitoring-to-esp-19191";
    private static final String TOPIC_RECEIVE = "smart-river-monitoring-from-esp-19191";

    @Override
    public void start() {
        MqttClient client = MqttClient.create(vertx);

        client.connect(1883, BROKER_ADDRESS, c -> {

            log("connected");

            log("subscribing...");
            client.publishHandler(s -> {
                vertx.eventBus().send("mqtt.to.logic", s.payload().toString());
            }).subscribe(TOPIC_RECEIVE, 2);
            vertx.eventBus().consumer("logic.to.mqtt", message -> {
                String mqttData = (String) message.body();
                if (client != null && client.isConnected()) {
                    client.publish(TOPIC_SEND,
                            Buffer.buffer(mqttData),
                            MqttQoS.AT_LEAST_ONCE,
                            false,
                            false);
                }
            });
        });
    }

    private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }
}
