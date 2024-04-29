const int LED_PIN = 3; // Define the pin connected to the LED

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.setTimeout(20);
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Check if data is available to read
  if (Serial.available()) {
    // Read string value
    String valueStr = Serial.readString();
    
    // Convert string to integer
    int value = valueStr.toInt();
    
    // Print received value
    //Serial.print("Received value: ");
    //Serial.println(value);
    
    // Example: Turn on LED if received value is 42
    if (value == 42) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
