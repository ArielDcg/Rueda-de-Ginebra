#include <Servo.h>

Servo myservo;

const float velocidadAngularMaxima = 4.71; 

unsigned long tiempoAnterior = 0;
unsigned long tiempoAnteriorImpresion = 0;
float vueltasPorSegundo = 0;
float aceleracionAngular = 0;
int valorServo = 90; // Iniciar desde 90
bool maxVelocidadAlcanzada = false;
bool sistemaDetenido = false;

void setup() {
  myservo.attach(9); 
  Serial.begin(9600); 
}

void loop() {
  if (sistemaDetenido) {
    return; 
  }

  unsigned long tiempoActual = millis();
  
  // Aumentar la velocidad cada 0.01 segundos
  if (!maxVelocidadAlcanzada && tiempoActual - tiempoAnterior >= 75) { // 7 ms = 0.075 s
    if (valorServo > 45) {
      valorServo--;
      myservo.write(valorServo);
    } else {
      maxVelocidadAlcanzada = true;
      tiempoAnterior = tiempoActual; 
    }
    tiempoAnterior = tiempoActual;
  }
  
  // Mantener la velocidad máxima durante 5 segundos
  if (maxVelocidadAlcanzada && tiempoActual - tiempoAnterior >= 5000) {
    myservo.write(90);
    sistemaDetenido = true;
  }
  
  if (tiempoActual - tiempoAnteriorImpresion >= 500) { // Cada 250 ms
    if (!maxVelocidadAlcanzada) {
      vueltasPorSegundo = ((-valorServo + 90) / 90.0) * (velocidadAngularMaxima / (2 * PI));
      aceleracionAngular = vueltasPorSegundo * 2 * PI / (tiempoActual / 1000.0);
    } else {
      vueltasPorSegundo = ((-valorServo + 90) / 90.0) * (velocidadAngularMaxima / (2 * PI));
      aceleracionAngular = 0;
    }
    Serial.print("Vueltas por segundo: ");
    Serial.print(vueltasPorSegundo);
    Serial.print(" | Aceleración angular: ");
    Serial.println(aceleracionAngular);
    tiempoAnteriorImpresion = tiempoActual;
  }
}
