package rivermonitoringservice;

// import java.util.Timer;
// import java.util.TimerTask;
// import jssc.SerialPort;
// import jssc.SerialPortEvent;
// import jssc.SerialPortEventListener;
// import jssc.SerialPortException;

// public class App {
//     private int count = 1;
//     private SerialMonitor sm;
//     private String receivedData = "";

//     public void connectToArduino() {
//         try {
//             sm = new SerialMonitor();
//             sm.start("COM5");
            
//             // Schedule the task to send a message every second
//             Timer timer = new Timer(true);
//             timer.scheduleAtFixedRate(new TimerTask() {
//                 @Override
//                 public void run() {
//                     sendMessageToArduino();
//                 }
//             }, 0, 1000);
//         } catch (Exception e) {
//             e.printStackTrace();
//         }
//     }

//     public void disconnectToArduino() {
//         try {
//             sm.close();
//         } catch (Exception ex) {
//             ex.printStackTrace();
//         }
//     }

//     private void sendMessageToArduino() {
//         try {
//             // Replace "YourMessage" with the actual message you want to send
//             sm.serialPort.writeString("ARDUINO_SET_VALVE_" + count + "\n");
//             count++;
//         } catch (SerialPortException ex) {
//             System.out.println("Error in sending string to COM-port: " + ex);
//         }
//     }

//     public class SerialMonitor implements SerialPortEventListener {
//         private SerialPort serialPort;

//         public void start(String portName) {
//             serialPort = new SerialPort(portName);
//             try {
//                 serialPort.openPort();

//                 serialPort.setParams(SerialPort.BAUDRATE_9600,
//                         SerialPort.DATABITS_8,
//                         SerialPort.STOPBITS_1,
//                         SerialPort.PARITY_NONE);

//                 serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN |
//                         SerialPort.FLOWCONTROL_RTSCTS_OUT);

//                 serialPort.addEventListener(this, SerialPort.MASK_RXCHAR);
//             } catch (SerialPortException ex) {
//                 System.out.println("There is an error on writing string to port: " + ex);
//             }
//         }

//         public synchronized void close() {
//             try {
//                 if (serialPort != null) {
//                     serialPort.removeEventListener();
//                     serialPort.closePort();
//                 }
//             } catch (Exception ex) {
//                 ex.printStackTrace();
//             }
//         }

//         private synchronized void processData(String data) {
//             System.out.println(data);
//         }

//         /**
//          * Handle an event on the serial port. Read the data and print it.
//          */
//         @Override
//         public synchronized void serialEvent(SerialPortEvent event) {
//             if (event.isRXCHAR() && event.getEventValue() > 0) {
//                 try {
//                     receivedData = receivedData.concat(serialPort.readString(event.getEventValue()));
//                     if (receivedData.contains("\n")) {
//                         processData(receivedData.trim());
//                         receivedData = "";
//                     }
//                 } catch (SerialPortException ex) {
//                     System.out.println("Error in receiving string from COM-port: " + ex);
//                 }
//             }
//         }
//     }

//     public static void main(String[] args) {
//         var app = new App();
//         app.connectToArduino();
//     }
// }



import io.vertx.core.Vertx;

public class App {
    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        vertx.deployVerticle(new WebSocksServerVerticle());
    }
    
    
}
