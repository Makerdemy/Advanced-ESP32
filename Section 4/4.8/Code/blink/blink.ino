void setup() {
  // put your setup code here, to run once:
   pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(5, HIGH);
    delay(1000);
    digitalWrite(5, LOW);
    delay(1000);
}
