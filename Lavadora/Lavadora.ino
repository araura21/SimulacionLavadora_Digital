
#include "SevSeg.h"

SevSeg sevseg1; 
SevSeg sevseg2; 

const int botonEncenderApagar = PC13;
const int botonIniciarPausar = PC14;
const int ledEncendido = PB2; 
const int ledVerde = PB12;     
const int ledRojo = PB13;
const int botonSeleccionar = PB3;


const int led18kg = PB8;
const int led12kg = PB9;
const int led7kg = PB10;

int horas = 0, minutos = 0, segundos = 0;

bool displayEncendido = false;
bool enMarcha = false;

bool prevEstadoEncender = HIGH;
bool prevEstadoIniciar = HIGH;
bool prevEstadoSeleccionar = HIGH;

unsigned long lastUpdateTime = 0;
unsigned long lastDebounceTimeEncender = 0;
unsigned long lastDebounceTimeIniciar = 0;
unsigned long lastDebounceTimeSeleccionar = 0; 
const unsigned long debounceDelay = 50;

int cantidadSeleccionada = 0;
void setup() {
    byte numDigits1 = 4;
    byte digitPins1[] = {PC9, PC10, PC11, PC12};
    byte segmentPins1[] = {PC0, PC1, PC2, PC3, PC5, PC6, PC7, PC8};

    byte numDigits2 = 4;
    byte digitPins2[] = {PB6, PB7, PB0, PB1};
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
    pinMode(botonSeleccionar, INPUT_PULLUP); // Configuración del botón de seleccionar

    pinMode(led18kg, OUTPUT);
    pinMode(led12kg, OUTPUT);
    pinMode(led7kg, OUTPUT);

    digitalWrite(ledEncendido, LOW); 
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, LOW);
    digitalWrite(led18kg, LOW); // LEDs apagados por defecto
    digitalWrite(led12kg, LOW);
    digitalWrite(led7kg, LOW);

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

        digitalWrite(PB6, LOW);  
        digitalWrite(PB7, LOW); 
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

    // Botón Iniciar/Pausar
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

    // Botón Seleccionar cantidad solo si está en pausa
    if (!enMarcha) { // Solo permitir cambiar la cantidad si está en pausa
        bool estadoSeleccionar = digitalRead(botonSeleccionar);
        if (estadoSeleccionar != prevEstadoSeleccionar && (millis() - lastDebounceTimeSeleccionar > debounceDelay)) {
            if (estadoSeleccionar == LOW) {
                cantidadSeleccionada++; // Aumentar la cantidad seleccionada
                if (cantidadSeleccionada > 3) cantidadSeleccionada = 1; // Ciclar la selección entre 1 y 3

                // Apagar todos los LEDs primero
                digitalWrite(led18kg, LOW);
                digitalWrite(led12kg, LOW);
                digitalWrite(led7kg, LOW);

                // Encender el LED correspondiente según la cantidad seleccionada y establecer el tiempo
                switch (cantidadSeleccionada) {
                    case 1:
                        digitalWrite(led7kg, HIGH);
                        horas = 0;
                        minutos = 1; // 30 minutos para 7 kg
                        break;
                    case 2:
                        digitalWrite(led12kg, HIGH);
                        horas = 0;
                        minutos = 2; // 1 hora para 12 kg
                        break;
                    case 3:
                        digitalWrite(led18kg, HIGH);
                        horas = 0;
                        minutos = 3; // 1 hora y 30 minutos para 18 kg
                        break;
                }
            }
            lastDebounceTimeSeleccionar = millis();
        }
        prevEstadoSeleccionar = estadoSeleccionar;
    }
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

