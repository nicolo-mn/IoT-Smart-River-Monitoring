package rivermonitoringservice;

/**
 * This class contains the values used by the application.
 */
public class Values {

    // water levels (in cm)
    private static final double WL1 = 5.0;
    private static final double WL2 = 10.0;
    private static final double WL3 = 15.0;
    private static final double WL4 = 20.0;
    // frequencies (in rilevations/minute)
    private static final int F1 = 30;
    private static final int F2 = 60;

    public static double getWL1() {
        return WL1;
    }

    public static double getWL2() {
        return WL2;
    }

    public static double getWL3() {
        return WL3;
    }

    public static double getWL4() {
        return WL4;
    }

    public static int getF1() {
        return F1;
    }

    public static int getF2() {
        return F2;
    }

    
}
