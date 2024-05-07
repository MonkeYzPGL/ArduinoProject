#include <SoftwareSerial.h>

const int ledPin = 9;
const int sensorPin = 7;

boolean val = 0;
int consecutiveOnes = 0;

SoftwareSerial mySerial(2, 3);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // Citirea starii senzorului de sunet.
  val = digitalRead(sensorPin);

  // Verificam daca am detectat 1 
  if (val == HIGH) {
    consecutiveOnes++;

    // Verifica daca am detectat 5 de 1 consecutiv
    if (consecutiveOnes == 5) {
      Serial.println("SOUND DETECTED");
      digitalWrite(ledPin, HIGH);
      delay(10000);  // led aprins pt 10 sec
      digitalWrite(ledPin, LOW);

      consecutiveOnes = 0;
    }
  } else {
    // daca este 0, resetam numarul de 1
    consecutiveOnes = 0;
  }

  // Comunicarea cu modulul Bluetooth
  if (mySerial.available()) {
    String receivedData = mySerial.readStringUntil('\r');
    Serial.println("Received from Bluetooth: " + receivedData);

    if (receivedData.compareTo("ON") == 0) { //Aprindere LED
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    } else if (receivedData.compareTo("OFF") == 0) { //Oprire LED
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    } else if (receivedData.compareTo("BLINK") == 0){ //Blink
      for(int i = 0; i < 10; i++){
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin,LOW);
        delay(500);
      } 
    }else if (isNumber(receivedData)) { //Setarea intensitatii
      int intensity = receivedData.toInt();
      if (intensity >= 0 && intensity <= 255) {
        analogWrite(ledPin, intensity);
        Serial.println("LED intensity set to: " + String(intensity));
      } else {
        Serial.println("Intensitatea LED-ului trebuie să fie între 0 și 255");
      }
    } else {
      Serial.println("Nicio comanda cunoscuta");
    }
  }

  // Transmiterea datelor catre bluetooth
  if (Serial.available()) {
    char data = Serial.read();
    mySerial.write(data);
  }

  delay(25);  // Mici pauze pentru citiri
}

boolean isNumber(String str) { //Functia de verificare pentru numar
  for (int i = 0; i < str.length(); i++) {
    if (!isdigit(str.charAt(i))) {
      return false;
    }
  }
  return true;
}
