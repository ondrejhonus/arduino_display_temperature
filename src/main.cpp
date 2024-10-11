#include <Arduino.h>
#include <SimpleDHT.h>
#include <Bounce2.h>

#define ROWS 10
#define COLS 8
#define D1 2
#define D2 3
#define DOTPIN 11
#define BUTTON_PIN 13

Bounce2::Button button = Bounce2::Button();

bool numbers[ROWS][COLS] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 1, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}};// 9

int pins[COLS] = {4, 5, 6, 7, 8, 9, 10, 11};

int pinDHT11 = 12;
SimpleDHT11 dht11(pinDHT11);
int dot = 0;

unsigned long currentTime;
unsigned long loopTime;
byte temperature;
byte humidity;

void setup() {
  for (int i = 0; i < COLS; i++) {
    pinMode(pins[i], OUTPUT);
  }
  button.attach(BUTTON_PIN, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  Serial.begin(9600);
  currentTime = millis();
  loopTime = currentTime;
  button.setPressedState(LOW); 
}

void set_num(int number) {
  for (int i = 0; i < COLS; i++) {
    digitalWrite(pins[i], numbers[number][i]);
  }
}

void set_both_digits(int number) {
  int first_num = number / 10;
  int second_num = number % 10;
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  set_num(first_num);
  if(dot == 0){
    digitalWrite(DOTPIN, HIGH);
  }
  delay(1);
  digitalWrite(DOTPIN, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D1, HIGH);
  set_num(second_num);
    if(dot == 1){
    digitalWrite(DOTPIN, HIGH);
  }
  delay(1);
  digitalWrite(DOTPIN, LOW);
}

int sense = 0; // 0 = Temperature, 1 = Humidity

void loop() {
  button.update();

  currentTime = millis();
  if (currentTime >= (loopTime + 5000) || currentTime == 0) {
    dht11.read(&temperature, &humidity, NULL);
    loopTime = currentTime;
  }

  if (button.pressed()){
    sense = !sense;
  }

  if (sense == 0) {
    set_both_digits(temperature);
    dot = 0;
  }
  else if (sense == 1) {
    set_both_digits(humidity);
    dot = 1;
  }
}
