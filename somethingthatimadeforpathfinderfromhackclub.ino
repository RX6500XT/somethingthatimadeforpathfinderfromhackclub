// Button pins - using the NodeMCU D pins
int button1Pin = D0;
int button2Pin = D1;  
int button3Pin = D2;

// LED output pins
int led1Pin = D3;
int led2Pin = D4;
int led3Pin = D5;

int buzzerPin = D6; // buzzer for sound effects

// Timing variables for button presses
unsigned long button1PressTime = 0;
unsigned long button2PressTime = 0;
int button3HoldCounter = 0;  // counter for how long button 3 is held

void setup() {
  // Set button pins as inputs with pullup resistors
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  
  // Set LED pins as outputs
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  
  pinMode(buzzerPin, OUTPUT);
  
  // Initialize all LEDs to off state
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
}

void loop() {
  // Button 1 logic - simple LED control
  if (digitalRead(button1Pin) == HIGH) {
    button1PressTime = millis();  // record when button was pressed
    digitalWrite(led1Pin, HIGH);   // turn on LED1
  } else {
    digitalWrite(led1Pin, LOW);    // turn off LED1 when button released
  }
  
  // Button 2 logic - just record the press time
  if (digitalRead(button2Pin) == HIGH) {
    button2PressTime = millis();
  }
  
  // Check if button 1 and 2 were pressed within 2 seconds of each other
  long timeDifference = abs(button2PressTime - button1PressTime);
  if (timeDifference <= 2000) {  // 2000ms = 2 seconds
    digitalWrite(led2Pin, HIGH);
  } else {
    digitalWrite(led2Pin, LOW);
  }
  
  // Button 3 hold detection
  if (digitalRead(button3Pin) == HIGH) {
    button3HoldCounter++;  // increment counter while held
  } else {
    button3HoldCounter = 0;  // reset counter when released
  }
  
  // Check if button 3 has been held long enough (win condition)
  if (button3HoldCounter >= 800) {  // roughly 8 seconds with 10ms delay
    digitalWrite(led3Pin, HIGH);
    playHaloTheme();  // play victory sound
  } else {
    digitalWrite(led3Pin, LOW);
  }
  
  delay(10);  // small delay for stability
}

// Function to play a short version of the Halo theme
void playHaloTheme() {
  // Note frequencies for the melody
  int melody[] = {294, 392, 440, 392, 349, 392, 440, 523};
  // Note durations in milliseconds  
  int noteDurations[] = {350, 350, 400, 350, 300, 350, 400, 600};
  
  // Play each note in sequence
  for (int noteIndex = 0; noteIndex < 8; noteIndex++) {
    int duration = noteDurations[noteIndex];
    tone(buzzerPin, melody[noteIndex], duration);
    
    // Make LED3 blink with the music for extra effect
    if (noteIndex % 2 == 0) {
      digitalWrite(led3Pin, HIGH);
    } else {
      digitalWrite(led3Pin, LOW);
    }
    
    delay(duration + (duration * 0.3)); // slight pause between notes
  }
  
  noTone(buzzerPin);        // stop the buzzer
  digitalWrite(led3Pin, LOW); // turn off LED3
}