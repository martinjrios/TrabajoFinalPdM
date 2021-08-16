CESE 15Co2021

# Programación de Microcontroladores
## Trabajo Final

Repositorio del Trabajo Final para la materia Programación de Microcontroladores.

Autor: Martín J. Rios - jrios@fi.uba.ar

### Resumen
Menú de configuración de un precinto electrónico para que un usuario, conectado mediante una terminal serie, pueda configurar los parámetros de conexión del equipo (ID del equipo y DNS del servidor), visualizar el estado del equipo (tensión de batería y estado abierto/cerrado) y realizar la apertura activando la salida correspondiente.

+================================================================+
|                      MENU DE CONFIGURACION                     |
+================================================================+

[0]-CONFIGURAR ID
[1]-CONFIGURAR SERVIDOR
[2]-MOSTRAR PARAMETROS CONFIGURADOS
[3]-MOSTRAR ESTADO EQUIPO
[4]-ABRIR
[5]-GUARDAR
[6]-SALIR

+================================================================+
Ingrese el numero de una de las opciones y presione ENTER: 

#### Condiciones de funcionamiento
###### Ejecución
Ejecutar en una terminal serie que soporte los códigos de escape VT100. Por ejemplo, en Linux utilizar GTKTerm.

###### Dependencias externas:
 - sAPI.h

#### Modularización
![](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/imagenes/capas.svg)

###### Archivos de modularización
| File name | hearders                    |
| ------------- | ------------------------------ |
| [avl_main.c](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/src/avl_main.c)|[avl_main.h](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_main.h)|
| [avl_menu.c](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_menu.c)|[avl_menu.h](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_menu.h)|
| [avl_uart.c](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_uart.c)|[avl_uart.h](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_uart.h)|
| [avl_adc.c](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_adc.c)|[avl_adc.h](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_adc.h)|
| [avl_gpio.c](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/src/avl_gpio.c)|[avl_gpio.h](https://github.com/martinjrios/TrabajoFinalPdM/blob/main/inc/avl_gpio.h)|
