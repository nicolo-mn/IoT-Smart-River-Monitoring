package rivermonitoringservice;

import io.vertx.core.AbstractVerticle;

public class LogicVerticle extends AbstractVerticle {

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
    }

    private void log(String msg) {
        System.out.println("[LOGIC] " + msg);
    }
}
