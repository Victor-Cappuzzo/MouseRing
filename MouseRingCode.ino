// Define the GPIO pins for the buttons
const int BUTTON_PIN_1 = D10;
const int BUTTON_PIN_2 = D9;
const int BUTTON_PIN_3 = D8;
const int BUTTON_PIN_4 = D7;

void setup() {
  
  // Initialize serial communication
  Serial.begin(115200);

  // Set button pins as inputs iwth internal pull-down enabled
  pinMode(BUTTON_PIN_1, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLDOWN);
  pinMode(BUTTON_PIN_4, INPUT_PULLDOWN);

}

void loop() {
  
  // Read the state of each button
  int buttonState1 = digitalRead(BUTTON_PIN_1);
  int buttonState2 = digitalRead(BUTTON_PIN_2);
  int buttonState3 = digitalRead(BUTTON_PIN_3);
  int buttonState4 = digitalRead(BUTTON_PIN_4);

  // Check if any button is pressed (HIGH indicates the button is pressed due to pull-down resistors)
  //if (buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH || buttonState4 == HIGH) {
    
    // Print a message indicating which button is pressed
    if (buttonState1 == HIGH) {
      Serial.println("Button 1 is pressed!");
    }

    if (buttonState2 == LOW) {
      Serial.println("Button 2 is pressed!");
    }

    if (buttonState3 == HIGH) {
      Serial.println("Button 3 is pressed!");
    }

    if (buttonState4 == HIGH) {
      Serial.println("Button 4 is pressed!");
    }

    // Add a small delay to debounce the buttons (optional)
    delay(200);
  //}

}

