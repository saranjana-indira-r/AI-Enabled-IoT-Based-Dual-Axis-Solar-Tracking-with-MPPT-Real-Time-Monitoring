#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Servo Objects
Servo servoX;
Servo servoY;

// LDR Pins
#define LDR_TOP     34
#define LDR_BOTTOM  35
#define LDR_LEFT    32
#define LDR_RIGHT   33

// Servo Pins
#define SERVO_X_PIN 18
#define SERVO_Y_PIN 19

int servoXAngle = 90;
int servoYAngle = 90;

void setup() {

  Serial.begin(115200);

  dht.begin();

  servoX.attach(SERVO_X_PIN);
  servoY.attach(SERVO_Y_PIN);

  servoX.write(servoXAngle);
  servoY.write(servoYAngle);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("OLED not found");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {

  int topValue = analogRead(LDR_TOP);
  int bottomValue = analogRead(LDR_BOTTOM);
  int leftValue = analogRead(LDR_LEFT);
  int rightValue = analogRead(LDR_RIGHT);

  float temperature = dht.readTemperature();

  // Horizontal Tracking
  if(leftValue > rightValue + 100){
    servoXAngle--;
  }
  else if(rightValue > leftValue + 100){
    servoXAngle++;
  }

  // Vertical Tracking
  if(topValue > bottomValue + 100){
    servoYAngle++;
  }
  else if(bottomValue > topValue + 100){
    servoYAngle--;
  }

  servoXAngle = constrain(servoXAngle,0,180);
  servoYAngle = constrain(servoYAngle,0,180);

  servoX.write(servoXAngle);
  servoY.write(servoYAngle);

  Serial.println("--------------------------------");
  Serial.print("Top LDR     : ");
  Serial.println(topValue);

  Serial.print("Bottom LDR  : ");
  Serial.println(bottomValue);

  Serial.print("Left LDR    : ");
  Serial.println(leftValue);

  Serial.print("Right LDR   : ");
  Serial.println(rightValue);

  Serial.print("Servo X     : ");
  Serial.println(servoXAngle);

  Serial.print("Servo Y     : ");
  Serial.println(servoYAngle);

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  display.clearDisplay();

  display.setCursor(0,0);
  display.println("SMART SOLAR");

  display.setCursor(0,12);
  display.print("Temp:");
  display.print(temperature);
  display.print(" C");

  display.setCursor(0,24);
  display.print("SX:");
  display.print(servoXAngle);

  display.setCursor(70,24);
  display.print("SY:");
  display.print(servoYAngle);

  display.setCursor(0,38);
  display.print("L:");
  display.print(leftValue);

  display.setCursor(70,38);
  display.print("R:");
  display.print(rightValue);

  display.setCursor(0,52);
  display.print("Tracking...");

  display.display();

  delay(200);
}