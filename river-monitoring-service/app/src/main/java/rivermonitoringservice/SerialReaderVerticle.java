package rivermonitoringservice;
import io.vertx.core.AbstractVerticle;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

public class SerialReaderVerticle extends AbstractVerticle {
    
    private String receivedData = "";

    @Override
    public void start() {
        // Configura la tua porta seriale e registra un gestore di eventi
        String serialPortName = "COM5"; // Sostituisci con il nome della tua porta seriale
        int baudRate = 9600;

        SerialPort serialPort = new SerialPort(serialPortName);

        try {
            serialPort.openPort();
            serialPort.setParams(baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

            // Registra un SerialPortEventListener per gestire gli eventi sulla porta seriale
            serialPort.addEventListener(new SerialPortEventListener() {
                @Override
                public void serialEvent(SerialPortEvent event) {
                    if (event.isRXCHAR() && event.getEventValue() > 0) {
                        try {
                            receivedData = receivedData.concat(serialPort.readString(event.getEventValue()));
                            if (receivedData.contains("\n")) {
                                handleSerialData(receivedData.trim());
                                receivedData = "";
                            }
                        } catch (SerialPortException ex) {
                            ex.printStackTrace();
                        }
                    }
                }
            }, SerialPort.MASK_RXCHAR);

        } catch (SerialPortException ex) {
            ex.printStackTrace();
        }
        // String serialData = "CIAO";
        // vertx.eventBus().send("serial.to.logic", serialData);
        // log(serialData);
        vertx.eventBus().consumer("logic.to.serial", message -> {
            String serialData = (String) message.body();
            log(serialData);
            try {
                serialPort.writeString(serialData + "\n");
            } catch (SerialPortException ex) {
                ex.printStackTrace();
            }
        });

    }

    private void handleSerialData(String data) {
        // Implementa la logica per gestire i dati letti dalla seriale
        System.out.println("Dati dalla seriale: " + data);
    }

    private void log(String msg) {
        System.out.println("[SERIAL] " + msg);
    }
}

