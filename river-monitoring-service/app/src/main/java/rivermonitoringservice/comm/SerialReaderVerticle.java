package rivermonitoringservice.comm;
import io.vertx.core.AbstractVerticle;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

public class SerialReaderVerticle extends AbstractVerticle {
    
    private String receivedData = "";
    private SerialPort serialPort;

    @Override
    public void start() {
        String serialPortName = "COM5";
        int baudRate = 9600;

        serialPort = new SerialPort(serialPortName);

        try {
            serialPort.openPort();
            serialPort.setParams(baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

            serialPort.addEventListener(new SerialPortEventListener() {
                @Override
                public void serialEvent(SerialPortEvent event) {
                    if (event.isRXCHAR() && event.getEventValue() > 0) {
                        try {
                            receivedData = receivedData.concat(serialPort.readString(event.getEventValue()));
                            if (receivedData.contains("\n")) {
                                vertx.eventBus().send("serial.to.logic", receivedData.trim());
                                receivedData = "";
                            }
                        } catch (SerialPortException ex) {
                            log("Error in receiving string from COM-port: " + ex);
                        }
                    }
                }
            }, SerialPort.MASK_RXCHAR);

        } catch (SerialPortException ex) {
            log("There is an error on writing string to port т: " + ex);
        }

        vertx.eventBus().consumer("logic.to.serial", message -> {
            String serialData = (String) message.body();
            try {
                serialPort.writeString(serialData + "\n");
            } catch (SerialPortException ex) {
                log("Error in writing string to COM-port: " + ex);
            }
        });

        // close serial port on shutdown
        Runtime.getRuntime().addShutdownHook(new Thread(() -> close()));
    }

    public synchronized void close() {
        try {
            if (serialPort != null) {
                serialPort.removeEventListener();
                serialPort.closePort();
            }
        } catch (Exception ex) {
            log("Error in closing to COM-port: " + ex);
        }
    }

    private void log(String msg) {
        System.out.println("[SERIAL] " + msg);
    }
}

