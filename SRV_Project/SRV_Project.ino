#include <Arduino_FreeRTOS.h>

#define MAX_TASKS 5
#define MAX_READ_LEN 64

char read_array[MAX_READ_LEN];

// Serial.print() init
void print_msg();
void print_msgln();
void print_num();
void print_numln();

// Serial.read() init
char* read_input();

// Functions init
void vPeriodicFunc(void * params);
void vAperiodicFunc(void * params);
void vLoggerFunc(void * params);



// Main
void setup() {
    Serial.begin(9600);
    Serial.println(F("------ SRV ------"));
//    Serial.println(F("Server:    S;5;10"));
//    Serial.println(F("Periodic:  P;p1;5;10;p1"));
//    Serial.println(F("Aperiodic: A;a1;5;10;50;a1"));
    Serial.flush();

    // B|S;2;8|P;p1;5;10;a|P;p2;1;5;b|A;a1;5;2;10;c|
    
    // Serial.print() setup
    set_debug_msg( &print_msg );
    set_debug_msgln( &print_msgln );
    set_debug_num( &print_num );
    set_debug_numln( &print_numln );

    // Periodic function
    set_vPeriodicFunc( &vPeriodicFunc );

    // Aperiodic function
    set_vAperiodicFunc( &vAperiodicFunc );
  
//    vInitialiseServerTask(     10,         5);

    //if (xIsSchedulable(3) == pdTRUE) Serial.println(F("Batch is schedulable."));
    //else Serial.println(F("Batch is NOT schedulable."));

    xTaskCreateLogger(vLoggerFunc, "logger", 80, NULL, 3, NULL,         4);
    
}



// Idle function
void loop() {
  //Serial.print("Im IDLE\n");
  //Serial.flush();
}



// Periodic task function
void vPeriodicFunc(void * pvParameters) {
  
  struct xTaskParameter * param = (struct xTaskParameter *) pvParameters;
  char * pcTaskName = param->pcTaskName;
  char * pcParameter = param->pcParameter;
  TickType_t xCapacity = param->xCapacity;

  TickType_t tick_counter = 1;
  TickType_t temp = portMAX_DELAY;
  
  while(tick_counter <= xCapacity) {
      if(temp != xTaskGetTickCount()){
          temp = xTaskGetTickCount();
          Serial.print(pcTaskName);
          Serial.print(F(" tick: "));
          Serial.println(temp);
//          Serial.print(F(" -> "));
//          Serial.println(pcParameter);
          Serial.flush();
          tick_counter++;
      }
  }

  while (temp == xTaskGetTickCount());

  vTaskMyDelete(0);
}



// Aperiodic task function
void vAperiodicFunc(void * pvParameters) {
  
  struct xTaskParameter * param = (struct xTaskParameter *) pvParameters;
  char * pcTaskName = param->pcTaskName;
  char * pcParameter = param->pcParameter;
  TickType_t xCapacity = param->xCapacity;

  TickType_t tick_counter = 1;
  TickType_t temp = portMAX_DELAY;
  
  while(tick_counter <= xCapacity) {
      if(temp != xTaskGetTickCount()){
          temp = xTaskGetTickCount();
          Serial.print(pcTaskName);
          Serial.print(F(" tick: "));
          Serial.println(temp);
//          Serial.print(F(" -> "));
//          Serial.println(pcParameter);
          Serial.flush();
          tick_counter++;
          vDecrementServerCapacity();
      }
  }

  while (temp == xTaskGetTickCount());

  vTaskDelete(0);
}



// Logger function
void vLoggerFunc(void * pvParameters){
  
  while(1) {
//      Serial.print(F("Logger - tick: "));
//      Serial.println(xTaskGetTickCount());
//      Serial.flush();
      
      char* serial_input = read_input();
      switch (serial_input[0]) {
          case 'S': vParseInputServer(serial_input);
              break;
          case 'P': vParseInputPeriodic(serial_input);
              break;
          case 'A': vParseInputAperiodic(serial_input);
              break;
          case 'D': vDeleteTask(serial_input);
              break;
          case 'B': vParseInputBatch(serial_input);
              break;
          case 'R': vRecomendServerCapacity(serial_input);
              break;
          default:
              break;
      }
      vTaskDelay(0);
  }
  
  vTaskDelete(0);
}



// Serial.print() impl
void print_num(double number) {
  Serial.print(number);
  Serial.flush();
}
void print_numln(double number) {
  Serial.println(number);
  Serial.flush();
}
void print_msg(char * msg) {
  Serial.print(msg);
  Serial.flush();
}
void print_msgln(char * msg) {
  Serial.println(msg);
  Serial.flush();
}

// Serial.read() impl
char* read_input() {
  int i;
  if (Serial.available()){
      for (i = 0; i < MAX_READ_LEN; i++){
          char c = Serial.read();
          if (c == '\r') break;
          read_array[i] = c;
      }
      read_array[i] = '\0';
      return read_array;
  } else {
      return NULL;
  }
}
