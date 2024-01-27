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
    private static final String FREQUENCY = "frequency";
    private static final String STATE = "STATE";

    private boolean isManual = false;
    private RiverState state = RiverState.NORMAL;

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
            // Send data to dashboard
            String mqttData = (String) message.body();
            JsonObject receivedObj = new JsonObject(mqttData);
            JsonObject rilevation = new JsonObject();
            rilevation.put(TYPE, RILEVATION_TYPE);
            rilevation.put(DATA, receivedObj);
            vertx.eventBus().send("logic.to.websocket", rilevation.encode());

            // check if river state needs to be changed
            double waterLevel = receivedObj.getDouble("waterLevel");
            if (!this.state.isWaterLevelValid(waterLevel)) {
                int prevFreq = this.state.getFrequency();
                this.state = RiverState.fromWaterLevel(waterLevel);
                log("New state: " + this.state.getName());
                // check if frequency needs to be changed
                if (prevFreq != this.state.getFrequency()) {
                    JsonObject espFreq = new JsonObject();
                    espFreq.put(FREQUENCY, this.state.getFrequency());
                    vertx.eventBus().send("logic.to.mqtt", espFreq.encode());
                }
                // notify Arduino of the changes
                if (!isManual) {
                    JsonObject arduinoSetValve = new JsonObject();
                    arduinoSetValve.put(TYPE, SET_VALVE_OPENING);
                    arduinoSetValve.put(VALVE_OPENING, this.state.getValveOpening());
                    vertx.eventBus().send("logic.to.serial", arduinoSetValve.encode());
                }
                // notify dashboard of the changes
                JsonObject dashboardSetState = new JsonObject();
                dashboardSetState.put(TYPE, STATE);
                dashboardSetState.put("state", this.state.getName());
                dashboardSetState.put(VALVE_OPENING, this.state.getValveOpening());
                vertx.eventBus().send("logic.to.websocket", dashboardSetState.encode());
            }

        });
    }

    private void log(String msg) {
        System.out.println("[LOGIC] " + msg);
    }
}
