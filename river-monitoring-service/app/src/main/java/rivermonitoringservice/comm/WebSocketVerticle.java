package rivermonitoringservice.comm;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.http.HttpServer;
import io.vertx.core.http.ServerWebSocket;

public class WebSocketVerticle extends AbstractVerticle {

    private HttpServer server;
    private ServerWebSocket webSocket;

    @Override
    public void start() {
        startServer(vertx);
        vertx.eventBus().consumer("logic.to.websocket", message -> {
            String messageFromLogic = (String) message.body();
            if (webSocket != null) {
                webSocket.writeTextMessage(messageFromLogic);
            } else {
                log("WebSocket connection not available.");
            }
        });
    }

    private void startServer(Vertx vertx) {
        server = vertx.createHttpServer();
        
        server.webSocketHandler((ctx) -> {    
            this.webSocket = ctx; 
            ctx.textMessageHandler((msg) -> {
                vertx.eventBus().publish("websocket.to.logic", msg);
            });
        }).listen(8080);
    }

    private void log(String msg) {
        System.out.println("[WEBSOCKET] " + msg);
    }
}
