#define BUZZER_PIN 23   // Buzzer pin
#define BUTTON_PIN 32   // Button pin

// Define musical notes
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976

// Speed levels (multipliers for note duration)
float speedLevels[] = {0.25, 0.5, 1, 1.5, 2};  
int speedIndex = 0;  // เริ่มที่ "0.25" (index 0)

// Debounce variables
volatile unsigned long lastPressTime = 0; // เวลาที่กดปุ่มล่าสุด
const unsigned long debounceDelay = 200;  // กำหนดดีเลย์ 200ms

// Super Mario Bros. Theme (Intro Part)
int melody[] = {
  NOTE_E6, NOTE_E6, 0, NOTE_E6, 0, NOTE_C6, NOTE_E6, 0,
  NOTE_G6, 0, 0, 0, NOTE_G5, 0, 0, 0,
  NOTE_C6, 0, 0, NOTE_G5, 0, 0, NOTE_E5, 0,
  0, NOTE_A5, 0, NOTE_B5, 0, NOTE_AS5, NOTE_A5, 0,
  NOTE_G5, NOTE_E6, NOTE_G6, NOTE_A6, 0, NOTE_F6, NOTE_G6, 0,
  NOTE_E6, 0, NOTE_C6, NOTE_D6, NOTE_B5, 0, 0
};

// Note durations (milliseconds)
int noteDurations[] = {
  150, 150, 150, 150, 150, 150, 150, 150,
  150, 150, 150, 150, 150, 150, 150, 150,
  150, 150, 150, 150, 150, 150, 150, 150,
  150, 150, 150, 150, 150, 150, 150, 150,
  150, 150, 150, 150, 150, 150, 150, 150,
  150, 150, 150, 150, 150, 150, 150
};

// Interrupt Service Routine (ISR) for button press
void IRAM_ATTR changeSpeed() {
  unsigned long currentTime = millis();
  if (currentTime - lastPressTime > debounceDelay) {  // ตรวจสอบว่าผ่านไปอย่างน้อย 200ms
    lastPressTime = currentTime;  // อัปเดตเวลาที่กดปุ่มล่าสุด
    speedIndex = (speedIndex + 1) % 5;  

    Serial.print("Speed changed to: ");
    Serial.println(speedLevels[speedIndex]);
  }
}

void setup() {
  Serial.begin(115200);  // เริ่มต้น Serial Monitor
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), changeSpeed, FALLING);
}

void loop() {
  int numNotes = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < numNotes; i++) {
    int adjustedDuration = noteDurations[i] / speedLevels[speedIndex];  // ปรับความเร็วให้เร็วขึ้นเมื่อ speed เพิ่ม

    if (melody[i] == 0) {  // Handle rest notes
      noTone(BUZZER_PIN);
    } else {
      tone(BUZZER_PIN, melody[i], adjustedDuration);
    }
    
    delay(adjustedDuration * 1.3);  // Short pause between notes
    noTone(BUZZER_PIN);
  }

  delay(2000);  // Pause before repeating the song
}
