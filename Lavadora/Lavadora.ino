
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

// Variables de estado para los botones de selección
bool prevEstadoNivelAgua = HIGH;  // Estado previo para el botón de selección de Nivel de Agua
bool prevEstadoEnjuague = HIGH;   // Estado previo para el botón de selección de Enjuague
bool prevEstadoCentrifugado = HIGH; // Estado previo para el botón de selección de Centrifugado
bool prevEstadoLavado = HIGH;     // Estado previo para el botón de selección de Lavado

unsigned long lastDebounceTimeNivelAgua = 0;  // Tiempo de debounce para Nivel de Agua
unsigned long lastDebounceTimeEnjuague = 0;   // Tiempo de debounce para Enjuague
unsigned long lastDebounceTimeCentrifugado = 0; // Tiempo de debounce para Centrifugado
unsigned long lastDebounceTimeLavado = 0;    // Tiempo de debounce para Lavado

// Botón Selección Nivel de Agua
const int botonNivelAgua = PA1; 
const int ledBajoAgua = PA10;   
const int ledMedioAgua = PA11;  
const int ledAltoAgua = PA12;    

int nivelAguaSeleccionado = 0; 

// Botón Selección Enjuague
const int botonEnjuague = PB11;    
const int ledEnjuague1 = PB4;    
const int ledEnjuague2 = PB5;   
const int ledEnjuague3 = PA0;    

int enjuagueSeleccionado = 0;  

// Botón Selección Centrifugado
const int botonCentrifugado = PB14; 
const int ledCentrifugado1 = PB15; 
const int ledCentrifugado2 = PC15; 
const int ledCentrifugado3 = PB6; 

int centrifugadoSeleccionado = 0;

/*
// Botón Selección Lavado
const int botonLavado = ;       
const int ledLavado1 = ;       
const int ledLavado2 = ;        
const int ledLavado3 = ;       

int lavadoSeleccionado = 0;     
*/

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

     // Botones y LEDs para Nivel de Agua
    pinMode(botonNivelAgua, INPUT_PULLUP);  // Define el pin del botón de nivel de agua
    pinMode(ledBajoAgua, OUTPUT);           // Define el pin para LED bajo nivel de agua
    pinMode(ledMedioAgua, OUTPUT);          // Define el pin para LED medio nivel de agua
    pinMode(ledAltoAgua, OUTPUT);           // Define el pin para LED alto nivel de agua

    // Botones y LEDs para Enjuague
    pinMode(botonEnjuague, INPUT_PULLUP);   // Define el pin del botón de enjuague
    pinMode(ledEnjuague1, OUTPUT);          // Define el pin para LED enjuague 1
    pinMode(ledEnjuague2, OUTPUT);          // Define el pin para LED enjuague 2
    pinMode(ledEnjuague3, OUTPUT);          // Define el pin para LED enjuague 3

    // Botones y LEDs para Centrifugado
    pinMode(botonCentrifugado, INPUT_PULLUP); // Define el pin del botón de centrifugado
    pinMode(ledCentrifugado1, OUTPUT);        // Define el pin para LED centrifugado 1
    pinMode(ledCentrifugado2, OUTPUT);        // Define el pin para LED centrifugado 2
    pinMode(ledCentrifugado3, OUTPUT);        // Define el pin para LED centrifugado 3

    /*
    // Botones y LEDs para Lavado
    pinMode(botonLavado, INPUT_PULLUP);       // Define el pin del botón de lavado
    pinMode(ledLavado1, OUTPUT);              // Define el pin para LED lavado 1
    pinMode(ledLavado2, OUTPUT);              // Define el pin para LED lavado 2
    pinMode(ledLavado3, OUTPUT);              // Define el pin para LED lavado 3
  */

    digitalWrite(ledBajoAgua, LOW);
    digitalWrite(ledMedioAgua, LOW);
    digitalWrite(ledAltoAgua, LOW);
    digitalWrite(ledEnjuague1, LOW);
    digitalWrite(ledEnjuague2, LOW);
    digitalWrite(ledEnjuague3, LOW);
    digitalWrite(ledCentrifugado1, LOW);
    digitalWrite(ledCentrifugado2, LOW);
    digitalWrite(ledCentrifugado3, LOW);
    /*
    digitalWrite(ledLavado1, LOW);
    digitalWrite(ledLavado2, LOW);
    digitalWrite(ledLavado3, LOW);
    */
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

        // Botón Selección Nivel de Agua
        bool estadoNivelAgua = digitalRead(botonNivelAgua);
        if (estadoNivelAgua != prevEstadoNivelAgua && (millis() - lastDebounceTimeNivelAgua > debounceDelay)) {
            if (estadoNivelAgua == LOW) {
                nivelAguaSeleccionado++; // Aumentar la cantidad seleccionada
                if (nivelAguaSeleccionado > 3) nivelAguaSeleccionado = 1; // Ciclar la selección entre 1 y 3

                // Apagar todos los LEDs primero
                digitalWrite(ledBajoAgua, LOW);
                digitalWrite(ledMedioAgua, LOW);
                digitalWrite(ledAltoAgua, LOW);

                // Encender el LED correspondiente según la selección
                switch (nivelAguaSeleccionado) {
                    case 1:
                        digitalWrite(ledBajoAgua, HIGH);
                        minutos=1;
                        break;
                    case 2:
                        digitalWrite(ledMedioAgua, HIGH);
                        minutos=2;
                        break;
                    case 3:
                        digitalWrite(ledAltoAgua, HIGH);
                        minutos=3;
                        break;
                }
            }
            lastDebounceTimeNivelAgua = millis();
        }
        prevEstadoNivelAgua = estadoNivelAgua;

        // Botón Selección Enjuague
        bool estadoEnjuague = digitalRead(botonEnjuague);
        if (estadoEnjuague != prevEstadoEnjuague && (millis() - lastDebounceTimeEnjuague > debounceDelay)) {
            if (estadoEnjuague == LOW) {
                enjuagueSeleccionado++; // Aumentar la selección de enjuague
                if (enjuagueSeleccionado > 3) enjuagueSeleccionado = 1; // Ciclar la selección entre 1 y 3

                // Apagar todos los LEDs primero
                digitalWrite(ledEnjuague1, LOW);
                digitalWrite(ledEnjuague2, LOW);
                digitalWrite(ledEnjuague3, LOW);

                // Encender el LED correspondiente según la selección
                switch (enjuagueSeleccionado) {
                    case 1:
                        digitalWrite(ledEnjuague1, HIGH);
                        segundos=30;
                        break;
                    case 2:
                        digitalWrite(ledEnjuague2, HIGH);
                        minutos=1;
                        break;
                    case 3:
                        digitalWrite(ledEnjuague3, HIGH);
                        minutos=2;
                        break;
                }
            }
            lastDebounceTimeEnjuague = millis();
        }
        prevEstadoEnjuague = estadoEnjuague;

        // Botón Selección Centrifugado
        bool estadoCentrifugado = digitalRead(botonCentrifugado);
        if (estadoCentrifugado != prevEstadoCentrifugado && (millis() - lastDebounceTimeCentrifugado > debounceDelay)) {
            if (estadoCentrifugado == LOW) {
                centrifugadoSeleccionado++; // Aumentar la selección de centrifugado
                if (centrifugadoSeleccionado > 3) centrifugadoSeleccionado = 1; // Ciclar la selección entre 1 y 3

                // Apagar todos los LEDs primero
                digitalWrite(ledCentrifugado1, LOW);
                digitalWrite(ledCentrifugado2, LOW);
                digitalWrite(ledCentrifugado3, LOW);

                // Encender el LED correspondiente según la selección
                switch (centrifugadoSeleccionado) {
                    case 1:
                        digitalWrite(ledCentrifugado1, HIGH);
                        segundos=30;
                        break;
                    case 2:
                        digitalWrite(ledCentrifugado2, HIGH);
                        minutos=1;
                        break;
                    case 3:
                        digitalWrite(ledCentrifugado3, HIGH);
                        minutos=2;
                        break;
                }
            }
            lastDebounceTimeCentrifugado = millis();
        }
        prevEstadoCentrifugado = estadoCentrifugado;
        /*
        // Botón Selección Lavado
        bool estadoLavado = digitalRead(botonLavado);
        if (estadoLavado != prevEstadoLavado && (millis() - lastDebounceTimeLavado > debounceDelay)) {
            if (estadoLavado == LOW) {
                lavadoSeleccionado++; // Aumentar la selección de lavado
                if (lavadoSeleccionado > 3) lavadoSeleccionado = 1; // Ciclar la selección entre 1 y 3

                // Apagar todos los LEDs primero
                digitalWrite(ledLavado1, LOW);
                digitalWrite(ledLavado2, LOW);
                digitalWrite(ledLavado3, LOW);

                // Encender el LED correspondiente según la selección
                switch (lavadoSeleccionado) {
                    case 1:
                        digitalWrite(ledLavado1, HIGH);
                        segundos=30;
                        break;
                    case 2:
                        digitalWrite(ledLavado2, HIGH);
                        minutos=1;
                        break;
                    case 3:
                        digitalWrite(ledLavado3, HIGH);
                        minutos=2;
                        break;
                }
            }
            lastDebounceTimeLavado = millis();
        }
        prevEstadoLavado = estadoLavado;
        */


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

