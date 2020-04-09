// Definici�n de dimensiones de mapas. (Pese a que una definici�n la mantengo aqu� porque es m�s practico).
#define DIVISIONES_X_MAPA 10
#define DIVISIONES_Y_MAPA 10
// Definici�n de mapa 1.
struct stmapa{  
  float valores_divisiones_mapa_eje_x[DIVISIONES_X_MAPA]={0,1,2,3,4,5,6,7,8,9};
  float valores_divisiones_mapa_eje_y[DIVISIONES_Y_MAPA]={0,1,2,3,4,5,6,7,8,9};
  float cartografia[DIVISIONES_Y_MAPA][DIVISIONES_X_MAPA]={
                                                          { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                                                          {10,11,12,13,14,15,16,17,18,19},
                                                          {20,21,22,23,24,25,26,27,28,29},
                                                          {30,31,32,33,34,35,36,37,38,39},
                                                          {40,41,42,43,44,45,46,47,48,49},
                                                          {50,51,52,53,54,55,56,57,58,59},
                                                           };
  }mapa_1;

  
// Configuraci�n.
struct stconfiguracion{
	int rpm_corte;
	int rpm_ralenti;  
	stmapa mapa_encendido;
	stmapa mapa_inyeccion;	
};

struct st_datos_entradas{
	float RPM;
	float MAF;
	float MAP;
	float POT;
	float IAT;
	float CT;
};

struct stdatos{
	int maquina_de_estados_mecanica;
	int maquina_de_estados_motor;
	int numero_de_cilindro_activo;
	int cuenta_actual_encoder_CKPS;
	int cuenta_anterior_CKPS_identificar_cilindro;
	int pulsos_cilindro_actual;
};
   
struct stcalculos{
	float carga_cil[CILINDROS_MAX];
	float tiempo_inyeccion[CILINDROS_MAX]={7,7,7,7};
  float angulo_encendido[CILINDROS_MAX]={20,20,20,20}; 
};
