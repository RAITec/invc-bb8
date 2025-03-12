#include <DabbleESP32.h> // Biblioteca para comunicação com o aplicativo Dabble
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DFRobotDFPlayerMini.h> // Biblioteca para o controle do MP3 Player
#include <HardwareSerial.h> // Biblioteca para manipulação de portas seriais
#include <driver/ledc.h> // Biblioteca para controle de LED e PWM no ESP32

// Definição dos pinos do motor
int IN1 = 13;
int IN2 = 14;
int IN3 = 34;
int IN4 = 25;
int ENA = 12;
int ENA = 12;
int ENB = 35;

// Definição dos pinos para o MP3 Player
static const uint8_t PIN_MP3_TX = 26;
static const uint8_t PIN_MP3_RX = 27;

// Inicializa a comunicação serial com o MP3 Player
HardwareSerial mySerial(1);
DFRobotDFPlayerMini player; // Objeto para manipular o MP3 Player

// Variáveis para os controles do joystick
float joystickX, joystickY;
int currentSpeedA = 0;
int currentSpeedB = 0;
const int step = 5; // Incremento da velocidade a cada iteração
const int delayTime = 50; // Delay para suavizar a aceleração
int modes = 2; // Modo inicial para controle do MP3 Player

void setup() {
    Serial.begin(9600); // Inicializa a porta serial para debug
    Dabble.begin("RAI_BB8"); // Configura o nome do dispositivo para o Dabble
 
    // Configura a comunicação serial com o MP3 Player
    mySerial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Inicializa o MP3 Player
    if (player.begin(mySerial)) {
        Serial.print("Tudo funcionando conforme o planejado!");
        player.volume(30); // Configura o volume entre 0 e 30
        player.playFolder(1, 1); // Toca a primeira música da pasta 1
    } else {
        Serial.print("Não funcionou");
    }

    // Cria a tarefa para controle do motor
    xTaskCreatePinnedToCore(
        motorControlTask, // Função que será executada
        "Motor Control", // Nome da tarefa
        10000, // Tamanho da pilha
        NULL, // Parâmetro para a função
        1, // Prioridade da tarefa
        NULL, // Handle da tarefa
        0 // Núcleo onde a tarefa será executada
    );

    // Cria a tarefa para controle do MP3 Player
    xTaskCreatePinnedToCore(
        mp3ControlTask,
        "MP3 Control",
        10000,
        NULL,
        1,
        NULL,
        1
    );
}

void loop() {
    // Loop principal vazio porque as tarefas são executadas em paralelo
    delay(1000);
}

void motorControlTask(void * parameter) {
    while (true) {
    Dabble.processInput(); // Processa os dados recebidos do aplicativo Dabble
    
    // Obtém os valores dos eixos do joystick
    joystickX = GamePad.getx_axis();
    joystickY = GamePad.gety_axis()
    
    // Calcula as velocidades base e de curva
    float baseSpeed = map(joystickY, -7, 7, -255, 255);
    float turningSpeed = map(joystickX, -7, 7, -255, 255);
    
    // Define as velocidades alvo para os motores
    float targetSpeedA = baseSpeed + turningSpeed;
    float targetSpeedB = baseSpeed - turningSpeed;
    
    // Garante que as velocidades estão dentro dos limites permitidos
    targetSpeedA = constrain(targetSpeedA, -255, 255);
    targetSpeedB = constrain(targetSpeedB, -255, 255);
    
    // Ajusta a velocidade atual para atingir a velocidade alvo
    if (currentSpeedA < targetSpeedA) {
        currentSpeedA = min(currentSpeedA + step, (int)targetSpeedA);
    } else if (currentSpeedA > targetSpeedA) {
        currentSpeedA = max(currentSpeedA - step, (int)targetSpeedA);
    }
    if (currentSpeedB < targetSpeedB) {
        currentSpeedB = min(currentSpeedB + step, (int)targetSpeedB);
    } else if (currentSpeedB > targetSpeedB) {
        currentSpeedB = max(currentSpeedB - step, (int)targetSpeedB);
    }
    
    // Configura os pinos de controle do motor A
    if (currentSpeedA >= 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }
    
    analogWrite(ENA, abs(currentSpeedA)); // Ajusta o PWM do motor A
    // Configura os pinos de controle do motor B
    if (currentSpeedB >= 0) {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    } else {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
    analogWrite(ENB, abs(currentSpeedB)); // Ajusta o PWM do motor B
    delay(delayTime); // Pequeno delay para suavizar as mudanças de velocidade
    }
}   

void mp3ControlTask(void * parameter) {
    int randomMusic;
    while (true) {
        // Controla o MP3 Player com base nos botões do GamePad
        if (GamePad.isSquarePressed()) {
            randomMusic = randomizer(); // Gera uma música aleatória
            player.playFolder(modes + 1, randomMusic);
        }
        if (GamePad.isCirclePressed()) {
            // Lógica para o botão círculo (não implementada)
        }
        if (GamePad.isCrossPressed()) {
            modes = (modes + 1) % 3; // Alterna entre os modos disponíveis
        }
        if (GamePad.isTrianglePressed()) {
            randomMusic = randomizer();
            player.playFolder(4, randomMusic);
        }
        if (GamePad.isStartPressed()) {
            // Lógica para o botão Start (não implementada)
        }
        if (GamePad.isSelectPressed()) {
            // Lógica para o botão Select (não implementada)
        }
        delay(100); // Pequeno delay para evitar processamento excessivo
    }
}

int randomizer() {
    return random(1, 6); // Gera um número aleatório entre 1 e 5 (inclusive)
}