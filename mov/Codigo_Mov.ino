#include <DabbleESP32.h>

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

// Definição dos pinos
int motor1IN1 = 18; 
int motor1IN2 = 19; 
int motor1EN = 14; 
int motor2IN1 = 25;
int motor2IN2 = 33;
int motor2EN = 32;
 
// Propriedades PWM
const int freq = 10000;
const int pwmChannelA = 0;
const int resolution = 8;
const int pwmChannelB = 1;

// Variaveis Joystick
float joystickX, joystickY;
int currentSpeedA = 0;
int currentSpeedB = 0;

const int step = 5;
const int delayTime = 30;
 
void setup() {
  Serial.begin(9600);
  Dabble.begin("RAI-BB8");

  pinMode(motor1IN1, OUTPUT);
  pinMode(motor1IN2, OUTPUT);
  pinMode(motor1EN, OUTPUT);
  pinMode(motor2IN1, OUTPUT);
  pinMode(motor2IN2, OUTPUT);
  pinMode(motor2EN, OUTPUT);

  // configura o PWM
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  
  ledcAttachPin(motor1EN, pwmChannelA);
  ledcAttachPin(motor2EN, pwmChannelB);

}
 
void loop() {
  Dabble.processInput();

  joystickX = GamePad.getx_axis();
  joystickY = GamePad.gety_axis();

  float baseSpeed = map(joystickY, -7, 7, -255, 255);
  float turningSpeed = map(joystickX, -7, 7, -255, 255);


  Serial.print("Valor de X: ");
  Serial.println(joystickX);
  Serial.print("Valor de Y: ");
  Serial.println(joystickY);

  float targetSpeedA = baseSpeed + turningSpeed;
  float targetSpeedB = baseSpeed - turningSpeed;

  Serial.print("Velocidade A: ");
  Serial.println(targetSpeedA);
  Serial.print("Velocidade B: ");
  Serial.println(targetSpeedB);

  targetSpeedA = constrain(targetSpeedA, -255, 255);
  targetSpeedB = constrain(targetSpeedB, -255, 255);

  if (currentSpeedA < targetSpeedA)
      currentSpeedA = min(currentSpeedA + step, (int)targetSpeedA);
    else if (currentSpeedA > targetSpeedA)
      currentSpeedA = max(currentSpeedA - step, (int)targetSpeedA);
  
  if (currentSpeedB < targetSpeedB)
    currentSpeedB = min(currentSpeedB + step, (int)targetSpeedB);
    else if (currentSpeedB > targetSpeedB)
      currentSpeedB = max(currentSpeedB - step, (int)targetSpeedB);
  
  if (currentSpeedA >= 0) {
    digitalWrite(motor1IN1, HIGH);
    digitalWrite(motor1IN2, LOW);
  }
  else {
    digitalWrite(motor1IN1, LOW);
    digitalWrite(motor1IN2, HIGH);
  }
  ledcWrite(pwmChannelA, abs(currentSpeedA));

  if (currentSpeedB >= 0) {
      digitalWrite(motor2IN1, HIGH);
      digitalWrite(motor2IN2, LOW);
    } else {
      digitalWrite(motor2IN1, LOW);
      digitalWrite(motor2IN2, HIGH);
    }
    ledcWrite(pwmChannelB, abs(currentSpeedB));


}