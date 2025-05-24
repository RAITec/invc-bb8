#include <DabbleESP32.h>
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

// Definição dos pinos dos motores
int motor1IN1 = 18; 
int motor1IN2 = 19; 
int motor1EN = 14; 
int motor2IN1 = 25;
int motor2IN2 = 33;
int motor2EN = 32;

// Definição dos pinos da ESP32 para comunicação com o DFPlayer Mini
static const uint8_t PIN_MP3_TX = 26;
static const uint8_t PIN_MP3_RX = 27;
HardwareSerial mySerial(1);

// Cria uma instância do DFPlayer Mini
DFRobotDFPlayerMini player;
 
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
  
  // Configuração dos motores
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
  
  // Configuração do DFPlayer
  mySerial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
  
  Serial.println("Inicializando DFPlayer...");
  
  if (player.begin(mySerial)) {
      Serial.println("DFPlayer Mini inicializado com sucesso!");
      
      player.volume(20);
      player.outputDevice(DFPLAYER_DEVICE_SD);
      
      player.playFolder(1, 1);
      Serial.println("Tocando música teste");
  } else {
      Serial.println("Falha ao inicializar DFPlayer Mini");
      while(true);
  }
}
 
void loop() {
  Dabble.processInput();
  
  // Controle de movimento
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
    player.playFolder(2,1);
    Serial.println("Tocando musica 1 da pasta 2");
  }
  else {
    digitalWrite(motor1IN1, LOW);
    digitalWrite(motor1IN2, HIGH);
    player.playFolder(3,3);
    Serial.println("Tocando musica 3 da pasta 3");
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
  
  // Controle de som
  
  if (GamePad.isTrianglePressed()) {
      player.playFolder(4,1);
      Serial.println("Tocando musica 1 da pasta 4");
      delay(300);
  }
  
  if (GamePad.isCrossPressed()) {
      player.playFolder(1,1);
      Serial.println("Tocando musica 1 da pasta 1");
      delay(300);
  }
  
  if (GamePad.isCirclePressed()) {
      player.playFolder(3,1);
      Serial.println("Tocando musica 1 da pasta 3");
      delay(300);
  }
  
  if (GamePad.isSquarePressed()) {
      player.playFolder(3,2);
      Serial.println("Tocando musica 2 da pasta 3");
      delay(300);
  }
  
  delay(20);
}

void playRandomFromFolder(uint8_t folder) {
    uint8_t randomTrack = random(1, 11);
    
    Serial.print("Tocando música aleatória da pasta ");
    Serial.print(folder);
    Serial.print(", faixa ");
    Serial.println(randomTrack);
    
    player.playFolder(folder, randomTrack);
}