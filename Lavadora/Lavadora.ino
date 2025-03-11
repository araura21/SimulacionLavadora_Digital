#include "SevSeg.h"

SevSeg sevseg1;
//SevSeg sevseg2;

//Encender/apagar
const int botonEncenderApagar = PC13;
const int ledEncendido = PB0;

//Iniciar/Pausar
const int botonIniciarPausar = PC14;
const int ledVerde = PB2;
const int ledRojo = PB1;

//Tipo de lavado
const int botonSeleccionarLavado = PC15;
const int ledLavFuerte = PB3;
const int ledLavRapido = PB4;
const int ledLavNormal = PB5;

int tipoLavadoSeleccionado = 0;

//Cantidad de ropa
const int botonCantidadRopa = PB14;
const int led18kg = PB6;
const int led12kg = PB7;
const int led7kg = PB8;

int cantidadSeleccionada = 0;

//Temperatura
const int botonTemperatura = PB15;
const int ledFrio = PB9;
const int ledCaliente = PB10;

//Nivel de Agua
const int botonNivelAgua = PD2; 
const int ledBajoAgua = PB13;   
const int ledMedioAgua = PB12;  
const int ledAltoAgua = PA11;    

int nivelAguaSeleccionado = 0; 

//Lavado
const int botonLavado = PA1;
const int ledLav5 = PA4;
const int ledLav10 = PA3;
const int ledLav20 = PA2;

int tiempoLavadoSeleccionado = 0;

//Enjuague
const int botonEnjuague = PA5;    
const int ledEnjuague1 = PA8;    
const int ledEnjuague2 = PA7;   
const int ledEnjuague3 = PA6;    

int enjuagueSeleccionado = 0;  

//Centrifugado
const int botonCentrifugado = PA9; 
const int ledCentrifugadoBajo = PA10; 
const int ledCentrifugadoMedio = PA11; 
const int ledCentrifugadoAlto = PA12; 

int centrifugadoSeleccionado = 0;

int horas = 0, minutos = 0, segundos = 0;

bool sistemaEncendido = false; 
bool enMarcha = false;


//variables de estado para botones de seleccion
bool prevEstadoEncender = HIGH;
bool prevEstadoIniciar = HIGH;
bool prevEstadoTipoLavado = HIGH;
bool prevEstadoRopa = HIGH;
bool prevEstadoTemperatura = HIGH;
bool prevEstadoNivelAgua = HIGH;  
bool prevEstadoLavado = HIGH;
bool prevEstadoEnjuague = HIGH;
bool prevEstadoCentrifugado = HIGH;

unsigned long lastDebounceTimeNivelAgua = 0;  
unsigned long lastDebounceTimeEnjuague = 0;   
unsigned long lastDebounceTimeCentrifugado = 0; 
unsigned long lastDebounceTimeLavado = 0; 
const unsigned long debounceDelay = 50;

unsigned long lastUpdateTime = 0;

void setup() {
    byte numDigits1 = 4;
    byte digitPins1[] = {PC9, PC10, PC11, PC12};
    byte segmentPins1[] = {PC0, PC1, PC2, PC3, PC5, PC6, PC7, PC8};

/*    byte numDigits2 = 4;
    byte digitPins2[] = {PA9, PA10, PA11, PA12};
    byte segmentPins2[] = {PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8};*/

    bool resistorsOnSegments = true;
    byte hardwareConfig = COMMON_ANODE;
    bool updateWithDelays = false;
    bool leadingZeros = false;
    bool disableDecPoint = false;

    sevseg1.begin(hardwareConfig, numDigits1, digitPins1, segmentPins1,
                  resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
    /*sevseg2.begin(hardwareConfig, numDigits2, digitPins2, segmentPins2,
                  resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);*/

    sevseg1.setBrightness(90);
    //sevseg2.setBrightness(90);

    //Encender/Apagar
    pinMode(botonEncenderApagar, INPUT_PULLUP);
    pinMode(ledEncendido, OUTPUT);

    //Iniciar/Pausar
    pinMode(botonIniciarPausar, INPUT_PULLUP);
    pinMode(ledVerde, OUTPUT);     
    pinMode(ledRojo, OUTPUT);    

    //Tipo de Lavado
    pinMode(botonSeleccionarLavado, INPUT_PULLUP);
    pinMode(ledLavNormal, OUTPUT);
    pinMode(ledLavRapido, OUTPUT);
    pinMode(ledLavFuerte, OUTPUT);

    //Cantidad de ropa
    pinMode(botonCantidadRopa, INPUT_PULLUP);
    pinMode(led18kg, OUTPUT);
    pinMode(led12kg, OUTPUT);
    pinMode(led7kg, OUTPUT);

    //Temperatura
    pinMode(botonTemperatura, INPUT_PULLUP);
    pinMode(ledFrio, OUTPUT);
    pinMode(ledCaliente, OUTPUT);

    //Nivel de Agua
    pinMode(botonNivelAgua, INPUT_PULLUP);  
    pinMode(ledBajoAgua, OUTPUT);           
    pinMode(ledMedioAgua, OUTPUT);          
    pinMode(ledAltoAgua, OUTPUT);

    // Botones y LEDs para Lavado
    pinMode(botonLavado, INPUT_PULLUP);       
    pinMode(ledLav5, OUTPUT);              
    pinMode(ledLav10, OUTPUT);              
    pinMode(ledLav20, OUTPUT);              
  
    //Enjuague
    pinMode(botonEnjuague, INPUT_PULLUP);   
    pinMode(ledEnjuague1, OUTPUT);          
    pinMode(ledEnjuague2, OUTPUT);          
    pinMode(ledEnjuague3, OUTPUT); 

    //Centrifugado
    pinMode(botonCentrifugado, INPUT_PULLUP); 
    pinMode(ledCentrifugadoBajo, OUTPUT);        
    pinMode(ledCentrifugadoMedio, OUTPUT);        
    pinMode(ledCentrifugadoAlto, OUTPUT);

    //definicion de leds
    digitalWrite(ledEncendido, LOW); 
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, LOW);

    digitalWrite(led18kg, LOW); 
    digitalWrite(led12kg, LOW);
    digitalWrite(led7kg, LOW);

    digitalWrite(ledLavNormal, LOW);
    digitalWrite(ledLavRapido, LOW);
    digitalWrite(ledLavFuerte, LOW);

    digitalWrite(ledFrio, LOW);
    digitalWrite(ledCaliente, LOW);

    digitalWrite(ledBajoAgua, LOW);
    digitalWrite(ledMedioAgua, LOW);
    digitalWrite(ledAltoAgua, LOW);

    digitalWrite(ledLav5, LOW);
    digitalWrite(ledLav10, LOW);
    digitalWrite(ledLav20, LOW);

    digitalWrite(ledEnjuague1, LOW);
    digitalWrite(ledEnjuague2, LOW);
    digitalWrite(ledEnjuague3, LOW);

    digitalWrite(ledCentrifugadoBajo, LOW);
    digitalWrite(ledCentrifugadoMedio, LOW);
    digitalWrite(ledCentrifugadoAlto, LOW);
   
   //setup de display
    sevseg1.setNumber(horas * 100 + minutos, 2);
    //sevseg2.setNumber(segundos, 2);
    sevseg1.refreshDisplay();
    //sevseg2.refreshDisplay();
    sevseg1.blank();
    //sevseg2.blank();
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
    //sevseg2.blank();

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

    if (!enMarcha && debounce(botonCantidadRopa, prevEstadoRopa, lastUpdateTime)) {
        cantidadSeleccionada = (cantidadSeleccionada % 4) + 1;
        configurarCantidadRopa(cantidadSeleccionada);
    }

    if (!enMarcha && debounce(botonSeleccionarLavado, prevEstadoTipoLavado, lastUpdateTime)) {
        tipoLavadoSeleccionado = (tipoLavadoSeleccionado % 4) + 1;
        configurarTipoLavado(tipoLavadoSeleccionado);
    }
}

void configurarTipoLavado(int tipo) {
    digitalWrite(ledLavNormal, LOW);
    digitalWrite(ledLavRapido, LOW);
    digitalWrite(ledLavFuerte, LOW);

    digitalWrite(ledLav10, LOW);
    digitalWrite(ledEnjuague2, LOW);
    digitalWrite(ledCentrifugadoMedio, LOW);
    digitalWrite(ledLav5, LOW);
    digitalWrite(ledEnjuague1, LOW);
    digitalWrite(ledCentrifugadoBajo, LOW); 
    digitalWrite(ledLav20, LOW);
    digitalWrite(ledEnjuague3, LOW);
    digitalWrite(ledCentrifugadoAlto, LOW); 

    switch (tipo) {
        case 1: 
            digitalWrite(ledLavFuerte, HIGH); minutos = 40; segundos = 0; 
            digitalWrite(ledLav20, HIGH);
            digitalWrite(ledEnjuague3, HIGH);
            digitalWrite(ledCentrifugadoAlto, HIGH);
            break;
        case 2: 
            digitalWrite(ledLavRapido, HIGH); minutos = 1; segundos = 0;
            digitalWrite(ledLav5, HIGH);
            digitalWrite(ledEnjuague1, HIGH);
            digitalWrite(ledCentrifugadoBajo, HIGH); 
            break;
        case 3: 
            digitalWrite(ledLavNormal, HIGH); minutos = 5; segundos = 0;
            digitalWrite(ledLav10, HIGH);
            digitalWrite(ledEnjuague2, HIGH);
            digitalWrite(ledCentrifugadoMedio, HIGH); 
            break;
        default:
            minutos = 0;
            segundos = 0;
            break;
  }
}

void configurarCantidadRopa(int cantidad) {
    digitalWrite(led18kg, LOW);
    digitalWrite(led12kg, LOW);
    digitalWrite(led7kg, LOW);

    switch (cantidadSeleccionada) {
      case 1: digitalWrite(led7kg, HIGH); horas = 0; minutos = 1; 
            break;
      case 2: digitalWrite(led12kg, HIGH); horas = 0; minutos = 2; 
            break;
      case 3: digitalWrite(led18kg, HIGH); horas = 0; minutos = 3; 
            break;
      default:
            horas = 0;
            minutos = 0;
            break;
  }
}
/*  
// Botón Selección Nivel de Agua
bool estadoNivelAgua = digitalRead(botonNivelAgua);
  if(estadoNivelAgua != prevEstadoNivelAgua && (millis() - lastDebounceTimeNivelAgua > debounceDelay)) {
    if (estadoNivelAgua == LOW) {
      nivelAguaSeleccionado++; // Aumentar la cantidad seleccionada
      if (nivelAguaSeleccionado > 3) nivelAguaSeleccionado = 1; // Ciclar la selección entre 1 y 3
      // Apagar todos los LEDs primero
          digitalWrite(ledBajoAgua, LOW);
          digitalWrite(ledMedioAgua, LOW);
          digitalWrite(ledAltoAgua, LOW);
      // Encender el LED correspondiente según la selección
          switch (nivelAguaSeleccionado) {
            case 1: digitalWrite(ledBajoAgua, HIGH); minutos=1;
                break;
            case 2: digitalWrite(ledMedioAgua, HIGH); minutos=2;
                break;
            case 3: digitalWrite(ledAltoAgua, HIGH); minutos=3;
                break;
              }
            }
            lastDebounceTimeNivelAgua = millis();
        }
        prevEstadoNivelAgua = estadoNivelAgua;*/
/*
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
          case 1: digitalWrite(ledEnjuague1, HIGH); segundos=30;
              break;
          case 2: digitalWrite(ledEnjuague2, HIGH); minutos=1;
              break;
          case 3: digitalWrite(ledEnjuague3, HIGH); minutos=2;
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
          case 1: digitalWrite(ledCentrifugado1, HIGH); segundos=30;
              break;
          case 2: digitalWrite(ledCentrifugado2, HIGH); minutos=1;
              break;
          case 3: digitalWrite(ledCentrifugado3, HIGH); minutos=2;
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
                    sevseg1.setChars("End");
                    sevseg1.refreshDisplay();
                    return;
                    //sevseg2.blank();
                }
            }
            mostrarTiempo();
        }
    }
}

void mostrarTiempo() {
    sevseg1.setNumber(horas * 100 + minutos, 2);
    //sevseg2.setNumber(segundos, 2);
    sevseg1.refreshDisplay();
    //sevseg2.refreshDisplay();
}

