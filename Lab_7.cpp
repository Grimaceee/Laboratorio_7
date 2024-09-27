#include<Arduino.h>

// Pines para los LEDs
const int Led_v = 2;  
const int Led_a = 4;  
const int Led_r = 5;  

// Pin del potenciómetro
const int poten = 34;  

// Variable para la lectura del potenciómetro
int lectura_poten = 0;

// Función para encender un LED por cierto tiempo
void Encender_LED(int led, int tiempo) {
  int ledPin;

  // Verificamos qué LED queremos encender
  switch (led) {
    case 1:
      ledPin = Led_v;
      break;
    case 2:
      ledPin = Led_a;
      break;
    case 3:
      ledPin = Led_r;
      break;
    default:
      Serial.println("Ups, LED inválido.");
      return;
  }

  // Encendemos el LED
  digitalWrite(ledPin, HIGH);
  vTaskDelay(tiempo / portTICK_PERIOD_MS);  
  digitalWrite(ledPin, LOW);  
}

// Función para mostrar el menú principal
void mostrarMenu() {
  Serial.println("\n--- ¡Hola! Elige qué quieres hacer ---");
  Serial.println("1. Checar el valor del potenciómetro");
  Serial.println("2. Encender uno de los LEDs");
  Serial.print("Escribe tu opción: ");
}

// Función para leer el valor del potenciómetro
void leerPoten() {
  lectura_poten = analogRead(poten);  
  float voltaje = (lectura_poten / 4095.0) * 3.3;  
  Serial.print("¡El valor del potenciómetro es: ");
  Serial.print(voltaje);
  Serial.println("V!");
}

// Función para controlar los LEDs
void controlarLEDs() {
  Serial.print("Dime qué LED (1 = Verde, 2 = Azul, 3 = Rojo) y por cuánto tiempo (en ms), ejemplo '1,500': ");
  
  while (!Serial.available()) {
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  String comando = Serial.readStringUntil('\n'); 
  comando.trim();  

  // Inicia el comando en LED y tiempo
  int comaIndex = comando.indexOf(',');
  if (comaIndex == -1) {
    Serial.println("Formato incorrecto, usa LED,tiempo. Ejemplo: '1,500'");
    return;
  }

  int led = comando.substring(0, comaIndex).toInt();
  int tiempo = comando.substring(comaIndex + 1).toInt();

  // Validamos si el LED y el tiempo son correctos
  if (led < 1 || led > 3 || tiempo <= 0) {
    Serial.println("Algo está mal... asegúrate de que LED esté entre 1 y 3 y el tiempo sea positivo.");
  } else {
    Encender_LED(led, tiempo);  
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(Led_v, OUTPUT);
  pinMode(Led_a, OUTPUT);
  pinMode(Led_r, OUTPUT);

  pinMode(poten, INPUT);

  mostrarMenu();
}

void loop() {
  if (Serial.available()) {
    String opcion = Serial.readStringUntil('\n');  
    opcion.trim();  

    if (opcion == "1") {
      leerPoten();
    } else if (opcion == "2") {
      controlarLEDs();
    } else {
      Serial.println("Opción no válida, intenta de nuevo.");
    }

    mostrarMenu();
  }
}
