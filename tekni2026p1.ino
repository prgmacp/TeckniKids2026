#include <SoftwareSerial.h>
#include <Servo.h>

// ==========================================
//        CONFIGURACIÓN DE PINES
// ==========================================

// ¡ATENCIÓN! Actualiza estos 4 números con los pines donde conectaste los motores
const int IN1 = 2; // Motor Izquierdo (Avanzar)
const int IN2 = 3; // Motor Izquierdo (Retroceder)
const int IN3 = 4; // Motor Derecho (Avanzar)
const int IN4 = 5; // Motor Derecho (Retroceder)

// Pin del Servomotor (Garra)
const int pinServo = 9; // Si usaste el pin 9 para un motor, cambia este pin a otro (ej. 3)

// Configuración del Bluetooth
// El pin TX del HC-05 va al pin 10 del Arduino
// El pin RX del HC-05 va al pin 11 del Arduino
SoftwareSerial BT(10, 11);

Servo garra;
char comando; 

void setup() {
  // Configurar pines de los motores como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Forzar el apagado de todos los motores al encender para evitar que giren solos
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // Inicializar la garra
  garra.attach(pinServo);
  garra.write(90); // Posición inicial (abierta)

  // Iniciar comunicaciones
  Serial.begin(9600); // Monitor serial para PC
  BT.begin(9600);     // Comunicación con el módulo Bluetooth HC-05
}

void loop() {
  // Leer los datos enviados por el script de Python
  if (BT.available()) {
    comando = BT.read();
    Serial.print("Comando recibido: ");
    Serial.println(comando);

    // Ejecutar la acción correspondiente
    switch (comando) {
      case 'F': 
        adelante();
        break;
      case 'B': 
        atras();
        break;
      case 'L': 
        izquierda();
        break;
      case 'R': 
        derecha();
        break;
      case 'S': 
        detener();
        break;
      case 'A': 
        abrirGarra();
        break;
      case 'C': 
        cerrarGarra();
        break;
    }
  }
}

// ==========================================
//           FUNCIONES DE MOVIMIENTO
// ==========================================

void adelante() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void atras() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void izquierda() {
  // Apaga el motor izquierdo, mantiene encendido el derecho
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void derecha() {
  // Mantiene encendido el motor izquierdo, apaga el derecho
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
}

void detener() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// ==========================================
//           FUNCIONES DE LA GARRA
// ==========================================

void abrirGarra() {
  garra.write(90); // Ángulo para abrir
}

void cerrarGarra() {
  garra.write(10); // Ángulo para cerrar (ajustar si la pinza hace ruido de forzada)
}