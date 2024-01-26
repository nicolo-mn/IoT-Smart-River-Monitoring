package rivermonitoringservice;

/**
 * This class contains the values used by the application.
 */
public class Values {

    // water levels (in cm)
    private static final double WL1 = 2.0;
    private static final double WL2 = 5.0;
    private static final double WL3 = 7.0;
    private static final double WL4 = 10.0;
    // frequencies (in rilevations/minute)
    private static final int F1 = 6;
    private static final int F2 = 12;

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
