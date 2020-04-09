// N�mero de cilindros m�ximos.
#define CILINDROS_MAX  4

// Estados de la m�quina mecánica.
#define IDENTIFICA_CILINDRO_EN_FASE_1 1
#define CILINDRO__EN_FASE_1_IDENTIFICADO 2
#define ESPERA_ANGULO_APERTURA_INYECTOR	3
#define ESPERA_ANGULO_INICIO_DWELL_TIME	4
#define DETECTA_COMBUSTION	5
#define SIGUIENTE_CILINDRO  6

// Pulsos que avanza el CKPS, durante el intervalo que CMPS est� a uno (Solo para motores GSI).
#define	PULSOS_CIL_1	12
#define	PULSOS_CIL_2	24
#define	PULSOS_CIL_3	36
#define	PULSOS_CIL_4	48

// Pulsos por revoluci�n CKPS
#define Pulsos_CKPS  48 
