package rivermonitoringservice;

import io.vertx.core.Vertx;
import rivermonitoringservice.comm.MQTTVerticle;
import rivermonitoringservice.comm.SerialReaderVerticle;
import rivermonitoringservice.comm.WebSocketVerticle;
import rivermonitoringservice.logic.LogicVerticle;

/**
 * Program's entry point.
 */
public class App {
    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new MQTTVerticle());
        vertx.deployVerticle(new WebSocketVerticle());
        vertx.deployVerticle(new LogicVerticle());
        vertx.deployVerticle(new SerialReaderVerticle());
    }
}
