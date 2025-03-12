#include "SevSeg.h"

SevSeg sevseg;

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
bool temperaturaBloqueada = false;  


int temperaturaSeleccionada = 0;

//Nivel de Agua
const int botonNivelAgua = PD2; 
const int ledBajoAgua = PB13;   
const int ledMedioAgua = PB12;  
const int ledAltoAgua = PB11;    

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

int contadorAgua = 0;
int contadorLavado = 0;
int contadorEnjuague = 0;
int contadorCentrifugado = 0;

void setup() {
    byte numDigits = 4;
    byte digitPins[] = {PC9, PC10, PC11, PC12};
    byte segmentPins[] = {PC0, PC1, PC2, PC3, PC5, PC6, PC7, PC8};

    bool resistorsOnSegments = true;
    byte hardwareConfig = COMMON_ANODE;
    bool updateWithDelays = false;
    bool leadingZeros = false;
    bool disableDecPoint = false;

    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins,
                  resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);

    sevseg.setBrightness(90);

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
    sevseg.setNumber(horas * 100 + minutos, 2);
    sevseg.refreshDisplay();
    sevseg.blank();
}

void loop() {
    manejarBotonEncender(); // Solo manejar este botón si el sistema está apagado

    if (sistemaEncendido) {
        manejarBotones();
        actualizarTemporizador();
        mostrarTiempo();
    } else {
        apagarTodo(); // Si el sistema está apagado, asegurarse de que todo esté apagado
    }

    if (!enMarcha && debounce(botonTemperatura, prevEstadoTemperatura, lastUpdateTime)) {
        cambiarTemperatura();  
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

void manejarBotonEncender() {
    if (debounce(botonEncenderApagar, prevEstadoEncender, lastUpdateTime)) {
        sistemaEncendido = !sistemaEncendido;
        digitalWrite(ledEncendido, sistemaEncendido ? HIGH : LOW);

        if (!sistemaEncendido) {
            temperaturaBloqueada = false; 
            apagarTodo();
        }
    }
}


void apagarTodo() {
    enMarcha = false;
    cantidadSeleccionada = 0;
    tipoLavadoSeleccionado = 0;
    temperaturaSeleccionada = 0;
    nivelAguaSeleccionado = 0;
    tiempoLavadoSeleccionado = 0;
    enjuagueSeleccionado = 0;
    centrifugadoSeleccionado = 0;
    horas = minutos = segundos = 0;

    sevseg.blank();

    int leds[] = {ledEncendido, ledVerde, ledRojo, led18kg, led12kg, led7kg, ledLavNormal, ledLavRapido, ledLavFuerte, ledFrio, ledCaliente,
    ledBajoAgua, ledMedioAgua, ledAltoAgua, ledLav5, ledLav10, ledLav20, ledEnjuague1, ledEnjuague2, ledEnjuague3, ledCentrifugadoBajo, 
    ledCentrifugadoMedio, ledCentrifugadoAlto}; 
    for (int i = 0; i < 23; i++) {
        digitalWrite(leds[i], LOW);
    }
}

void manejarBotones() {
    if (debounce(botonIniciarPausar, prevEstadoIniciar, lastUpdateTime)) {
        
        if (temperaturaSeleccionada == 0) {  
            return;  // Sale de la función y no inicia el lavado
        }

        if ((horas > 0 || minutos > 0 || segundos > 0) && cantidadSeleccionada > 0) {
            if (tipoLavadoSeleccionado == 0) {
                tipoLavadoSeleccionado = 1;
                configurarTipoLavado(tipoLavadoSeleccionado);
            }
            enMarcha = !enMarcha;
            digitalWrite(ledVerde, enMarcha ? HIGH : LOW);
            digitalWrite(ledRojo, enMarcha ? LOW : HIGH);

            if (enMarcha) {
                temperaturaBloqueada = true;  
            }
        }
        lastUpdateTime = millis();
    }

    
    if (!temperaturaBloqueada && debounce(botonTemperatura, prevEstadoTemperatura, lastUpdateTime)) {
        cambiarTemperatura();  
    }

    if (!enMarcha && debounce(botonCantidadRopa, prevEstadoRopa, lastUpdateTime)) {
        cantidadSeleccionada = (cantidadSeleccionada % 4) + 1;
        configurarCantidadRopa(cantidadSeleccionada);
    }

    if (!enMarcha && debounce(botonSeleccionarLavado, prevEstadoTipoLavado, lastUpdateTime)) {
        tipoLavadoSeleccionado = (tipoLavadoSeleccionado % 4) + 1;
        configurarTipoLavado(tipoLavadoSeleccionado);
    }

    if (!enMarcha && debounce(botonNivelAgua, prevEstadoNivelAgua, lastUpdateTime)) {
        nivelAguaSeleccionado = (nivelAguaSeleccionado % 4) + 1;
        configurarNivelAgua(nivelAguaSeleccionado);
    }

    if (!enMarcha && debounce(botonLavado, prevEstadoLavado, lastUpdateTime)) {
        tiempoLavadoSeleccionado = (tiempoLavadoSeleccionado % 4) + 1;
        configurarNivelLavado(tiempoLavadoSeleccionado);
    }

    if (!enMarcha && debounce(botonEnjuague, prevEstadoEnjuague, lastUpdateTime)) {
        enjuagueSeleccionado = (enjuagueSeleccionado % 4) + 1;
        configurarEnjuague(enjuagueSeleccionado);
    }

    if (!enMarcha && debounce(botonCentrifugado, prevEstadoCentrifugado, lastUpdateTime)) {
        centrifugadoSeleccionado = (centrifugadoSeleccionado % 4) + 1;
        configurarCentrifugado(centrifugadoSeleccionado);
    }

    ajustarTiempo();
}

void ajustarTiempo(){
  // Ajustar segundos a minutos
    if (segundos >= 60) {
        minutos += segundos / 60; 
        segundos = segundos % 60;
    }
    if (minutos >= 60) {
        horas += minutos / 60;
        minutos = minutos % 60;
    }
    if (segundos < 0){
      segundos = 0;
    }
    if (minutos < 0){
      minutos = 0;
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
            digitalWrite(ledLavFuerte, HIGH); minutos += 6; segundos += 0; 
            digitalWrite(ledLav20, HIGH);
            digitalWrite(ledEnjuague3, HIGH);
            digitalWrite(ledCentrifugadoAlto, HIGH);
            tiempoLavadoSeleccionado = enjuagueSeleccionado = centrifugadoSeleccionado = 3;
            contadorLavado = contadorEnjuague = contadorCentrifugado = 1;
            break;
        case 2: 
            digitalWrite(ledLavRapido, HIGH); minutos += 1; segundos += 30;
            digitalWrite(ledLav5, HIGH);
            digitalWrite(ledEnjuague1, HIGH);
            digitalWrite(ledCentrifugadoBajo, HIGH); 
            tiempoLavadoSeleccionado = enjuagueSeleccionado = centrifugadoSeleccionado = 1;
            contadorLavado = contadorEnjuague = contadorCentrifugado = 3;
            break;
        case 3: 
            digitalWrite(ledLavNormal, HIGH); minutos += 3; segundos += 0;
            digitalWrite(ledLav10, HIGH);
            digitalWrite(ledEnjuague2, HIGH);
            digitalWrite(ledCentrifugadoMedio, HIGH); 
            tiempoLavadoSeleccionado = enjuagueSeleccionado = centrifugadoSeleccionado = 2;
            contadorLavado = contadorEnjuague = contadorCentrifugado = 2;
            break;
        case 4:
            minutos -= 10;
            segundos -= 30;
            contadorLavado = contadorEnjuague = contadorCentrifugado = 0;
            break;
        default:
            minutos = 0;
            segundos = 0;
            break;
  }
}

void configurarCantidadRopa(int cantidad) {
    // Apagar todos los LEDs de nivel de agua
    digitalWrite(ledBajoAgua, LOW);
    digitalWrite(ledMedioAgua, LOW);
    digitalWrite(ledAltoAgua, LOW);

    // Apagar todos los LEDs de cantidad de ropa
    digitalWrite(led18kg, LOW);
    digitalWrite(led12kg, LOW);
    digitalWrite(led7kg, LOW);

    switch (cantidad) {
        case 1: 
            digitalWrite(led7kg, HIGH); // 7kg
            // Enciende el LED de nivel de agua bajo (P13)
            digitalWrite(ledBajoAgua, HIGH); 
            contadorAgua = nivelAguaSeleccionado = 1;
            horas = 0; 
            minutos += 1; 
            break;
        case 2: 
            digitalWrite(led12kg, HIGH); // 12kg
            // Enciende el LED de nivel de agua medio (P12)
            digitalWrite(ledMedioAgua, HIGH);
            contadorAgua = nivelAguaSeleccionado = 2; 
            horas = 0; 
            minutos += 2; 
            break;
        case 3: 
            digitalWrite(led18kg, HIGH); // 18kg
            // Enciende el LED de nivel de agua alto (P11)
            digitalWrite(ledAltoAgua, HIGH); 
            contadorAgua = nivelAguaSeleccionado = 3;
            horas = 0; 
            minutos += 3; 
            break;
        case 4: 
            horas = 0; 
            minutos -= 6;
            break;
        default:
            horas = 0;
            minutos = 0;
            break;
    }
}

void cambiarTemperatura() {
    if (temperaturaBloqueada) return; 

    static int estadoTemperatura = 0;  

    // Apagar ambos LEDs antes de cambiar estado
    digitalWrite(ledFrio, LOW);
    digitalWrite(ledCaliente, LOW);

    switch (estadoTemperatura) {
        case 0:
            digitalWrite(ledFrio, HIGH);
            segundos += 30;
            temperaturaSeleccionada = 1;  
            ajustarTiempo();
            break;
        case 1:
            segundos -= 30;
            temperaturaSeleccionada = 1;  
            ajustarTiempo();
            break;
        case 2:
            digitalWrite(ledCaliente, HIGH);
            minutos += 2;
            temperaturaSeleccionada = 1;  
            ajustarTiempo();
            break;
        case 3:
            minutos -= 2;
            temperaturaSeleccionada = 1;  
            ajustarTiempo();
            break;
    }

    // Alternar estado (de 0 → 1 → 2 → 3 → 0)
    estadoTemperatura = (estadoTemperatura + 1) % 4;
}





void configurarNivelAgua(int cantidad) {
    digitalWrite(ledBajoAgua, LOW);
    digitalWrite(ledMedioAgua, LOW);
    digitalWrite(ledAltoAgua, LOW);

    switch (cantidad) {
      case 1: digitalWrite(ledBajoAgua, HIGH); horas = 0; minutos += 1; contadorAgua++;
            break;
      case 2: digitalWrite(ledMedioAgua, HIGH); horas = 0; minutos += 2; contadorAgua++;
            break;
      case 3: digitalWrite(ledAltoAgua, HIGH); horas = 0; minutos += 3; contadorAgua++;
            break;
      case 4: 
            switch (contadorAgua){
              case 1: minutos -= 3; break;
              case 2: minutos -= 5; break;
              case 3: minutos -= 6; break;
              default: minutos = 0; break;
            }
            contadorAgua = 0;
            break;
      default:
            horas = 0;
            minutos = 0;
            break;
  }
}

void configurarNivelLavado(int cantidad) {
    digitalWrite(ledLav5, LOW);
    digitalWrite(ledLav10, LOW);
    digitalWrite(ledLav20, LOW);

    switch (cantidad) {
      case 1: digitalWrite(ledLav5, HIGH); horas = 0; minutos += 0; segundos += 30; contadorLavado++;
            break;
      case 2: digitalWrite(ledLav10, HIGH); horas = 0; minutos += 1; segundos += 0; contadorLavado++;
            break;
      case 3: digitalWrite(ledLav20, HIGH); horas = 0; minutos += 2; segundos += 0; contadorLavado++;
            break;
      case 4:
            switch (contadorLavado){
              case 1: minutos -= 2; break;
              case 2: minutos -= 3; break;
              case 3: minutos -= 3; segundos -= 30; break;
              default: minutos = 0; segundos = 0; break;
            }
            contadorLavado = 0;
            break;
      default:
            horas = 0;
            minutos = 0;
            segundos = 0;
            break;
  }
}

void configurarEnjuague(int cantidad) {
    digitalWrite(ledEnjuague1, LOW);
    digitalWrite(ledEnjuague2, LOW);
    digitalWrite(ledEnjuague3, LOW);

    switch (cantidad) {
      case 1: digitalWrite(ledEnjuague1, HIGH); horas = 0; minutos += 0; segundos += 30; contadorEnjuague++;
            break;
      case 2: digitalWrite(ledEnjuague2, HIGH); horas = 0; minutos += 1; segundos += 0; contadorEnjuague++;
            break;
      case 3: digitalWrite(ledEnjuague3, HIGH); horas = 0; minutos += 2; segundos += 0; contadorEnjuague++;
            break;
      case 4:
            switch (contadorEnjuague){
              case 1: minutos -= 2; break;
              case 2: minutos -= 3; break;
              case 3: minutos -= 3; segundos -= 30; break;
              default: minutos = 0; segundos = 0; break;
            }
            contadorEnjuague = 0;
            break;
      default:
            horas = 0;
            minutos = 0;
            segundos = 0;
            break;
  }
}

void configurarCentrifugado(int cantidad) {
    digitalWrite(ledCentrifugadoBajo, LOW);
    digitalWrite(ledCentrifugadoMedio, LOW);
    digitalWrite(ledCentrifugadoAlto, LOW);

    switch (cantidad) {
      case 1: digitalWrite(ledCentrifugadoBajo, HIGH); horas = 0; minutos += 0; segundos += 30; contadorCentrifugado++;
            break;
      case 2: digitalWrite(ledCentrifugadoMedio, HIGH); horas = 0; minutos += 1; segundos += 0; contadorCentrifugado++;
            break;
      case 3: digitalWrite(ledCentrifugadoAlto, HIGH); horas = 0; minutos += 2; segundos += 0; contadorCentrifugado++;
            break;
      case 4: 
            switch (contadorCentrifugado){
              case 1: minutos -= 2; break;
              case 2: minutos -= 3; break;
              case 3: minutos -= 3; segundos -= 30; break;
              default: minutos = 0; segundos = 0; break;
            }
            contadorCentrifugado = 0;
            break;
      default:
            horas = 0;
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
                    sevseg.blank();  // Limpiar el display
                    sevseg.setChars("End");  // Mostrar "End"
                    sevseg.refreshDisplay();
                    return;
                    //sevseg2.blank();
                }
            }
            mostrarTiempo();
        }
    }
}

void mostrarTiempo() {
    sevseg.setNumber(minutos * 100 + segundos, 2);
    sevseg.refreshDisplay();
}

