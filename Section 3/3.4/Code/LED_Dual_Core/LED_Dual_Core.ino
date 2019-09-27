
TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
const int RedLED = 25;
const int GreenLED = 5;

void setup() {
  Serial.begin(115200); 
  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1500 ms
void Task1code( void * pvParameters ){
  for(;;){
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());

    digitalWrite(RedLED, HIGH);
    delay(1400);
    digitalWrite(RedLED, LOW);
    delay(1400);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  for(;;){
    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());

    digitalWrite(GreenLED, HIGH);
    delay(700);
    digitalWrite(GreenLED, LOW);
    delay(700);
  }
}

void loop() {
  
}
