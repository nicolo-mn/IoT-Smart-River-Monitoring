package rivermonitoringservice.logic;

import java.util.function.Predicate;

/**
 * This enum contains the possible states of the river.
 */
public enum RiverState {

    ALARM_TOO_LOW("ALARM_TOO_LOW", Values.getF1(), 0, (waterLevel) -> waterLevel < Values.getWL1()),
    NORMAL("NORMAL", Values.getF1(), 25, (waterLevel) -> waterLevel >= Values.getWL1() && waterLevel <= Values.getWL2()),
    PRE_ALARM_TOO_HIGH("PRE_ALARM_TOO_HIGH", Values.getF2(), 25, (waterLevel) -> waterLevel > Values.getWL2() && waterLevel <= Values.getWL3()),
    ALARM_TOO_HIGH("ALARM_TOO_HIGH", Values.getF2(), 50, (waterLevel) -> waterLevel > Values.getWL3() && waterLevel <= Values.getWL4()),
    ALARM_TOO_HIGH_CRITIC("ALARM_TOO_HIGH_CRITIC", Values.getF2(), 100, (waterLevel) -> waterLevel > Values.getWL4());

    private final String name;
    private final int frequency;
    private final int valveOpening;
    private final Predicate<Double> isWaterLevelInMyRange;

    RiverState(String name, int frequency, int valveOpening, Predicate<Double> waterLevelPredicate) {
        this.name = name;
        this.frequency = frequency;
        this.valveOpening = valveOpening;
        this.isWaterLevelInMyRange = waterLevelPredicate;
    }

    public String getName() {
        return name;
    }

    public int getFrequency() {
        return frequency;
    }

    public int getValveOpening() {
        return valveOpening;
    }

    public boolean isWaterLevelValid(double waterLevel) {
        return isWaterLevelInMyRange.test(waterLevel);
    }

    public static RiverState fromWaterLevel(double waterLevel) {
        for (RiverState state : RiverState.values()) {
            if (state.isWaterLevelValid(waterLevel)) {
                return state;
            }
        }
        // default state
        return NORMAL;
    }
}
