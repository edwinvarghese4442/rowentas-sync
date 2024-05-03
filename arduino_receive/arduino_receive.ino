
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(15);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    parseDictionary(data);
  }
}

void parseDictionary(String data) {
  Serial.println(data);
  int startPos = 0;
  while (startPos < data.length()) {
    // Serial.println(startPos);
    // Serial.println(data.length());
    int colonPos = data.indexOf(':', startPos);
    Serial.println(colonPos);
    if (colonPos == -1) {
      break;
    }
    String key = data.substring(startPos, colonPos);
    int commaPos = data.indexOf(',', colonPos);
    Serial.println(commaPos);
    if (commaPos == -1) {
      break;
    }
    String valueStr = data.substring(colonPos + 1, commaPos);
    int value = valueStr.toInt(); // Convert value string to integer
    Serial.println(key);
    Serial.println(value);



    // Check for specific key-value combinations and illuminate the LED accordingly
    if (key == "togdisp" && value == 1) {
      digitalWrite(5, HIGH); // Turn on LED
    }
    else if (key == "togdisp" && value == 0) {
      digitalWrite(5, LOW);
    }

    if (key == "togmach" && value == 1) {
      digitalWrite(2, HIGH); // Turn on LED
    }
    else if (key == "togmach" && value == 0){
      digitalWrite(2, LOW);
    }

    if (key == "togpb" && value == 1) {
      digitalWrite(3, HIGH); // Turn on LED
    }
    else if (key == "togpb" && value == 0){
      digitalWrite(3, LOW);
    }
    
    // Add more conditions for other key-value combinations as needed

    startPos = commaPos + 1;
    Serial.println(startPos);
    Serial.println(data.length());
    // startPos < data.length()
  }
}
