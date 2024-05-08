/*
  BunHMI Demo
*/
#include <Servo.h>
#include <arduino-timer.h> // https://www.arduino.cc/reference/en/libraries/arduino-timer/
#define EOT 0x04 // Buntalk script termination

auto timer = timer_create_default();

Servo myservo;  // create servo object to control a servo

/**
* timer tick call back
*/
bool timer_tick(void*) {
  char buff[256];

  static int sec_tick_cnt = 0;
  static int test_cnt = 0;
  if (++sec_tick_cnt >= 1000) {
    // 1 Sec
    sec_tick_cnt = 0;
    snprintf(buff, sizeof(buff),"lab.text(\"Timer:%d\");\x04", test_cnt++);
    Serial1.print(buff);
  }
  if ((sec_tick_cnt % 100) == 0) {
    // 100ms
    int sensorValue = (analogRead(A0) * 100) / 1024;
    snprintf(buff,  sizeof(buff), "bar.val(%d,1);", sensorValue);
    Serial1.print(buff);    
    Serial1.write(EOT);
  }
  return true;
}

// the setup routine runs once when you press reset:
void setup() {
  // Set  POW to PWM mode, reduce ripple for ADC
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  myservo.attach(D2);  // attaches the servo on pin 2 to the servo object
  // initialize serial communication at 115200 bits per second:
  Serial1.setRX(PIN_SERIAL1_RX);
  Serial1.setTX(PIN_SERIAL1_TX);
  Serial1.begin(115200);
  Serial1.setTimeout(10);

  timer.every(1, timer_tick);
  // Send dummy cmd, to clear rx buffer HMI
  Serial1.print("\x04\x04");
  // clear serial rx buffer of pico
  delay(100);
  char buff[256];
  int rxlen;
  do{
    rxlen = Serial1.readBytes(buff, sizeof(buff));
  }while(rxlen > 0);  
}

/**
  * Description: Receive Hmi Data
  * Parameters:
  *  dat: Data buffer
  *  dat_len: Data buffer length
  * Return:
  *  length of rx bytes
*/
int rxHmiData(char* dat, int dat_len) {
  if (!Serial1.available()) {
    return 0;
  }
  int rxlen = Serial1.readBytes(dat, dat_len);
  if(rxlen > 0){
    dat[rxlen-1] = 0; // Replace 0x04(EOT) to 0
    return rxlen;
  }
  return 0;
}

// String from BunHMI ptr cmd
const char* LED_ = "LED:";
const char* SLI_ = "SLI:";
void handleHmiData(const char* dat) {
  int val;
  if (strncmp(dat, LED_, strlen(LED_)) == 0) {
    val = strtoul(dat + strlen(LED_), NULL, 0);
    digitalWrite(LED_BUILTIN, val);
    return;
  }
  if (strncmp(dat, SLI_, strlen(SLI_)) == 0) {
    val = strtoul(dat + strlen(SLI_), NULL, 0);        
    int servo = map(val, 0, 100, 0, 180);     // scale it to use it with the servo (value between 0 and 180)    
    myservo.write(servo);           
    return;
  }
}
// the loop routine runs over and over again forever:
void loop() {
  char rxbuff[256];
  int rxlen;

  timer.tick();
  // Check Data from HMI
  rxlen = rxHmiData(rxbuff, sizeof(rxbuff));
  if (rxlen) {
    // Handle data from HMI
    handleHmiData(rxbuff);
  }
}