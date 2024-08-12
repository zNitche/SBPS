const float VOLTAGE_REFERENCE = 1.06; // 1.1v reference, may slightly differ between units, for me this value works fine (1.06V)
const float VOLTAGE_DIVIDER_RATIO = 4.3; // 1v at ADC == 4.3v of Vs
const float ADC_CONVERSION_VALUE = (VOLTAGE_REFERENCE / 1023) * VOLTAGE_DIVIDER_RATIO;

const int READING_SAMPLES_COUNT = 20;

const int OVERCHARGING_CUTOFF_VOLTAGE = 4.2;
const int LOW_VOLTAGE_LOAD_CUTOFF_VOLTAGE = 3.1;

const int CHARGING_SWITCH = 0;
const int LOAD_SWITCH = 1;

void setup() {
  analogReference(INTERNAL);

  pinMode(CHARGING_SWITCH, OUTPUT);
  pinMode(LOAD_SWITCH, OUTPUT);
}

float getVoltage() {
  float avg = 0;

  for (int i = 0; i < READING_SAMPLES_COUNT; i++) {
    int adcValue = analogRead(A2);
    avg += adcValue * ADC_CONVERSION_VALUE;

    delay(100);
  }

  return avg / READING_SAMPLES_COUNT;
}


void manageLoadSwitch(float voltage) {
  if (voltage >= LOW_VOLTAGE_LOAD_CUTOFF_VOLTAGE) {
    digitalWrite(LOAD_SWITCH, HIGH);
  } else {
    digitalWrite(LOAD_SWITCH, LOW);
  }
}

void manageChargingSwitch(float voltage) {
  if (voltage >= OVERCHARGING_CUTOFF_VOLTAGE) {
    digitalWrite(CHARGING_SWITCH, LOW);
  } else {
    digitalWrite(CHARGING_SWITCH, HIGH);
  }
}

void loop() {
  digitalWrite(CHARGING_SWITCH, LOW);
  digitalWrite(LOAD_SWITCH, LOW);

  while (true) {
    float voltage = getVoltage();

    manageChargingSwitch(voltage);
    manageLoadSwitch(voltage);

    delay(500);
  }
}
