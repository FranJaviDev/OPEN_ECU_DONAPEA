// PROGRAMA PARA SIMULAR LA MECÁNICA DE UN MOTOR OPEL ASTRA GSI.
#define CKPS PD2
#define CMPS PD3

#define POT_VEL A6 
#define PULSOS_POR_CILINDRO (48)
#define PULSOS_POR_CILINDRO_1 (48/4)*1
#define PULSOS_POR_CILINDRO_2 (48/4)*2
#define PULSOS_POR_CILINDRO_3 (48/4)*3
#define PULSOS_POR_CILINDRO_4 (48/4)*4

#define CILINDROS 4
//#define DELAY delayMicroseconds(10000);
#define DELAY delay(velocidad);
#define LECTURA velocidad=analogRead(POT_VEL)/10;

void setup() {
  // put your setup code here, to run once:
  pinMode(CKPS, OUTPUT)  ;
  pinMode(CMPS, OUTPUT)  ;

  digitalWrite(CKPS,LOW);
  digitalWrite(CMPS,LOW);
};

void loop() {

  int i=0,velocidad=0;
  
  // put your main code here, to run repeatedly:
  while(1){
    
    LECTURA
    
    digitalWrite(CMPS, HIGH);
    for(i=0;i<PULSOS_POR_CILINDRO_1;i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };
    digitalWrite(CMPS, LOW);
    for(i=0;i<(PULSOS_POR_CILINDRO-PULSOS_POR_CILINDRO_1);i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };

    digitalWrite(CMPS, HIGH);
    for(i=0;i<PULSOS_POR_CILINDRO_2;i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };
    digitalWrite(CMPS, LOW);
    for(i=0;i<(PULSOS_POR_CILINDRO-PULSOS_POR_CILINDRO_2);i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };

    digitalWrite(CMPS, HIGH);
    for(i=0;i<PULSOS_POR_CILINDRO_3;i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };
    digitalWrite(CMPS, LOW);
    for(i=0;i<(PULSOS_POR_CILINDRO-PULSOS_POR_CILINDRO_3);i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };

    digitalWrite(CMPS, HIGH);
    for(i=0;i<PULSOS_POR_CILINDRO_4;i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };
    digitalWrite(CMPS, LOW);
    for(i=0;i<(PULSOS_POR_CILINDRO-PULSOS_POR_CILINDRO_4);i++){
      digitalWrite(CKPS,HIGH);
      DELAY
      LECTURA
      digitalWrite(CKPS, LOW);
      DELAY
      LECTURA
    };
  };
  
};
