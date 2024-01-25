package rivermonitoringservice;





import io.vertx.core.Vertx;

public class App {
    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        // vertx.deployVerticle(new MQTTVerticle());
        // vertx.deployVerticle(new WebSocketVerticle());
        vertx.deployVerticle(new SerialReaderVerticle());
        // var conn = new SerialMonitor();
        // conn.connectToArduino();
    }
    
    
}
