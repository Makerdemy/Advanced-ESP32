
TaskHandle_t Producer;
TaskHandle_t Consumer;

QueueHandle_t queue;
int queueSize = 10;
const int RedLED = 25;
 
void setup() {
 
  Serial.begin(112500);
  pinMode(RedLED, OUTPUT);
  queue = xQueueCreate( queueSize, sizeof(int) );
 
  xTaskCreatePinnedToCore(
                    producerTask,     /* Task function. */
                    "Producer",       /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    &Producer,
                    0);            
 
  xTaskCreatePinnedToCore(
                    consumerTask,     /* Task function. */
                    "Consumer",       /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    &Consumer,
                    1);            

}

 
void producerTask( void * parameter )
{
  for(;;)
  {
    int val = hallRead();
    for( int i = 0;i<queueSize;i++ )
    {
      xQueueSend(queue, &val, portMAX_DELAY);
    }
  } 
  delay(200);
}
 
void consumerTask( void * parameter)
{
  for(;;)
  {
    int element;
    for( int i = 0;i < queueSize;i++ )
    {
     xQueueReceive(queue, &element, portMAX_DELAY);
     if(element<0 || element>25)
        {
           digitalWrite(RedLED, HIGH);
         }
     else
         {
           digitalWrite(RedLED, LOW);
          }
     }
   delay(200);
  }
}

void loop() {
  
}
