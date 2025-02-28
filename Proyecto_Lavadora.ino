#include "SevSeg.h"

SevSeg displayPrincipal;

//Aqui toca poner en minutos 0, internamente debemos definir antes de ActualizarTiempo y ActualizarDisplays
int horas = 1, minutos = 0, segundos = 0;
unsigned long ultimoTiempo;

//Definicion de botones---------------------------
//START / STOP
const int BotonStartStop = PA2;
bool pausadoStart = false;
bool estadoPrevPausa       = HIGH;

//ENCENDIDO / APAGADO
const int BotonInOut = PA1;
bool pausadoEncendido = false;
bool estadoPrevEncendido       = HIGH;





void setup() {
  //Configuracion inicial para el display
  byte numDigitosPrincipal = 4;
  byte pinesDigitosPrincipal[] = {PC9, PC10, PC11, PC12};
  byte pinesSegmentosPrincipal[] = {PC0, PC1, PC2, PC3, PC5, PC6, PC7, PC8};

  bool resistenciasEnSegmentos = true;
  byte tipoDisplay = COMMON_ANODE;
  bool actualizarConRetardos = false;
  bool mostrarCeros = false;
  bool puntoDecimalInactivo = false;

  displayPrincipal.begin(tipoDisplay, numDigitosPrincipal, pinesDigitosPrincipal, 
                       pinesSegmentosPrincipal, resistenciasEnSegmentos, 
                       actualizarConRetardos, mostrarCeros, puntoDecimalInactivo);

  displayPrincipal.setBrightness(120);
  ultimoTiempo = millis();

  //Configuracion para el boton START/STOP
  pinMode(BotonStartStop, INPUT_PULLUP);
  pinMode(BotonInOut, INPUT_PULLUP);
}

void loop() {
  ManejarEncendido();
  //Si esta encendido
  if(!pausadoEncendido){
    ManejarPausa();

    if(!pausadoStart){
      //Aqui es cuando se empezo a lavar
      ActualizarTiempo();
      
    }else{
      //Aqui es cuando pausamos la lavada o esta apagado el switch
      
    }
    ActualizarDisplays();
  }
  
}

// Botón de pausa y empezar
void ManejarEncendido() {
  bool estadoActual = digitalRead(BotonInOut);
  if (estadoPrevEncendido == HIGH && estadoActual == LOW) {
    pausadoEncendido = !pausadoEncendido;
    if (!pausadoEncendido) ultimoTiempo = millis();
    delay(100);
  }
  estadoPrevEncendido = estadoActual;
}

// Botón de encendido y apagado
void ManejarPausa() {
  bool estadoActual = digitalRead(BotonStartStop);
  if (estadoPrevPausa == HIGH && estadoActual == LOW) {
    pausadoStart = !pausadoStart;
    if (!pausadoStart) ultimoTiempo = millis();
    delay(100);
  }
  estadoPrevPausa = estadoActual;
}

// Actualizar el tiempo en cuenta regresiva
void ActualizarTiempo() {
  if (millis() - ultimoTiempo >= 1000) {
    ultimoTiempo += 1000;
    
    if (segundos == 0) {
      if (minutos == 0) {
        if (horas == 0) {
          return;  // Se detiene en 00:00
        } else {
          horas--;
          minutos = 59;
          segundos = 59;
        }
      } else {
        minutos--;
        segundos = 59;
      }
    } else {
      segundos--;
    }
  }
}

// Actualizar displays
void ActualizarDisplays() {
  int tiempoFormateado = horas * 100 + minutos;
  displayPrincipal.setNumber(tiempoFormateado, 2);
  displayPrincipal.refreshDisplay();
}