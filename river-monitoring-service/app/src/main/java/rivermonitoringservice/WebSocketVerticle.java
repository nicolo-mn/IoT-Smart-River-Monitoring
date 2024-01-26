package rivermonitoringservice;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.http.ServerWebSocket;
import io.vertx.core.json.JsonObject;

class WebSocketVerticle extends AbstractVerticle {

    private HttpServer server;
    private ServerWebSocket webSocket;
    private int i = 1;
    private int action = 0;

    @Override
    public void start() {
        startServer(vertx);
        vertx.eventBus().consumer("logic.to.websocket", message -> {
            String messageFromLogic = (String) message.body();
            if (webSocket != null) {
                // Use the existing WebSocket connection to send the message
                webSocket.writeTextMessage(messageFromLogic);
                log(messageFromLogic);
            } else {
                log("WebSocket connection not available.");
            }
        });
    }

    private void startServer(Vertx vertx) {
        server = vertx.createHttpServer();
        
        server.webSocketHandler((ctx) -> {    
            // ctx.writeTextMessage(new JsonObject().encode());
            this.webSocket = ctx; 
            ctx.textMessageHandler((msg) -> {
                vertx.eventBus().publish("websocket.to.logic", msg);
                // System.out.println("Server " + msg);
                // JsonObject item = new JsonObject();
                // if (action % 4 == 0) {
                //     String type = "RILEVATION";
                //     long time = i;
                //     double waterLevel = i;
                //     i++;
                //     // System.out.println(i);
                //     JsonObject rilevation = new JsonObject();
                //     rilevation.put("time", time);
                //     rilevation.put("waterLevel", waterLevel);
                //     item.put("type", type);
                //     item.put("data", rilevation);
                // } else if (action % 4 == 1) {
                //     String type = "MODE";
                //     item.put("type", type);
                //     String mode = "MANUAL";
                //     item.put("mode", mode);

                //     // disable manual
                //     mode = "AUTOMATIC";
                //     JsonObject item2 = new JsonObject();
                //     item2.put("type", type);
                //     item2.put("mode", mode);
                //     vertx.setTimer(15000, timerId -> {
                //         ctx.writeTextMessage(item2.encode());
                //     });
                // } else if (action % 4 == 2) {
                //     String type = "VALVE_OPENING";
                //     item.put("type", type);
                //     int valveOpening = 25;
                //     item.put("valveOpening", valveOpening);
                // } else if (action % 4 == 3) {
                //     String type = "STATE";
                //     item.put("type", type);
                //     int valveOpening = 75;
                //     item.put("valveOpening", valveOpening);
                //     String state = "ALARM_TOO_LOW";
                //     item.put("state", state);
                // }
                
                // action++;
                // vertx.setTimer(3000, timerId -> {
                //     ctx.writeTextMessage(item.encode());
                // });
            });
        }).listen(8080);
    }

    private void log(String msg) {
        System.out.println("[WEBSOCKET] " + msg);
    }
}
