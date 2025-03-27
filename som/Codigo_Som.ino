#include <DabbleESP32.h>          // Para comunicação Bluetooth com o Dabble
#define CUSTOM_SETTINGS           // Habilita configurações personalizadas do Dabble
#define INCLUDE_GAMEPAD_MODULE    // Habilita o módulo de Gamepad do Dabble
#include <DFRobotDFPlayerMini.h>  // Para controlar o módulo MP3
#include <HardwareSerial.h>       // Para comunicação serial com o DFPlayer

// Definição dos pinos da ESP32 para comunicação com o DFPlayer Mini
static const uint8_t PIN_MP3_TX = 26;  // Pino TX da ESP32 → RX do DFPlayer
static const uint8_t PIN_MP3_RX = 27;  // Pino RX da ESP32 → TX do DFPlayer

HardwareSerial mySerial(1);  // Usa Serial1 (UART1) da ESP32

// Cria uma instância do DFPlayer Mini
DFRobotDFPlayerMini player;

void setup() {
    // Inicializa a comunicação serial com o computador (para debug)
    Serial.begin(115200);
    
    // Inicializa o Dabble Bluetooth com o nome do dispositivo
    Dabble.begin("RAITec_BB8");
    
    // Configura a comunicação serial com o DFPlayer Mini
    mySerial.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);
    
    Serial.println("Inicializando DFPlayer...");
    
    // Iniciar o DFPlayer Mini
    if (player.begin(mySerial)) {
        Serial.println("DFPlayer Mini inicializado com sucesso!");
        
        // Configurações iniciais do player:
        player.volume(20);  // Define volume (0-30)
        player.outputDevice(DFPLAYER_DEVICE_SD);  // Usa o cartão SD como fonte
        
        // Toca a primeira música da pasta 1 ao iniciar
        player.playFolder(1, 1);
        Serial.println("Tocando música teste");
    } else {
        Serial.println("Falha ao inicializar DFPlayer Mini");
        while(true);  // Trava o programa se o player não iniciar
    }
}

void loop() {
    // Processa os comandos recebidos via Bluetooth
    Dabble.processInput();
    
    // Verifica se os botões do Gamepad foram pressionados:
    
    // Botão UP → Toca música aleatória da pasta 2 (BB8 bravo)
    if (GamePad.isUpPressed()) {
        playRandomFromFolder(2);
        delay(300);  // Debounce (evita leituras múltiplas)
    }
    
    // Botão RIGHT → Toca música aleatória da pasta 3 (BB8 feliz)
    if (GamePad.isRightPressed()) {
        playRandomFromFolder(3);
        delay(300);
    }
    
    // Botão DOWN → Toca música aleatória da pasta 4 (raitecos)
    if (GamePad.isDownPressed()) {
        playRandomFromFolder(4);
        delay(300);
    }
    
    // Botão TRIÂNGULO → Aumenta volume
    if (GamePad.isTrianglePressed()) {
        player.volumeUp();
        Serial.println("Volume aumentado");
        delay(300);
    }
    
    // Botão X (CRUZ) → Diminui volume
    if (GamePad.isCrossPressed()) {
        player.volumeDown();
        Serial.println("Volume diminuído");
        delay(300);
    }
    
    // Botão CÍRCULO → Pausa a reprodução
    if (GamePad.isCirclePressed()) {
        player.pause();
        Serial.println("Pausado");
        delay(300);
    }
    
    // Botão QUADRADO → Retoma a reprodução
    if (GamePad.isSquarePressed()) {
        player.start();
        Serial.println("Retomando reprodução");
        delay(300);
    }
    
    delay(20); 
}

// Função para tocar uma música aleatória de uma pasta específica
void playRandomFromFolder(uint8_t folder) {
    // Gera um número aleatório entre 1 e 10 (não lembro quantas músicas tem)
    uint8_t randomTrack = random(1, 11);
    
    // Exibe no Serial Monitor qual música está tocando
    Serial.print("Tocando música aleatória da pasta ");
    Serial.print(folder);
    Serial.print(", faixa ");
    Serial.println(randomTrack);
    
    // Comanda o DFPlayer tocar a faixa selecionada
    player.playFolder(folder, randomTrack);
}
