#include <ros.h>
#include <arduino_msgs/SensorsMsg.h>
#include <std_msgs/String.h>

#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DELAY_FACTOR  (100)
#define NUM_OF_DIGITS (2)
#define DHT11_PIN 8

/* DHT */
DHT dht(8, DHT11); //Pin, Jenis DHT

/* LCD */
LiquidCrystal_I2C lcd(0x27,16,2);

/* 4 digit 7 segment */
// 4 display on/off pin (for the common anode/cathode)
const int control_pins[NUM_OF_DIGITS] = {3,4};
// pin 11 of 74HC595 (SHCP)
const int bit_clock_pin = 5;
// pin 12 of 74HC595 (STCP)
const int digit_clock_pin = 6;
// pin 14 of 74HC595 (DS)
const int data_pin = 7;
// digit pattern for a 7-segment display
const byte digit_pattern[16] =
{
  B00111111,  // 0
  B00000110,  // 1
  B01011011,  // 2
  B01001111,  // 3
  B01100110,  // 4
  B01101101,  // 5
  B01111101,  // 6
  B00000111,  // 7
  B01111111,  // 8
  B01101111,  // 9
  B01110111,  // A
  B01111100,  // b
  B00111001,  // C
  B01011110,  // d
  B01111001,  // E
  B01110001   // F
};

int digit_data[NUM_OF_DIGITS] = {0};
int scan_position = 0;

String curr_time;

int mode = 0;
int prev_mode = 2;

/* ROS Setup */
ros::NodeHandle nh;

arduino_msgs::SensorsMsg msg;
ros::Publisher sensors_pub("/data/sensors", &msg);

void setup()
{
  int i;
  // set related pins as output pins
  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    pinMode(control_pins[i], OUTPUT);
  }
  pinMode(data_pin, OUTPUT);
  pinMode(bit_clock_pin, OUTPUT);
  pinMode(digit_clock_pin, OUTPUT);

  pinMode(2, INPUT);

  dht.begin();
  lcd.begin();
  nh.getHardware()->setBaud(9600);
  nh.initNode();
  nh.advertise(sensors_pub);
}
 
void update_one_digit()
{
  int i;
  byte pattern;
  
  // turn off all digit
  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    digitalWrite(control_pins[i], LOW);
  }

  // get the digit pattern of the position to be updated
  pattern = digit_pattern[digit_data[scan_position]];

  // turn off the output of 74HC595
  digitalWrite(digit_clock_pin, LOW);
  
  // update data pattern to be outputed from 74HC595
  // because it's a common anode LED, the pattern needs to be inverted
  shiftOut(data_pin, bit_clock_pin, MSBFIRST, ~pattern);
  
  // turn on the output of 74HC595
  digitalWrite(digit_clock_pin, HIGH);

  // turn on the digit to be updated in this round
  digitalWrite(control_pins[scan_position], HIGH);

  // go to next update position
  scan_position++;
  
  if (scan_position >= NUM_OF_DIGITS)
  {
    scan_position = 0; 
  }
}

void loop()
{ 
  int temp = dht.readTemperature();
  int humid = dht.readHumidity();
  int button_state = digitalRead(2);
  int ldr = analogRead(A3);
  int i;
  unsigned int number;
  number = temp;

  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    digit_data[i] = number % 10;
    number /= 10;
  }

  update_one_digit();
  if (button_state) {
    lcd.clear();
    if(mode == 2){
      mode == 0;
    }else{
      mode++;
    }
    delay(500);
  }
    /* Temp Display */
  if(mode == 0){
    lcd.setCursor(0,0);
    lcd.print("Hello!");
  }else if(mode == 1){
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.print(temp);
  }else if(mode == 2){
    lcd.setCursor(0,0);
    lcd.print("Humidity: ");
    lcd.print(humid);
  }
  
  /* Publish sensors data msg */
  msg.temp = temp;
  msg.light = ldr;
  sensors_pub.publish(&msg);

  nh.spinOnce();
}
