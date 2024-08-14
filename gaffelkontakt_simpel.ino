// Define pins for the RGB LED
const int redPin = 25;      // Red pin of the RGB LED connected to GPIO 25
const int greenPin = 26;    // Green pin of the RGB LED connected to GPIO 26

// Define pin for the button
const int buttonPin = 12;   // Button connected to GPIO 12

// Define pin for Raspberry Pi signal
const int outputPin = 18;   // Pin to send signal to Raspberry Pi

// Variables to store button state and timing
bool lastButtonState = HIGH;
unsigned long buttonLastReleaseTime = 0;
unsigned long releaseDurationThreshold = 2000; // Duration threshold in milliseconds (2 seconds)
bool signalSentOnPress = false;
bool signalSentOnRelease = false;

void setup() {
  Serial.begin(115200);

  // Initialize the RGB LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // Initialize the button pin as input with an internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize the output pin for Raspberry Pi
  pinMode(outputPin, OUTPUT);

  // Set the initial state of the RGB LED to red (button not pressed)
  setRGBColor(255, 0); // Red color

  Serial.println("Starting...");
}

void loop() {
  // Check button state
  checkButton();
}

void checkButton() {
  // Read the current state of the button
  bool currentButtonState = digitalRead(buttonPin);

  // If the button is pressed and a signal has not been sent yet
  if (currentButtonState == LOW && !signalSentOnPress) {
    setRGBColor(0, 255); // Green color
    digitalWrite(outputPin, HIGH); // Send signal to Raspberry Pi
    Serial.println("Button pressed - Signal sent to Raspberry Pi");
    signalSentOnPress = true;
    signalSentOnRelease = false; // Reset release signal flag
    buttonLastReleaseTime = millis(); // Reset release timer
  }

  // If the button is released
  if (currentButtonState == HIGH) {
    // Start timing the release
    if (!signalSentOnRelease && (millis() - buttonLastReleaseTime >= releaseDurationThreshold)) {
      setRGBColor(255, 0); // Red color
      digitalWrite(outputPin, LOW); // Send signal to Raspberry Pi
      Serial.println("Button released for over 2 seconds - Signal sent to Raspberry Pi");
      signalSentOnRelease = true;
      signalSentOnPress = false; // Reset press signal flag for next cycle
    }
  } else {
    // Reset the release timer when the button is pressed
    buttonLastReleaseTime = millis();
    signalSentOnRelease = false; // Allow release signal in the next release
  }

  // Update last state
  lastButtonState = currentButtonState;
}

void setRGBColor(int red, int green) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
}
