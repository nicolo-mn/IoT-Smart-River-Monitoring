package rivermonitoringservice;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.json.JsonObject;

class WebSocksServerVerticle extends AbstractVerticle {

    private HttpServer server;
    private int i = 1;

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
                String type = "rilevationData";
                long time = i;
                double waterLevel = i;
                i++;
                System.out.println(i);
                JsonObject rilevation = new JsonObject();
                rilevation.put("time", time);
                rilevation.put("waterLevel", waterLevel);
                item.put("type", type);
                item.put("data", rilevation);
                    vertx.setTimer(3000, timerId -> {
                        ctx.writeTextMessage(item.encode());
                    });
                });
        }).listen(8080);

    }
}
