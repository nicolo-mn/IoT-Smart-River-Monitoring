package rivermonitoringservice;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Verticle;
import io.vertx.mqtt.MqttClient;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.json.JsonObject;

public class MQTTVerticle extends AbstractVerticle {
    private static final String BROKER_ADDRESS = "broker.mqtt-dashboard.com";
    private static final String TOPIC_SEND = "smart-river-monitoring-to-esp-19191";
    private static final String TOPIC_RECEIVE = "smart-river-monitoring-from-esp-19191";
    private long frequency = 1000;

    @Override
    public void start() {
        MqttClient client = MqttClient.create(vertx);

        client.connect(1883, BROKER_ADDRESS, c -> {

            log("connected");

            log("subscribing...");
            client.publishHandler(s -> {
                // System.out.println("There are new message in topic: " + s.topicName());
                // System.out.println("Content(as string) of the message: " + s.payload().toString());
                // System.out.println("QoS: " + s.qosLevel());
                // // Parse the received JSON payload
                // JsonObject receivedJson = new JsonObject(s.payload());

                // // Retrieve the value of the 'distance' field
                // if (receivedJson.containsKey("distance")) {
                //     long distanceValue = receivedJson.getLong("distance");
                //     System.out.println("Received distance value: " + distanceValue);
                // } else {
                //     System.out.println("Received message does not contain 'distance' field.");
                // }


                // log("publishing a msg");
                // frequency += 1000;
                // JsonObject item = new JsonObject();
                // item.put("frequency", frequency);
                // client.publish(TOPIC_SEND,
                //         Buffer.buffer(item.encode()),
                //         MqttQoS.AT_LEAST_ONCE,
                //         false,
                //         false);
                vertx.eventBus().send("mqtt.to.logic", s.payload().toString());
            }).subscribe(TOPIC_RECEIVE, 2);

        });
    }

    private void log(String msg) {
        System.out.println("[MQTT AGENT] " + msg);
    }
}
