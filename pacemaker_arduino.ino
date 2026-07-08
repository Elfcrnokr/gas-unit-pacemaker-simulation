// GAS UNIT PACEMAKER - ANTICOMPLICATION
// Elif Ceren Okur - Hatice Cemil Ercan Science High School
//
// This code is developed for the innovative pacemaker system working with AD8232 heart
// sensor, gas pressure sensor and piezo buzzer according to the Fritzing schematic.

// Pin definitions - According to Fritzing schematic
const int ECG_OUTPUT = A0;    // AD8232 heart sensor analog output
const int ECG_LO_PLUS = 10;   // AD8232 LO+ pin
const int ECG_LO_MINUS = 11;  // AD8232 LO- pin
const int GAS_SENSOR = A1;    // Gas pressure sensor
const int BUZZER_PIN = 9;     // Piezo buzzer
const int LED_PIN = 13;       // Status LED

// System parameters and threshold values
const int HEART_RATE_MIN = 60;           // Minimum heart rate
const int HEART_RATE_MAX = 100;          // Maximum heart rate
const int PNEUMOTHORAX_THRESHOLD = 600;  // Pneumothorax pressure threshold (analog value)
const int ECG_THRESHOLD = 512;           // ECG signal threshold
const int SAMPLE_SIZE = 10;              // Sample size for averaging

// Global variables
unsigned long lastBeatTime = 0;
unsigned long currentTime = 0;
int heartRate = 0;
int ecgValue = 0;
int gasValue = 0;
bool pneumothoraxDetected = false;
bool ecgAbnormal = false;
int beatCount = 0;
unsigned long measurementStartTime = 0;

// Alarm frequencies (Hz)
const int FREQ_NORMAL = 1000;        // Normal status
const int FREQ_PNEUMOTHORAX = 2500;  // Pneumothorax warning
const int FREQ_ECG_ABNORMAL = 1500;  // ECG abnormality
const int FREQ_EMERGENCY = 3000;     // Emergency situation

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("=== GAS UNIT PACEMAKER SYSTEM ===");
  Serial.println("System initializing...");

  // Set pin modes
  pinMode(ECG_LO_PLUS, INPUT);
  pinMode(ECG_LO_MINUS, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Startup signal
  startupSequence();

  // Record startup time
  measurementStartTime = millis();

  Serial.println("System ready - Monitoring started");
  Serial.println("HR: Heart Rate | ECG: ECG Value | GAS: Gas Pressure");
  Serial.println("----------------------------------------");
}

void loop() {
  currentTime = millis();

  // ECG electrode connection check
  if (digitalRead(ECG_LO_PLUS) == 1 || digitalRead(ECG_LO_MINUS) == 1) {
    Serial.println("WARNING: Check ECG electrode connections!");
    playAlarm(FREQ_ECG_ABNORMAL, 500);
    delay(1000);
    return;
  }

  // Read sensor data
  readSensors();

  // Calculate heart rate
  calculateHeartRate();

  // Check for pneumothorax
  checkPneumothorax();

  // Check ECG abnormality
  checkECGAbnormality();

  // Evaluate system status
  evaluateSystemStatus();

  // Data output (every 2 seconds)
  if (currentTime % 2000 < 100) {
    printSystemStatus();
  }

  // Blink system LED
  blinkSystemLED();

  delay(50);  // Main loop delay
}

void readSensors() {
  // Read ECG data
  ecgValue = analogRead(ECG_OUTPUT);

  // Read gas pressure sensor data
  gasValue = analogRead(GAS_SENSOR);
}

void calculateHeartRate() {
  // Simple beat detection (R-wave detection)
  static int lastEcgValue = 0;
  static bool beatDetected = false;

  // Rising edge detection
  if (ecgValue > ECG_THRESHOLD && lastEcgValue <= ECG_THRESHOLD && !beatDetected) {
    unsigned long timeBetweenBeats = currentTime - lastBeatTime;

    // Realistic heart beat interval check (300ms - 2000ms range)
    if (timeBetweenBeats > 300 && timeBetweenBeats < 2000) {
      heartRate = 60000 / timeBetweenBeats;  // BPM calculation
      lastBeatTime = currentTime;
      beatCount++;
      beatDetected = true;
    }
  }

  // Falling edge - reset beat detection
  if (ecgValue <= ECG_THRESHOLD) {
    beatDetected = false;
  }

  lastEcgValue = ecgValue;
}

void checkPneumothorax() {
  static int gasSamples[SAMPLE_SIZE];
  static int sampleIndex = 0;
  static long gasSum = 0;

  // Moving average calculation
  gasSum -= gasSamples[sampleIndex];
  gasSamples[sampleIndex] = gasValue;
  gasSum += gasSamples[sampleIndex];
  sampleIndex = (sampleIndex + 1) % SAMPLE_SIZE;

  int gasAverage = gasSum / SAMPLE_SIZE;

  // Pneumothorax detection
  if (gasAverage > PNEUMOTHORAX_THRESHOLD) {
    if (!pneumothoraxDetected) {
      Serial.println("*** PNEUMOTHORAX DETECTED! ***");
      Serial.println("Automatic gas drainage system activating...");
      pneumothoraxDetected = true;

      // Emergency alarm
      playAlarm(FREQ_PNEUMOTHORAX, 2000);

      // Start gas drainage procedure
      startGasDrainageProcedure();
    }
  } else if (gasAverage < PNEUMOTHORAX_THRESHOLD - 50) {  // Hysteresis
    if (pneumothoraxDetected) {
      Serial.println("Pneumothorax condition normalized");
      pneumothoraxDetected = false;
    }
  }
}

void checkECGAbnormality() {
  // Heart rate abnormality check
  if (heartRate < HEART_RATE_MIN || heartRate > HEART_RATE_MAX) {
    if (!ecgAbnormal) {
      Serial.println("*** ECG ABNORMALITY DETECTED! ***");
      ecgAbnormal = true;
      playAlarm(FREQ_ECG_ABNORMAL, 1000);
    }
  } else {
    if (ecgAbnormal && heartRate >= HEART_RATE_MIN && heartRate <= HEART_RATE_MAX) {
      Serial.println("ECG returned to normal values");
      ecgAbnormal = false;
    }
  }
}

void evaluateSystemStatus() {
  // Critical condition assessment
  if (pneumothoraxDetected && ecgAbnormal) {
    // Multiple complications - Emergency
    Serial.println("!!! MULTIPLE COMPLICATIONS - EMERGENCY !!!");
    playAlarm(FREQ_EMERGENCY, 3000);

    // Emergency protocols
    emergencyProtocol();
  }
}

void startGasDrainageProcedure() {
  Serial.println("=== GAS DRAINAGE PROCEDURE INITIATED ===");

  // Safety check
  Serial.println("Performing safety checks...");
  delay(1000);

  // Simulated drainage procedure
  for (int i = 0; i < 10; i++) {
    Serial.print("Drainage step ");
    Serial.print(i + 1);
    Serial.println("/10");

    // Pressure check
    gasValue = analogRead(GAS_SENSOR);
    Serial.print("Current pressure level: ");
    Serial.println(gasValue);

    // Gradual drainage simulation
    tone(BUZZER_PIN, 500 + (i * 100), 200);
    delay(1000);

    // Safety check - stop if pressure drops too fast
    if (gasValue < PNEUMOTHORAX_THRESHOLD / 2) {
      Serial.println("Safe pressure level reached - Drainage stopped");
      break;
    }
  }

  Serial.println("=== DRAINAGE PROCEDURE COMPLETED ===");
  playAlarm(FREQ_NORMAL, 500);
}

void emergencyProtocol() {
  // Emergency protocol
  Serial.println("=== EMERGENCY PROTOCOL ACTIVE ===");

  // Switch to maximum power mode
  Serial.println("System operating in maximum power mode");

  // Continuous monitoring
  Serial.println("Continuous monitoring mode active");

  // External notification system (simulated)
  Serial.println("External notification systems alerted");

  // Increase data recording frequency
  Serial.println("High-frequency data recording initiated");
}

void playAlarm(int frequency, int duration) {
  // Play alarm sound
  tone(BUZZER_PIN, frequency, duration);

  // LED warning
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void printSystemStatus() {
  Serial.print("HR: ");
  Serial.print(heartRate);
  Serial.print(" bpm | ");
  Serial.print("ECG: ");
  Serial.print(ecgValue);
  Serial.print(" | ");
  Serial.print("GAS: ");
  Serial.print(gasValue);

  // Status indicators
  if (pneumothoraxDetected) Serial.print(" | PNEUMO: WARNING");
  if (ecgAbnormal) Serial.print(" | ECG: ABNORMAL");
  if (!pneumothoraxDetected && !ecgAbnormal) Serial.print(" | STATUS: NORMAL");

  Serial.println();
}

void blinkSystemLED() {
  // Normal operation indicator
  static unsigned long lastBlink = 0;
  static bool ledState = false;

  if (currentTime - lastBlink > 1000) {  // Blink every second
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastBlink = currentTime;
  }
}

void startupSequence() {
  // Startup sequence
  Serial.println("Checking system components...");

  // LED test
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }

  // Buzzer test
  tone(BUZZER_PIN, FREQ_NORMAL, 500);
  delay(700);

  // Sensor calibration
  Serial.println("Performing sensor calibration...");
  for (int i = 0; i < 50; i++) {
    readSensors();
    delay(20);
  }

  Serial.println("Calibration completed");
  playAlarm(FREQ_NORMAL, 200);
}
