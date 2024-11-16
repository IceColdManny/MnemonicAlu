char data[26];
int number_of_bytes_received;
int A = 0;
int B = 0;
int resultado = 0;
bool negativo = false;
bool indeterminado = false;
String operacion = "";

void setup() { 
  for (int i = 2; i <= 9; i++) {
    pinMode(i, OUTPUT);  
    digitalWrite(i, LOW);
  }

  Serial.begin(9600);
  Serial.println("En línea, esperando instrucciones...");
}

void loop() {
  if (Serial.available()) {
    number_of_bytes_received = Serial.readBytesUntil(13, data, 25); 
    data[number_of_bytes_received] = 0;

    if (operacion == "") {
      if (strcmp(data, "sum") == 0 || strcmp(data, "res") == 0 || strcmp(data, "mul") == 0 || 
          strcmp(data, "div") == 0 || strcmp(data, "and") == 0 || strcmp(data, "or") == 0 || 
          strcmp(data, "xor") == 0 || strcmp(data, "not") == 0 || strcmp(data, "xnor") == 0 || 
          strcmp(data, "nand") == 0 || strcmp(data, "nor") == 0) {
        operacion = String(data);
        Serial.println(operacion + " seleccionada. Ingrese los valores de A y B separados por una coma (ej. 0,1):");
        delay(2000);  
      } 
    } else {

      leerValores();
      realizarOperacion();
      operacion = ""; 
    }
  }
}

void leerValores() {

  char* token = strtok(data, ",");
  if (token != NULL) {
    A = atoi(token);
  }
  token = strtok(NULL, ",");
  if (token != NULL) {
    B = atoi(token);
  }

  if ((A != 0 && A != 1) || (B != 0 && B != 1)) {
    Serial.println("Valores inválidos para A y B. Deben ser 0 o 1.");
    flushSerial();
    return;
  }
}

void realizarOperacion() {
  resultado = 0;  
  negativo = false;
  indeterminado = false;

  if (operacion == "sum") {
    resultado = A + B;
    mostrarResultado("Suma");
  } else if (operacion == "res") {
    resultado = A - B;
    if (resultado < 0) {
      negativo = true;
      resultado = -resultado;  
    }
    mostrarResultado("Resta");
  } else if (operacion == "mul") {
    resultado = A * B;
    mostrarResultado("Multiplicación");
  } else if (operacion == "div") {
    if (B != 0) {
      resultado = A / B;
    } else {
      indeterminado = true;
    }
    mostrarResultado("División");
  } else if (operacion == "and") {
    resultado = A & B;
    mostrarResultado("AND");
  } else if (operacion == "or") {
    resultado = A | B;
    mostrarResultado("OR");
  } else if (operacion == "xor") {
    resultado = A ^ B;
    mostrarResultado("XOR");
  } else if (operacion == "not") {
    resultado = ~A & 1;  // NOT se aplica solo a A
    mostrarResultado("NOT");
  } else if (operacion == "xnor") {
    resultado = ~(A ^ B) & 1;
    mostrarResultado("XNOR");
  } else if (operacion == "nand") {
    resultado = ~(A & B) & 1;
    mostrarResultado("NAND");
  } else if (operacion == "nor") {
    resultado = ~(A | B) & 1;
    mostrarResultado("NOR");
  }
  delay(1500);
}

void mostrarResultado(String operacion) {
  Serial.print(operacion);
  Serial.print(" de ");
  Serial.print(A);
  Serial.print(" y ");
  Serial.print(B);
  Serial.print(" = ");

  if (indeterminado) {
    Serial.println("Indeterminado (División por 0).");
  } else if (negativo) {
    Serial.println("Resultado negativo (-1)");
  } else if (!negativo && !indeterminado) { 
    Serial.println(resultado);
  } 

  clearDisplay();
  if (!indeterminado) {
    displayResultado(resultado);
  } else {
    displayIndeterminacion();
  }
  displaySignoNegativo(negativo);
}

void displayResultado(int numero) {
  int segmentos[] = {
    0b0111111,  // 0
    0b0000110,  // 1
    0b1011011   // 2
  };

  if (numero >= 0 && numero <= 2) {
    int segment = segmentos[numero];
    for (int i = 2; i <= 8; i++) {
      digitalWrite(i, segment & 1);
      segment >>= 1;
    }
  }
}

void displaySignoNegativo(bool isNegative) {
  digitalWrite(9, isNegative ? HIGH : LOW);  
}
void displayIndeterminacion() {
  digitalWrite(2, HIGH); 
  digitalWrite(5, HIGH); 
  digitalWrite(8, HIGH); 
}
void clearDisplay() {
  for (int i = 2; i <= 7; i++) {
    digitalWrite(i, LOW);
  }
  digitalWrite(8, LOW); 
}
void flushSerial() {
  while (Serial.available() > 0) {
    Serial.read(); 
  }
}


