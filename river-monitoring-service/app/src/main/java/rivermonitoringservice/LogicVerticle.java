package rivermonitoringservice;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.Json;
import io.vertx.core.json.JsonObject;

public class LogicVerticle extends AbstractVerticle {

    private static final String TYPE = "type";
    private static final String MODE = "mode";
    private static final String MODE_TYPE = "MODE";
    private static final String RILEVATION_TYPE = "RILEVATION";
    private static final String DATA = "data";
    private static final String SET_VALVE_OPENING = "SET_VALVE_OPENING";
    private static final String VALVE_OPENING = "valveOpening";
    private static final String MANUAL_MSG = "MANUAL";
    private static final String AUTOMATIC_MSG = "AUTOMATIC";

    private boolean isManual = false;

    public void start() {
        // Subscribe to messages from serial port (assuming String messages)
        // vertx.eventBus().consumer("serial.to.logic", message -> {
        //     String serialData = (String) message.body();
        //     log(serialData);

        //     // Send a message to LogicVerticle (assuming String message)
        //     vertx.eventBus().send("logic.to.websocket", serialData);
        // });
        vertx.eventBus().consumer("websocket.to.logic", message -> {
            String setOpeningData = (String) message.body();
            vertx.eventBus().publish("logic.to.serial", setOpeningData);
        });

        vertx.eventBus().consumer("serial.to.logic", message -> {
            String serialData = (String) message.body();
            JsonObject receivedObj = new JsonObject(serialData);
            String type = receivedObj.getString(TYPE);
            if (type.equals(MODE_TYPE)) {
                String mode = receivedObj.getString(MODE);
                isManual = mode.equals(MANUAL_MSG);
                vertx.eventBus().send("logic.to.websocket", serialData);
            }
            vertx.eventBus().send("logic.to.websocket", serialData);
        });

        vertx.eventBus().consumer("mqtt.to.logic", message -> {
            String mqttData = (String) message.body();
            JsonObject receivedObj = new JsonObject(mqttData);
            JsonObject rilevation = new JsonObject();
            rilevation.put(TYPE, RILEVATION_TYPE);
            rilevation.put(DATA, receivedObj);
            vertx.eventBus().send("logic.to.websocket", rilevation.encode());
        });
    }

    private void log(String msg) {
        System.out.println("[LOGIC] " + msg);
    }
}
