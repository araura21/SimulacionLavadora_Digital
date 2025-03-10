
#include "SevSeg.h"

SevSeg sevseg1; 
SevSeg sevseg2; 

const int botonEncenderApagar = PC13;
const int botonIniciarPausar = PC14;
const int ledEncendido = PB2; 
const int ledVerde = PB12;     
const int ledRojo = PB13;
     

int horas = 4, minutos = 4, segundos = 0;

bool displayEncendido = false;
bool enMarcha = false;

bool prevEstadoEncender = HIGH;
bool prevEstadoIniciar = HIGH;

unsigned long lastUpdateTime = 0;
unsigned long lastDebounceTimeEncender = 0;
unsigned long lastDebounceTimeIniciar = 0;
const unsigned long debounceDelay = 50;

void setup() {
    
    byte numDigits1 = 4;
    byte digitPins1[] = {PC9, PC10, PC11, PC12};
    byte segmentPins1[] = {PC0, PC1, PC2, PC3, PC5, PC6, PC7, PC8};

   
    byte numDigits2 = 2;
    byte digitPins2[] = {PB0, PB1};
    byte segmentPins2[] = {PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9};

    bool resistorsOnSegments = true;
    byte hardwareConfig = COMMON_ANODE;
    bool updateWithDelays = false;
    bool leadingZeros = false;
    bool disableDecPoint = false;

    sevseg1.begin(hardwareConfig, numDigits1, digitPins1, segmentPins1,
                  resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
    sevseg2.begin(hardwareConfig, numDigits2, digitPins2, segmentPins2,
                  resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);

    sevseg1.setBrightness(90);
    sevseg2.setBrightness(90);

    pinMode(botonEncenderApagar, INPUT_PULLUP);
    pinMode(botonIniciarPausar, INPUT_PULLUP);
    pinMode(ledEncendido, OUTPUT); 
    pinMode(ledVerde, OUTPUT);     
    pinMode(ledRojo, OUTPUT);     

    digitalWrite(ledEncendido, LOW); 
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, LOW);

  
    sevseg1.setNumber(horas * 100 + minutos, 2);
    sevseg2.setNumber(segundos, 2);
    sevseg1.refreshDisplay();
    sevseg2.refreshDisplay();
    sevseg1.blank();
    sevseg2.blank();
}

void loop() {
    manejarBotones();
    if (displayEncendido) {
        actualizarTemporizador();
        mostrarTiempo();
    }
}

void manejarBotones() {
    // Botón Encender/Apagar
    bool estadoEncender = digitalRead(botonEncenderApagar);
    if (estadoEncender != prevEstadoEncender && (millis() - lastDebounceTimeEncender > debounceDelay)) {
        if (estadoEncender == LOW) {
            displayEncendido = !displayEncendido;
            enMarcha = false;
            
            if (displayEncendido) {
                digitalWrite(ledEncendido, HIGH); 
                digitalWrite(ledVerde, LOW);     
                digitalWrite(ledRojo, LOW);   
                sevseg1.setNumber(horas * 100 + minutos, 2);
                sevseg2.setNumber(segundos, 2);
                sevseg1.refreshDisplay();
                sevseg2.refreshDisplay();
            } else {
                digitalWrite(ledEncendido, LOW); 
                digitalWrite(ledVerde, LOW);
                digitalWrite(ledRojo, LOW);
                horas = minutos = segundos = 0;
                sevseg1.blank();
                sevseg2.blank();
            }
        }
        lastDebounceTimeEncender = millis();
    }
    prevEstadoEncender = estadoEncender;

 
    bool estadoIniciarPausar = digitalRead(botonIniciarPausar);
    if (estadoIniciarPausar != prevEstadoIniciar && (millis() - lastDebounceTimeIniciar > debounceDelay)) {
        if (estadoIniciarPausar == LOW) {
            enMarcha = !enMarcha;
            lastUpdateTime = millis();

            if (enMarcha) {
                digitalWrite(ledVerde, HIGH); 
                digitalWrite(ledRojo, LOW);  
            } else {
                digitalWrite(ledVerde, LOW); 
                digitalWrite(ledRojo, HIGH); 
            }
        }
        lastDebounceTimeIniciar = millis();
    }
    prevEstadoIniciar = estadoIniciarPausar;
}

void actualizarTemporizador() {
    if (enMarcha && millis() - lastUpdateTime >= 1000) {
        lastUpdateTime += 1000;
        segundos--;
        if (segundos < 0) {
            segundos = 59;
            minutos--;
            if (minutos < 0) {
                minutos = 59;
                horas--;
                if (horas < 0) {
                    horas = 0;
                    minutos = 0;
                    segundos = 0;
                    enMarcha = false; 
                    digitalWrite(ledVerde, LOW);
                    digitalWrite(ledRojo, LOW);
                }
            }
        }
    }
}

void mostrarTiempo() {
    sevseg1.setNumber(horas * 100 + minutos, 2);
    sevseg2.setNumber(segundos, 2);
    sevseg1.refreshDisplay();
    sevseg2.refreshDisplay();
}

