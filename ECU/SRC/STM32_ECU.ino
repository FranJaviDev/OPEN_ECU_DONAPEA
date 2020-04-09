// Recursos.
#include "declaraciones.h"      // Constantes usdadas en el programa.
#include "EntradasSalidas.h"    // Constantes de para la asignación de entradas/salidas.
#include "EstructurasDeDatos.h" // Definicion de las estruturas de datos.

#include "HardwareTimer.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Variables.
stconfiguracion   configuracion;  // Almacena la configuración en la EEPROM que se usa para la gestión.
st_datos_entradas entradas;       // Datos de entrada.
stdatos           datos;          // Contiene el estado actual de todo.
stcalculos        calculos;       // Contiene los resultados de los cálculos.

HardwareTimer contador_CKPS(2);           // Contador para determinar las posiciones del cicgüeñal.
HardwareTimer temporizador_iny(3);        // Temporizador utilizado para controlar la inyección.
HardwareTimer temporizador_dwel_coil(4);  // Timer para la carga y disparo de la bobina de encendido.
HardwareTimer periodo_revolucion(5);      // Timer para medir las RPM

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned long ints = 0;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);

/*
  // Comunicación a la pantalla LCD.
  lcd.begin(20, 4);
  lcd.home (); 
  lcd.setCursor ( 0, 0 );
  lcd.print("  *** ECU  ***");
*/

  // Configura salidas digitales.
  pinMode(inyector_1, OUTPUT);
  pinMode(inyector_2, OUTPUT);
  pinMode(inyector_3, OUTPUT);
  pinMode(inyector_4, OUTPUT);
  pinMode(spark_1, OUTPUT);
  pinMode(spark_2, OUTPUT);

  pinMode(CMPS,INPUT);
  pinMode(TPS,INPUT);
  pinMode(CT,INPUT);
  
  digitalWrite(inyector_1,LOW);
  digitalWrite(inyector_2,LOW);
  digitalWrite(inyector_3,LOW);
  digitalWrite(inyector_4,LOW);
  digitalWrite(spark_1,LOW);
  digitalWrite(spark_2,LOW);

  // Estados iniciales del programa.
  datos.maquina_de_estados_mecanica=IDENTIFICA_CILINDRO_EN_FASE_1;
  datos.numero_de_cilindro_activo=0;
  
  // Configura entradas.
     
  // Configuración del contador rápido conectado al cigüeñal.
  pinMode(D2, INPUT_PULLUP);  //channel A
  pinMode(D3, INPUT_PULLUP);  //channel B
  
  // Encoder CKPS.
  contador_CKPS.setMode(1, TIMER_ENCODER); //set mode, the channel is not used when in this mode (but it must be [1..4]). 
  contador_CKPS.pause(); //stop... 
  contador_CKPS.setPrescaleFactor(1); //normal for encoder to have the lowest or no prescaler. 
  contador_CKPS.setOverflow(Pulsos_CKPS);    //use this to match the number of pulse per revolution of the encoder. Most industrial use 1024 single channel steps. 
  contador_CKPS.setCount(0);          //reset the counter. 
  contador_CKPS.setEdgeCounting(TIMER_SMCR_SMS_ENCODER3); //or TIMER_SMCR_SMS_ENCODER1 or TIMER_SMCR_SMS_ENCODER2. This uses both channels to count and ascertain direction. 
  contador_CKPS.attachInterrupt(0, INT_contador_CKPS); //channel must be 0 here 
  contador_CKPS.resume();                 //start the encoder... 

  // Interrupción por cambio de la señal del árbol de levas.
  attachInterrupt(digitalPinToInterrupt(CMPS),INT_CMPS,CHANGE);
  
};


void loop() {

};

//*****************************************************
// Interrupción por sensor de posición árbol de levas.
//*****************************************************
void INT_CMPS(){    
    
    /* Con cada flanco positivo incrementamos el cilindro activo.
     * 
     * Si el número de cilindro es 0 significa que no tenemos identificado el cilnidro que toca. 
     * 
     * Para identificar el cilindro al inicio del arranque, el motor que tenemos 
     * emplea un sistema de codificación en el arbol de levas.
     * Cada rotación del arbol está dividida en 4 pulsos de diferente longitud.
     * La longitud de cada pulso se mide contando los pulsos que da el cigüeñal mientras el pulso está a nivel alto. 
     * Mediante éste sistema identificamos el número de cilindro activo, y por tanto, la secuencia del motor.
     */    
    datos.cuenta_actual_encoder_CKPS=contador_CKPS.getCount();
    if(datos.maquina_de_estados_mecanica!=IDENTIFICA_CILINDRO_EN_FASE_1){
        // Siguiente cilindro de la secuencia.        
        if(digitalRead(CMPS)==HIGH){                            
          calcula_rpm();                  
          switch(datos.numero_de_cilindro_activo){
              case 1:
                datos.numero_de_cilindro_activo=4;
                programa_posicion_encendido();
                inyecta();
              break;

              case 2:
                datos.numero_de_cilindro_activo=3;
                programa_posicion_encendido();
                inyecta();
              break;

              case 3:
                datos.numero_de_cilindro_activo=1;
                programa_posicion_encendido();
                inyecta();
              break;

              case 4:
                datos.numero_de_cilindro_activo=2;
                programa_posicion_encendido();
                inyecta();
              break;              
           };   
        };
      }else{
        // Identificamos el cilindro.
        if(digitalRead(CMPS)==HIGH){
            // Si estamos en flanco ascendente empezamos a contar los pulsos de cigüeñal    
            datos.cuenta_anterior_CKPS_identificar_cilindro=datos.cuenta_actual_encoder_CKPS;
          }else{
            // Si es el flanco descendente apuntamos la cuenta de pulsos.
            datos.pulsos_cilindro_actual=datos.cuenta_actual_encoder_CKPS-datos.cuenta_anterior_CKPS_identificar_cilindro;
            Serial.print("\n Pulsos: ");
            Serial.print(datos.pulsos_cilindro_actual);
            // e identificamos el cilindro activo por los pulsos anotados.
            if(datos.pulsos_cilindro_actual=PULSOS_CIL_1){
              datos.numero_de_cilindro_activo= 1;
              datos.maquina_de_estados_mecanica=CILINDRO__EN_FASE_1_IDENTIFICADO;
            };
            if(datos.pulsos_cilindro_actual=PULSOS_CIL_2){
              datos.numero_de_cilindro_activo= 2;
              datos.maquina_de_estados_mecanica=CILINDRO__EN_FASE_1_IDENTIFICADO;
            };
            if(datos.pulsos_cilindro_actual=PULSOS_CIL_3){              
              datos.numero_de_cilindro_activo= 3;
              datos.maquina_de_estados_mecanica=CILINDRO__EN_FASE_1_IDENTIFICADO;
            };
            if(datos.pulsos_cilindro_actual=PULSOS_CIL_4){
              datos.numero_de_cilindro_activo= 4;
              datos.maquina_de_estados_mecanica=CILINDRO__EN_FASE_1_IDENTIFICADO;
            };            
          };     
     };
  };

void INT_contador_CKPS(){
    
       switch(datos.numero_de_cilindro_activo){
         case 1:
           digitalWrite(spark_1,HIGH);      
         break;
         case 2:
           digitalWrite(spark_2,HIGH);               
         break;
         case 3:
           digitalWrite(spark_3,HIGH);  
         break;
         case 4:
           digitalWrite(spark_4,HIGH);  
         break;
         default:
        
         break;
      };                       
           
      // Configuramos el timer para generar una interrupción cuando termine el tiempo de carga de la bobina de encendido. 
      temporizador_dwel_coil.setChannel1Mode(TIMER_OUTPUTCOMPARE);
      temporizador_dwel_coil.pause();
      temporizador_dwel_coil.setPeriod(1000); // in microseconds
      temporizador_dwel_coil.setOverflow(10000000);
      temporizador_dwel_coil.attachCompare1Interrupt(INT_temporizador_dwel_coil);
      temporizador_dwel_coil.setCount(0);
      temporizador_dwel_coil.refresh();
      temporizador_dwel_coil.resume();
};

void INT_temporizador_dwel_coil(){

        digitalWrite(spark_1,LOW);  
        digitalWrite(spark_2,LOW);  
        digitalWrite(spark_3,LOW);  
        digitalWrite(spark_4,LOW);                          
  };

void INT_Overlow_timer_rpm(){
  
  };

void calcula_rpm(){
     int periodo;

     periodo=periodo_revolucion.getCount();     
     periodo_revolucion.setCount(0);
     entradas.RPM=(60*1000000/(float)periodo);
       
  };

void inyecta(){
    // Abrimos el inyector.    
    switch(datos.numero_de_cilindro_activo){
        case 1:
          digitalWrite(inyector_1,HIGH);      
        break;
        case 2:
          digitalWrite(inyector_2,HIGH);               
        break;
        case 3:
          digitalWrite(inyector_3,HIGH);  
        break;
        case 4:
          digitalWrite(inyector_4,HIGH);  
        break;
        default:
        
        break;
      };    
                     
    // Configuramos el timer para generar una interrupción cuando termine el tiempo de apertura del inyector. 
    temporizador_iny.setChannel1Mode(TIMER_OUTPUTCOMPARE);
    temporizador_iny.pause();
    temporizador_iny.setPeriod(100000); // in microseconds
    temporizador_iny.setOverflow(100000);
//    temporizador_iny.setPeriod(calculos.tiempo_inyeccion[datos.numero_de_cilindro_activo]); // in microseconds
    temporizador_iny.attachCompare1Interrupt(INT_temporizador_cierra_iny);
    temporizador_iny.setCount(0);
    temporizador_iny.refresh();
    temporizador_iny.resume();  
  };

void INT_temporizador_cierra_iny(){    
    digitalWrite(inyector_1,LOW);
    digitalWrite(inyector_2,LOW);
    digitalWrite(inyector_3,LOW);
    digitalWrite(inyector_4,LOW);
    temporizador_iny.pause();
  };


void programa_posicion_encendido(){

  INT_contador_CKPS();

/*  contador_CKPS.setMode(1, TIMER_ENCODER); //set mode, the channel is not used when in this mode (but it must be [1..4]). 
  contador_CKPS.pause(); //stop... 
  contador_CKPS.setPrescaleFactor(1); //normal for encoder to have the lowest or no prescaler. 
  contador_CKPS.setOverflow(Pulsos_CKPS);    //use this to match the number of pulse per revolution of the encoder. Most industrial use 1024 single channel steps. 
  contador_CKPS.setCount(0);          //reset the counter. 
  contador_CKPS.setEdgeCounting(TIMER_SMCR_SMS_ENCODER3); //or TIMER_SMCR_SMS_ENCODER1 or TIMER_SMCR_SMS_ENCODER2. This uses both channels to count and ascertain direction. 
  contador_CKPS.attachInterrupt(0, INT_contador_CKPS); //channel must be 0 here 
  contador_CKPS.resume();                 //start the encoder... 
  */ 
  };

/* Calcula mediante una itnerpolación lineal el valor que corresponde al eje z 
    dentro del intérvalo de valores de los ejex x e y.
    */
float calcula_z_mapa(float x, float y,stmapa mapa){

    int index_x,x_inferior, x_superior,z_x_inferior, z_x_superior;
    int index_y,y_inferior, y_superior,z_y_inferior, z_y_superior;
    
    float valor_z_x_max_y_min, valor_z_x_max_y_max, valor_interpolado_z_y_max;
    float valor_z_x_min_y_min, valor_z_x_min_y_max, valor_interpolado_z_y_min; 
    float valor_z_y_max, valor_z_y_min, valor_interpolado_z;
           
    // Localizo el valor máximo y el valor mínimo de x que pertenece a ese intérvalo.
    for(index_x=0;index_x<(DIVISIONES_X_MAPA);index_x++){
      if(x<=mapa.valores_divisiones_mapa_eje_x[index_x]){        
        break;
      };       
    };
    x_inferior=mapa.valores_divisiones_mapa_eje_x[index_x-1];  // ***ACOTAR ESTO <-
    x_superior=mapa.valores_divisiones_mapa_eje_x[index_x];

    // Localizo el valor máximo y el valor mínimo de x que pertenece a ese intérvalo.
    for(index_y=0;index_y<(DIVISIONES_Y_MAPA);index_y++){
      if(y<=mapa.valores_divisiones_mapa_eje_y[index_y]){
        break;
      };       
    };
    y_inferior=mapa.valores_divisiones_mapa_eje_y[index_y-1];  // ***ACOTAR ESTO <-
    y_superior=mapa.valores_divisiones_mapa_eje_y[index_y];

    valor_z_x_min_y_min = mapa.cartografia[index_y-1][index_x-1];
    valor_z_x_max_y_min = mapa.cartografia[index_y-1][index_x];
    valor_interpolado_z_y_min=((valor_z_x_max_y_min-valor_z_x_min_y_min)/(x_superior-x_inferior))*(x-x_inferior)+valor_z_x_min_y_min;
       
    valor_z_x_min_y_max = mapa.cartografia[index_y][index_x-1];
    valor_z_x_max_y_max = mapa.cartografia[index_y][index_x];
    valor_interpolado_z_y_max=((valor_z_x_max_y_max-valor_z_x_min_y_max)/(x_superior-x_inferior))*(x-x_inferior)+valor_z_x_min_y_max;

    valor_interpolado_z = ((valor_interpolado_z_y_max-valor_interpolado_z_y_min)/(y_superior-y_inferior))*(y-y_inferior)+valor_interpolado_z_y_min;
   
  }
  
