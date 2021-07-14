/*************FLAGS*************/
boolean FLAG_OK = false;
boolean FLAG_STOP = false;
boolean FLAG_TIME = false;
boolean FLAG_BOMBA = false;
boolean FLAG_BUZZER = false;
boolean FLAG_ALARM = false;
boolean FLAG_TIME_OUT = false;
boolean FLAG_SENSOR_F = false;
boolean FLAG_SENSOR_U = false;
boolean FLAG_TIME_SENSOR_F = false;
boolean FLAG_TIME_SENSOR_U = false;
boolean FLAG_LED_F_R = false;
boolean FLAG_LED_F_Y = false;
boolean FLAG_LED_U_R = false;
boolean FLAG_LED_U_Y = false;
boolean FLAG_LED_U_G = false;
boolean FLAG_CONTROL = false;
boolean FLAG_TIME_ALARM = false;
boolean FLAG_SIST_1 = false;
boolean FLAG_SIST_0 = true;

/*******AUXILIARES********/
boolean AUX_LED1 = false;
boolean AUX_LED2 = false;
boolean AUX_LED3 = false;
boolean AUX_LED4 = false;
boolean AUX_LED5 = false;
boolean AUX_BOMBA = false;
boolean AUX_ALARM = false;

unsigned char texto[10] = "00000000";
unsigned char c_texto = 0;
unsigned long tempoA = 0;
int tempoB = 0;
boolean sirene = false;
double cm = 0.0;
double cmAux = 0.0;
int sensorFumaca = 0;
int Fumacadetectada = 250;



/************CONFIG_PINOS************/
const int sensor = A0;        // saída para o sensor de fumaça MQ-2
const int buzzer = 12;      // saída para o buzzer
const int ledAmareloF = 11;   // saída para o led amarelo referente ao sensor de fumaça
const int ledVermelhoF = 10;  // saída para o led vermelho referente ao sensor de fumaça
const int ledVerdeU = 9;      // saída para o led  verde referente ao sensor ultrassonico
const int ledAmareloU = 8;    // saída para o led  amarelo referente ao sensor ultrassonico
const int ledVermelhoU = 7;   // saída para o led  vermelho referente ao sensor ultrassonico
const int IN1 = 4;            // saída para o relé IN1

//Função que calcula a distancia usando o sensor Ultrassonico
long lerDistancia(int echoPin, int triggerPin)
{        
  pinMode(triggerPin, OUTPUT); 
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup()
{
  Serial.begin(9600);
  
  //  inicialização dos componentes
  pinMode(IN1, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledAmareloF, OUTPUT);
  pinMode(ledVermelhoF, OUTPUT);
  pinMode(ledVerdeU, OUTPUT);
  pinMode(ledAmareloU, OUTPUT);
  pinMode(ledVermelhoU, OUTPUT);

  //  inicialmente todos em nivel baixo
  digitalWrite(IN1, HIGH);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledAmareloF, LOW);
  digitalWrite(ledVermelhoF, LOW);
  digitalWrite(ledVerdeU, LOW);
  digitalWrite(ledAmareloU, LOW);
  digitalWrite(ledVermelhoU, LOW);
}

void loop()
{
  
  leituraSerial();
  
  void interrupt
  if(FLAG_TIME) {
    if(FLAG_SIST_1 || buzzer)
      tempoB = 950;
    else
      tempoB = 200;
    if(((millis() - tempoA) > tempoB)) {
      FLAG_TIME_OUT = true;
      FLAG_TIME_SENSOR_F = false;
      FLAG_TIME_SENSOR_U = false;
      FLAG_TIME_ALARM = false;
    } else 
      if((millis() - tempoA) > 120 && !FLAG_TIME_ALARM) {
        FLAG_ALARM = true;
        FLAG_TIME_ALARM = true;
      } else 
      if((millis() - tempoA) > 80 && !FLAG_TIME_SENSOR_U) {
        FLAG_SENSOR_U = true;
        FLAG_TIME_SENSOR_U = true;
      } else 
        if((millis() - tempoA) > 40 && !FLAG_TIME_SENSOR_F) {
          FLAG_SENSOR_F = true;
          FLAG_TIME_SENSOR_F = true;
        }  
  }
  cm = 0.01399 * lerDistancia(6, 5);  // armazena o valor lido pelo sensor Ultrassonico
  cmAux = cm*100;
  sensorFumaca = analogRead(sensor);  // lê a entrada do sensor e armazena o valor
  
  if(FLAG_TIME_OUT && FLAG_OK) {
    //TIMER ATUALIZA
    Serial.print("[TIMER]");
    FLAG_TIME_OUT = false;
    FLAG_TIME = false;
  }

  if(FLAG_SENSOR_F && FLAG_OK) {
    delay(5);
    Serial.print("[F0");
    Serial.print(sensorFumaca/100);
    Serial.print((sensorFumaca/10)%10);
    Serial.print(sensorFumaca%10);
    Serial.print(']');
    FLAG_SENSOR_F = false;
  }
  if(FLAG_SENSOR_U && FLAG_OK) {
    delay(5);
    Serial.print("[U");
    if(cmAux<1000)
      Serial.print("0");
    Serial.print((int)cmAux/100);
    Serial.print(((int)cmAux/10)%10);
    Serial.print((int)cmAux%10);
    Serial.print(']');
    FLAG_SENSOR_U = false;
  }
  if(FLAG_ALARM && FLAG_OK) {
    delay(5);
    if(FLAG_SIST_0) {
      Serial.print("[ALRM0]");
      FLAG_SIST_1 = false;
    }
    else {
      FLAG_SIST_1 = true;
      Serial.print("[ALRM1]");
    }
    FLAG_ALARM = false;
  }
  if(FLAG_STOP) {
    digitalWrite(IN1, HIGH);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledAmareloF, LOW);
    digitalWrite(ledVermelhoF, LOW);
    digitalWrite(ledVerdeU, LOW);
    digitalWrite(ledAmareloU, LOW);
    digitalWrite(ledVermelhoU, LOW);
    sirene = false;
    FLAG_STOP = false;
  }
  
  if(FLAG_CONTROL) {
    if(FLAG_LED_F_Y && FLAG_OK) {
      AUX_LED1 = !AUX_LED1;
      if(AUX_LED1)
        digitalWrite(ledAmareloF, HIGH);
    else 
      digitalWrite(ledAmareloF, LOW);
      FLAG_LED_F_Y  = 0;
    }
  
    if(FLAG_LED_F_R && FLAG_OK) {
      AUX_LED2 = !AUX_LED2;
      if(AUX_LED2)
        digitalWrite(ledVermelhoF, HIGH);
      else 
        digitalWrite(ledVermelhoF, LOW);
      FLAG_LED_F_R = 0;
    }
    
    if(FLAG_LED_U_G && FLAG_OK) {
      AUX_LED3 = !AUX_LED3;
      if(AUX_LED3) 
        digitalWrite(ledVerdeU, HIGH);
      else 
        digitalWrite(ledVerdeU, LOW);
      FLAG_LED_U_G  = 0;
    }
    
    if(FLAG_LED_U_Y && FLAG_OK) {
      AUX_LED4 = !AUX_LED4;
      if(AUX_LED4)
        digitalWrite(ledAmareloU, HIGH);
        else
          digitalWrite(ledAmareloU, LOW);
      FLAG_LED_U_Y  = 0;
    }
    
    if(FLAG_LED_U_R && FLAG_OK) {
      AUX_LED5 = !AUX_LED5;
      if(AUX_LED5)
        digitalWrite(ledVermelhoU, HIGH);
      else 
        digitalWrite(ledVermelhoU, LOW);
      FLAG_LED_U_R  = 0;
      }
    
    if(FLAG_BOMBA && FLAG_OK) {
      AUX_BOMBA = !AUX_BOMBA;
      if(AUX_BOMBA)
        digitalWrite(IN1, LOW);
      else 
        digitalWrite(IN1, HIGH);
      FLAG_BOMBA  = 0;
    }
    
    if(FLAG_SIST_1){
      digitalWrite(buzzer, HIGH);
      digitalWrite(ledVermelhoF, HIGH);
      digitalWrite(ledAmareloF, LOW);
      delay(150);
      digitalWrite(ledVermelhoF, LOW);
      digitalWrite(ledAmareloF, HIGH);
      delay(150);
    }
  } 
    //AUTOMATICO 
  else 
  {
    if(sensorFumaca > Fumacadetectada) {
      digitalWrite(buzzer, HIGH);
      // liga a bomba submersa
      digitalWrite(IN1, LOW);
      sirene = true;
      FLAG_SIST_0 = false;
      FLAG_SIST_1 = true; 
    } else {
        digitalWrite(ledVermelhoF, LOW);
        digitalWrite(ledAmareloF, LOW);
        digitalWrite(buzzer, LOW);
        digitalWrite(IN1, HIGH);
        sirene = false;
        FLAG_SIST_0 = true; 
        FLAG_SIST_1 = false;
      }
    if(cm > 15) { 
      digitalWrite(ledVerdeU,LOW);
      digitalWrite(ledAmareloU,LOW);
      digitalWrite(ledVermelhoU,HIGH);
      // Desliga a bomba submersa
      digitalWrite(IN1, HIGH);
    } else if(cm > 10) {
        digitalWrite(ledVerdeU,LOW);
        digitalWrite(ledAmareloU,HIGH);
        digitalWrite(ledVermelhoU,LOW);
    } else {
        digitalWrite(ledVerdeU,HIGH);
        digitalWrite(ledAmareloU,LOW);
        digitalWrite(ledVermelhoU,LOW);
    }
    if(sirene){
      digitalWrite(ledVermelhoF, HIGH);
      digitalWrite(ledAmareloF, LOW);
      delay(150);
      digitalWrite(ledVermelhoF, LOW);
      digitalWrite(ledAmareloF, HIGH);
      delay(150);
      if(sensorFumaca > Fumacadetectada) {
        sirene = false;
        digitalWrite(ledAmareloF, LOW);
        digitalWrite(ledVermelhoF, LOW);
      }
    }
  }
}
  
void leituraSerial() {
  if(Serial.available() > 0) {
    texto[c_texto] = Serial.read();
    if(texto[0] != '[') c_texto = 0;
    else {
      if(texto[c_texto] == ']') {
      
        c_texto = 0;                //[BOMBA]
        if((texto[0] == '[')&&(texto[1] == 'B')&&(texto[2] == 'O')&&(texto[3] == 'M')&&(texto[4] == 'B')&&(texto[5] == 'A')&&(texto[6] == ']')) 
          FLAG_BOMBA = true;
                                    //[LEDFY] - led sensor fumaça yellow
        if((texto[0] == '[')&&(texto[1] == 'L')&&(texto[2] == 'E')&&(texto[3] == 'D')&&(texto[4] == 'F')&&(texto[5] == 'Y')&&(texto[6] == ']'))
          FLAG_LED_F_Y = true;
                                    //[LEDFR] - led sensor fumaça red
        if((texto[0] == '[')&&(texto[1] == 'L')&&(texto[2] == 'E')&&(texto[3] == 'D')&&(texto[4] == 'F')&&(texto[5] == 'R')&&(texto[6] == ']'))
          FLAG_LED_F_R = true;
                                    //[LEDUG] - led sensor ultrassonico green
        if((texto[0] == '[')&&(texto[1] == 'L')&&(texto[2] == 'E')&&(texto[3] == 'D')&&(texto[4] == 'U')&&(texto[5] == 'G')&&(texto[6] == ']'))
          FLAG_LED_U_G = true;
                                    //[LEDUY] - led sensor ultrassonico yellow
        if((texto[0] == '[')&&(texto[1] == 'L')&&(texto[2] == 'E')&&(texto[3] == 'D')&&(texto[4] == 'U')&&(texto[5] == 'Y')&&(texto[6] == ']'))
          FLAG_LED_U_Y = true;
                                    //[LEDUR] - led sensor ultrassonico red
        if((texto[0] == '[')&&(texto[1] == 'L')&&(texto[2] == 'E')&&(texto[3] == 'D')&&(texto[4] == 'U')&&(texto[5] == 'R')&&(texto[6] == ']'))
          FLAG_LED_U_R = true; 
                                    //[ATVS1] - Ativa o sistema
        if((texto[0] == '[')&&(texto[1] == 'A')&&(texto[2] == 'T')&&(texto[3] == 'V')&&(texto[4] == 'S')&&(texto[5] == '1')&&(texto[6] == ']')) {
          FLAG_SIST_0 = false;
          FLAG_SIST_1 = true; 
        }                           //[ATVS0] - Desativa o sistema
        if((texto[0] == '[')&&(texto[1] == 'A')&&(texto[2] == 'T')&&(texto[3] == 'V')&&(texto[4] == 'S')&&(texto[5] == '0')&&(texto[6] == ']')) {
          FLAG_SIST_0 = true; 
          FLAG_SIST_1 = false;
          FLAG_STOP = true;
        }
                                    //[TIMER] - timer para atualizar
        if((texto[0] == '[')&&(texto[1] == 'T')&&(texto[2] == 'I')&&(texto[3] == 'M')&&(texto[4] == 'E')&&(texto[5] == 'R')&&(texto[6] ==']')){
          FLAG_TIME = true;
          tempoA = millis();
        }
                                    //[START] - habilita o sistema a funcionar
        if((texto[0] == '[')&&(texto[1] == 'S')&&(texto[2] == 'T')&&(texto[3] == 'A')&&(texto[4] == 'R')&&(texto[5] == 'T')&&(texto[6] == ']'))
          FLAG_OK = true;
                                    //[STOPX] - para todo o sistema
        if((texto[0] == '[')&&(texto[1] == 'S')&&(texto[2] == 'T')&&(texto[3] == 'O')&&(texto[4] == 'P')&&(texto[5] == 'X')&&(texto[6] == ']')) {
          FLAG_OK = false;
          FLAG_STOP = true;
        }
                                    //[CTROL] - para todo o sistema
        if((texto[0] == '[')&&(texto[1] == 'C')&&(texto[2] == 'T')&&(texto[3] == 'R')&&(texto[4] == 'O')&&(texto[5] == 'L')&&(texto[6] == ']')) {
          FLAG_CONTROL = !FLAG_CONTROL;
          FLAG_STOP = true;
        }
      } else {
          c_texto++;
          if(c_texto > 6) c_texto = 0;
        }
    }
  }
}
