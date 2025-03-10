#include "SevSeg.h"

SevSeg sevseg1; // Primer display (Horas y Minutos)
SevSeg sevseg2; // Segundo display (Segundos)

const int botonEncenderApagar = PC13;
const int botonIniciarPausar = PC14;

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
    // Configuración para el primer display (horas y minutos)
    byte numDigits1 = 4;
    byte digitPins1[] = {PC9, PC10, PC11, PC12};
    byte segmentPins1[] = {PC0, PC1, PC2, PC3, PC5, PC6, PC7, PC8};

    // Configuración para el segundo display (segundos)
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

    // Mostrar la configuración inicial al encender
    int tiempoHM = (horas * 100) + minutos;
    sevseg1.setNumber(tiempoHM, 2);
    sevseg2.setNumber(segundos, 2);
    sevseg1.refreshDisplay();
    sevseg2.refreshDisplay();
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
            if (!displayEncendido) {
                horas = minutos = segundos = 0;
                sevseg1.blank();
                sevseg2.blank();
            } else {
                int tiempoHM = (horas * 100) + minutos;
                sevseg1.setNumber(tiempoHM, 2);
                sevseg2.setNumber(segundos, 2);
                sevseg1.refreshDisplay();
                sevseg2.refreshDisplay();
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
                    enMarcha = false; // Detiene el conteo cuando llega a 00:00:00
                }
            }
        }
    }
}

void mostrarTiempo() {
    int tiempoHM = (horas * 100) + minutos;
    sevseg1.setNumber(tiempoHM, 2);
    sevseg2.setNumber(segundos, 2);
    sevseg1.refreshDisplay();
    sevseg2.refreshDisplay();
}
