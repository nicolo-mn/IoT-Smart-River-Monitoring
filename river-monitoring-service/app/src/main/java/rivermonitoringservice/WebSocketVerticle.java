package rivermonitoringservice;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.json.JsonObject;

class WebSocketVerticle extends AbstractVerticle {

    private HttpServer server;
    private int i = 1;
    private int action = 0;

    @Override
    public void start() {
        startServer(vertx);
    }

    private void startServer(Vertx vertx) {
        server = vertx.createHttpServer();
        
        server.webSocketHandler((ctx) -> {    
            ctx.writeTextMessage(new JsonObject().encode());    
            ctx.textMessageHandler((msg) -> {
                System.out.println("Server " + msg);
                JsonObject item = new JsonObject();
                if (action % 4 == 0) {
                    String type = "RILEVATION";
                    long time = i;
                    double waterLevel = i;
                    i++;
                    // System.out.println(i);
                    JsonObject rilevation = new JsonObject();
                    rilevation.put("time", time);
                    rilevation.put("waterLevel", waterLevel);
                    item.put("type", type);
                    item.put("data", rilevation);
                } else if (action % 4 == 1) {
                    String type = "MODE";
                    item.put("type", type);
                    String mode = "MANUAL";
                    item.put("mode", mode);

                    // disable manual
                    mode = "AUTOMATIC";
                    JsonObject item2 = new JsonObject();
                    item2.put("type", type);
                    item2.put("mode", mode);
                    vertx.setTimer(15000, timerId -> {
                        ctx.writeTextMessage(item2.encode());
                    });
                } else if (action % 4 == 2) {
                    String type = "VALVE_OPENING";
                    item.put("type", type);
                    int valveOpening = 25;
                    item.put("valveOpening", valveOpening);
                } else if (action % 4 == 3) {
                    String type = "STATE";
                    item.put("type", type);
                    int valveOpening = 75;
                    item.put("valveOpening", valveOpening);
                    String state = "ALARM_TOO_LOW";
                    item.put("state", state);
                }
                
                action++;
                vertx.setTimer(3000, timerId -> {
                    ctx.writeTextMessage(item.encode());
                });
            });
        }).listen(8080);

    }
}
