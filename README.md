# OPEN_ECU_DONAPEA
An Arduino based, STM32 Engine Control Unit. 

El proyecto partió originalmente de el profesor de la asignatura de motores Javier Zamora, quien ideo una pequeña centralita 
para la gestión de inyección de una motocicleta, que tuvo una gran acogida por parte del alumnado.

Debido a la acogida por parte del alumnado, a las posibilidades de trabajo interdisciplinares que posee, así como 
las posibilidades para facilitar la asimilación por parte de los alumnos, de conceptos teóricos trabajados durante el módulo.

He decidio rediseñar todo el hardware y el software, para desarrollar una unidad de control para un motor inyección de 4 cilidros.

El objetivo es ir incorporando diferentes estrategias de control de motor en función de que se estudien, así como posibilitar 
la adaptación para trabajar con diferentes combustibles como GLP, GNC ó Hidrógeno. 

Facilitando así la asimilación de conceptos expuestos tanto en la asigntura de motores como de vehículos híbridos.

Finalmente, posibilitará el marco de desarrollo sobre el cual los alumnos podrán ir desarrollando mejoras mediante trabajos de fin de
ciclo.

V1.0- PRIMERA VERSIÓN LIBRE.

El código sigue las premisas de simplicidad y claridad sin entrar en detalles técnicos de la arquitectura STM32. 
Para ello se ha utilizado el entorno de desarrollo abierto de Arduino.

Gestión de inyección de combustible básica. La estrategia de cálculo se basa simplemente en un mapa que relacciona el tiempo de inyección con las revoluciones y la apertura de la mariposa de gases.

Gestión de encendido básica. Encendido de chispa perdida (DIS). La estrategia igualmente se basa en un mapa que relacciona ángulo con 
revoluciones y posición de mariposa.


Jueves 09/04/2020
Fran.
