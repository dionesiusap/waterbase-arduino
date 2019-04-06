#define DELAY_FACTOR  (100)
#define NUM_OF_DIGITS (2)
#include <DHT.h>
#define DHT11_PIN 8
String input;
DHT dht(8, DHT11); //Pin, Jenis DHT
// 4 display on/off pin (for the common anode/cathode)
const int control_pins[NUM_OF_DIGITS] = {3,4};
// pin 11 of 74HC595 (SHCP)
const int bit_clock_pin = 5;
// pin 12 of 74HC595 (STCP)
const int digit_clock_pin = 6;
// pin 14 of 74HC595 (DS)
const int data_pin = 7;
bool lampu1 = false;
bool lampu2 = false;
bool lampu3 = false;
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

unsigned int counter = 0;

void setup()
{
  int i;

  // set related pins as output pins
  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    pinMode(control_pins[i], OUTPUT);
  }
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(bit_clock_pin, OUTPUT);
  pinMode(digit_clock_pin, OUTPUT);  
  Serial.begin(9600);
  dht.begin();
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
  int i;
  unsigned int number;
  int temp;
  unsigned int digit_base;
  number = dht.readTemperature();
  counter++;

  digit_base = 10;
  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    digit_data[i] = number % digit_base;
    number /= digit_base;
  }
  if (Serial.available() > 0) {

    input = Serial.readString(); // read the incoming byte:
    input.trim();
    if(input == "lampu1"){
      if(lampu1){
        lampu1 = false;
      }else{
        lampu1 = true;
      }
    }else if(input == "lampu2"){
      if(lampu2){
        lampu2 = false;
      }else{
        lampu2 = true;
      }
    }else if(input == "lampu3"){
      if(lampu3){
        lampu3 = false;
      }else{
        lampu3 = true;
      }
    }
    
  }
  if(lampu1){
    digitalWrite(10, HIGH);
  }else{
    digitalWrite(10, LOW);
  }
  if(lampu2){
    digitalWrite(11, HIGH);
  }else{
    digitalWrite(11, LOW);
  }
  if(lampu3){
    digitalWrite(12, HIGH);
  }else{
    digitalWrite(12, LOW);
  }
  update_one_digit();
  
  delay(4);
}
