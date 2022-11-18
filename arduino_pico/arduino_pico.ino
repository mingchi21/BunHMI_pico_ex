/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

#include <arduino-timer.h>

auto timer = timer_create_default();
int sec_tick_cnt = 0;

int test_cnt = 0;

bool sys_tick(void*){
  char buff[128];
  if(++sec_tick_cnt >= 1000){
    // 1 Sec
    sec_tick_cnt = 0;
    snprintf(buff, sizeof(buff),"w4.text(\"CNT:%d\")\n", test_cnt++);
    Serial1.print(buff);
  }
  if((sec_tick_cnt % 100) == 0){
    // 100ms
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // turn the LED on (HIGH is the voltage level)  
    
    int sensorValue = (analogRead(A0)*100)/1024;
    snprintf(buff, sizeof(buff),"w2.val(%d,1)\n", sensorValue);
    Serial1.print(buff);
  }
  return true;
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial communication at 9600 bits per second:
  Serial1.setRX(PIN_SERIAL1_RX);
  Serial1.setTX(PIN_SERIAL1_TX);
  Serial1.begin(115200);  
  //Serial1.println("Hello Pico");

  Serial.begin(9600); // USB serial

  timer.every(1, sys_tick);
}



// the loop routine runs over and over again forever:
void loop() {
  timer.tick();

  if(Serial1.available()){
    String str = Serial1.readString();
    Serial.print(str);
  }

  // read the input on analog pin 0:
  
  // int sensorValue = analogRead(A0);
  // // print out the value you read:
  // digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // Serial1.println(sensorValue);
  // delay(100);                      // delay in between reads for stability    
  // Serial.print("USB Ser=");
  // Serial.println(sensorValue);
  // digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  // delay(100);                      // wait for a second
}