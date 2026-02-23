#include <CheapStepper.h>
#include <Servo.h>

// ---------- CONFIG ----------
Servo servo1;
#define IR_PIN 5
#define PROXI_PIN 6
#define BUZZER_PIN 12
const int POT_PIN = A0; // analog sensor (currently used like "soil" reading)

CheapStepper stepper(8, 9, 10, 11);

// timing / debounce
unsigned long lastProxiTrigger = 0;
unsigned long lastIrTrigger = 0;
const unsigned long debounceMs = 200; // simple debounce

// calibration / thresholds (adjust after testing)
const int ANALOG_MIN = 485;  // same as your constrain lower bound
const int ANALOG_MAX = 1023;
const int DRY_THRESHOLD_PERCENT = 20; // your condition fsoil > 20

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);
  pinMode(PROXI_PIN, INPUT_PULLUP);
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  servo1.attach(7);
  stepper.setRpm(17);

  // initial servo position
  servoMoveToDefault();
}

// ---------- MAIN LOOP ----------
void loop() {
  // Check proximity first (object approaching)
  if (isProximityTriggered()) {
    handleProximityEvent();
  }

  // Check IR sensor for object placed in position (classification trigger)
  if (isIrTriggered()) {
    handleIrEvent();
  }

  // other tasks could go here (non-blocking): e.g., checking bin fullness, sending logs, etc.
}

// ---------- SENSOR UTILS ----------
bool isProximityTriggered() {
  int L = digitalRead(PROXI_PIN); // INPUT_PULLUP: 0 means triggered
  if (L == 0) {
    if (millis() - lastProxiTrigger > debounceMs) {
      lastProxiTrigger = millis();
      return true;
    }
  }
  return false;
}

bool isIrTriggered() {
  int val = digitalRead(IR_PIN); // assume 0 when object present
  if (val == 0) {
    if (millis() - lastIrTrigger > debounceMs) {
      lastIrTrigger = millis();
      return true;
    }
  }
  return false;
}

// ---------- EVENT HANDLERS ----------
void handleProximityEvent() {
  Serial.println("Proximity detected -> welcome sequence");
  alertBuzzer(1000, 1000);
  // Rotate diverter to accept or show movement (your original: 240 degrees)
  stepper.moveDegreesCW(240);
  delay(1000);
  servoOpenCloseAction();
  stepper.moveDegreesCCW(240);
  delay(1000);
  Serial.println("Proximity sequence done");
}

void handleIrEvent() {
  Serial.println("IR detected -> start classification");
  alertBuzzer(1000, 500);

  int fsoilPercent = readAnalogPercentage(3); // read 3 samples and average
  Serial.print("AnalogPercent: ");
  Serial.print(fsoilPercent);
  Serial.println("%");

  // Decide based on measured percent (same logic you had)
  if (fsoilPercent > DRY_THRESHOLD_PERCENT) {
    // action when "dry" - in your code this triggered stepper 120 degrees
    Serial.println("Classified as category A (dry) -> actuate diverter short");
    stepper.moveDegreesCW(120);
    delay(1000);
    servoOpenCloseAction();
    stepper.moveDegreesCCW(120);
    delay(1000);
  } else {
    // else path: play buzzer and just servo action (you had that)
    Serial.println("Classified as category B (wet) -> alert + single servo action");
    alertBuzzer(1000, 500);
    servoOpenCloseAction();
  }
}

// ---------- ACTUATOR HELPERS ----------
void servoMoveToDefault() {
  servo1.write(180);
  delay(1000);
  servo1.write(70);
  delay(1000);
}

void servoOpenCloseAction() {
  // Single open/close cycle for a flap or lid
  servo1.write(180);
  delay(1000);
  servo1.write(70);
  delay(1000);
}

void alertBuzzer(unsigned int freq, unsigned long durationMs) {
  tone(BUZZER_PIN, freq, durationMs);
  // Note: tone() is non-blocking for the CPU, but you might still keep delays for UX.
}

// ---------- ANALOG UTILS ----------
int readAnalogPercentage(int samples) {
  long sum = 0;
  for (int i = 0; i < samples; ++i) {
    int raw = analogRead(POT_PIN);
    raw = constrain(raw, ANALOG_MIN, ANALOG_MAX);
    int pct = map(raw, ANALOG_MIN, ANALOG_MAX, 100, 0); // same mapping as original code
    sum += pct;
    delay(75);
  }
  int avg = (int)(sum / samples);
  return avg;
}
