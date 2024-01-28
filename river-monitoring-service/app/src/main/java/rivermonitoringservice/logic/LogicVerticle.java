package rivermonitoringservice.logic;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;

public class LogicVerticle extends AbstractVerticle {

    private static final String TYPE = "type";
    private static final String VALVE_OPENING = "valveOpening";

    private boolean isManual = false;
    private RiverState state = RiverState.NORMAL;

    public void start() {
        vertx.eventBus().consumer("websocket.to.logic", message -> {
            String setOpeningData = (String) message.body();
            vertx.eventBus().publish("logic.to.serial", setOpeningData);
        });

        vertx.eventBus().consumer("serial.to.logic", message -> {
            String serialData = (String) message.body();
            JsonObject receivedObj = new JsonObject(serialData);
            String type = receivedObj.getString(TYPE);
            // check for mode changes
            if (type.equals("CHANGE_MODE")) {
                String mode = receivedObj.getString("mode");
                isManual = mode.equals("MANUAL");
                vertx.eventBus().send("logic.to.websocket", serialData);
            }
            vertx.eventBus().send("logic.to.websocket", serialData);
        });

        vertx.eventBus().consumer("mqtt.to.logic", message -> {
            // send data to dashboard
            String mqttData = (String) message.body();
            JsonObject receivedObj = new JsonObject(mqttData);
            JsonObject rilevation = new JsonObject();
            rilevation.put(TYPE, "RILEVATION");
            rilevation.put("data", receivedObj);
            vertx.eventBus().send("logic.to.websocket", rilevation.encode());

            // check if river state needs to be changed
            double waterLevel = receivedObj.getDouble("waterLevel");
            if (!this.state.isWaterLevelValid(waterLevel)) {
                int prevFreq = this.state.getFrequency();
                this.state = RiverState.fromWaterLevel(waterLevel);
                // check if frequency needs to be changed
                if (prevFreq != this.state.getFrequency()) {
                    JsonObject espFreq = new JsonObject();
                    espFreq.put("frequency", this.state.getFrequency());
                    vertx.eventBus().send("logic.to.mqtt", espFreq.encode());
                }
                // notify dashboard of the changes
                JsonObject dashboardSetState = new JsonObject();
                dashboardSetState.put(TYPE, "CHANGE_STATE");
                dashboardSetState.put("state", this.state.getName());
                // notify Arduino of the changes
                if (!isManual) {
                    JsonObject arduinoSetValve = new JsonObject();
                    arduinoSetValve.put(TYPE, "SET_VALVE_AUTOMATIC");
                    arduinoSetValve.put(VALVE_OPENING, this.state.getValveOpening());
                    vertx.eventBus().send("logic.to.serial", arduinoSetValve.encode());
                    dashboardSetState.put(VALVE_OPENING, this.state.getValveOpening());
                }
                vertx.eventBus().send("logic.to.websocket", dashboardSetState.encode());
            }

        });
    }
}
