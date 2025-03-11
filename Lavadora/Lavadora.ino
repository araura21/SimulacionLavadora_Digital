#include "SevSeg.h"

SevSeg sevseg1;
SevSeg sevseg2;

const int botonEncenderApagar = PC13;
const int botonIniciarPausar = PC14;
const int botonCantidadRopa = PB3;
const int botonSeleccionarLavado = PB14;

const int ledEncendido = PB2;
const int ledVerde = PB12;
const int ledRojo = PB13;
const int led18kg = PB8;
const int led12kg = PB9;
const int led7kg = PB10;
const int ledLavNormal = PB4;
const int ledLavRapido = PB5;
const int ledLavFuerte = PB11;

int horas = 0, minutos = 0, segundos = 0;
bool sistemaEncendido = false; // Cambié el nombre de displayEncendido a sistemaEncendido
bool enMarcha = false;
int cantidadSeleccionada = 0;
int tipoLavadoSeleccionado = 0;

bool prevEstadoEncender = HIGH;
bool prevEstadoIniciar = HIGH;
bool prevEstadoSeleccionarRopa = HIGH;
bool prevEstadoSeleccionarLavado = HIGH;

unsigned long lastUpdateTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
    configurarDisplay();
    configurarBotones();
    configurarLEDs();
    sevseg1.blank();
    sevseg2.blank();
}

void loop() {
    manejarBotonEncender(); // Solo se manejará este botón si el sistema está apagado

    if (sistemaEncendido) {
        manejarBotones();
        actualizarTemporizador();
        mostrarTiempo();
    } else {
        apagarTodo(); // Si el sistema está apagado, asegurarse de que todo esté apagado
    }
}

void configurarDisplay() {
    byte digitPins1[] = {PC9, PC10, PC11, PC12};
    byte segmentPins1[] = {PC0, PC1, PC2, PC3, PC5, PC6, PC7, PC8};
    sevseg1.begin(COMMON_ANODE, 4, digitPins1, segmentPins1, true, false, false, false);

    byte digitPins2[] = {PB6, PB7, PB0, PB1};
    byte segmentPins2[] = {PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9};
    sevseg2.begin(COMMON_ANODE, 4, digitPins2, segmentPins2, true, false, false, false);

    sevseg1.setBrightness(90);
    sevseg2.setBrightness(90);
}

void configurarBotones() {
    pinMode(botonEncenderApagar, INPUT_PULLUP);
    pinMode(botonIniciarPausar, INPUT_PULLUP);
    pinMode(botonCantidadRopa, INPUT_PULLUP);
    pinMode(botonSeleccionarLavado, INPUT_PULLUP);
}

void configurarLEDs() {
    int leds[] = {ledEncendido, ledVerde, ledRojo, led18kg, led12kg, led7kg, ledLavNormal, ledLavRapido, ledLavFuerte};
    for (int i = 0; i < 9; i++) {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW); // Asegurar que todos los LEDs están apagados al iniciar
    }
}

bool debounce(int pin, bool &prevEstado, unsigned long &lastDebounceTime) {
    bool estado = digitalRead(pin);
    if (estado != prevEstado && (millis() - lastDebounceTime > debounceDelay)) {
        lastDebounceTime = millis();
        prevEstado = estado;
        return estado == LOW;
    }
    return false;
}

// Maneja solo el botón de encendido
void manejarBotonEncender() {
    if (debounce(botonEncenderApagar, prevEstadoEncender, lastUpdateTime)) {
        sistemaEncendido = !sistemaEncendido;
        digitalWrite(ledEncendido, sistemaEncendido ? HIGH : LOW);

        if (!sistemaEncendido) {
            apagarTodo();
        }
    }
}

// Apagar todo si el sistema está apagado
void apagarTodo() {
    enMarcha = false;
    cantidadSeleccionada = 0;
    tipoLavadoSeleccionado = 0;
    horas = minutos = segundos = 0;

    sevseg1.blank();
    sevseg2.blank();

    int leds[] = {ledEncendido, ledVerde, ledRojo, led18kg, led12kg, led7kg, ledLavNormal, ledLavRapido, ledLavFuerte};
    for (int i = 0; i < 9; i++) {
        digitalWrite(leds[i], LOW);
    }
}

void manejarBotones() {
    if (debounce(botonIniciarPausar, prevEstadoIniciar, lastUpdateTime)) {
        enMarcha = !enMarcha;
        digitalWrite(ledVerde, enMarcha ? HIGH : LOW);
        digitalWrite(ledRojo, enMarcha ? LOW : HIGH);
        lastUpdateTime = millis();
    }

    if (!enMarcha && debounce(botonCantidadRopa, prevEstadoSeleccionarRopa, lastUpdateTime)) {
        cantidadSeleccionada = (cantidadSeleccionada % 3) + 1;
        configurarCantidadRopa(cantidadSeleccionada);
    }

    if (!enMarcha && debounce(botonSeleccionarLavado, prevEstadoSeleccionarLavado, lastUpdateTime)) {
        tipoLavadoSeleccionado = (tipoLavadoSeleccionado % 3) + 1;
        configurarTipoLavado(tipoLavadoSeleccionado);
    }
}

void configurarCantidadRopa(int cantidad) {
    digitalWrite(led18kg, LOW);
    digitalWrite(led12kg, LOW);
    digitalWrite(led7kg, LOW);

    switch (cantidad) {
        case 1: digitalWrite(led7kg, HIGH); minutos = 1; break;
        case 2: digitalWrite(led12kg, HIGH); minutos = 2; break;
        case 3: digitalWrite(led18kg, HIGH); minutos = 3; break;
        default:
            digitalWrite(led7kg, LOW);
            digitalWrite(led12kg, LOW);
            digitalWrite(led18kg, LOW);
            minutos = 0;
            segundos = 0;
            break;
    }
}

void configurarTipoLavado(int tipo) {
    digitalWrite(ledLavNormal, LOW);
    digitalWrite(ledLavRapido, LOW);
    digitalWrite(ledLavFuerte, LOW);

    switch (tipo) {
        case 1: digitalWrite(ledLavFuerte, HIGH); minutos = 40; segundos = 0; break;
        case 2: digitalWrite(ledLavRapido, HIGH); minutos = 0; segundos = 50; break;
        case 3: digitalWrite(ledLavNormal, HIGH); minutos = 5; segundos = 0; break;
        default:
            minutos = 0;
            segundos = 0;
            break;
    }
}

void actualizarTemporizador() {
    if (enMarcha && millis() - lastUpdateTime >= 1000) {
        lastUpdateTime += 1000;
        if (--segundos < 0) {
            segundos = 59;
            if (--minutos < 0) {
                minutos = 59;
                if (--horas < 0) {
                    horas = minutos = segundos = 0;
                    enMarcha = false;
                    digitalWrite(ledVerde, LOW);
                    digitalWrite(ledRojo, LOW);
                    sevseg1.blank();
                    sevseg2.blank();
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

