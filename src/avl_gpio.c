/*=====avl_gpio.c============================================================
 * Author: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/

#include "sapi.h"
#include "avl_gpio.h"

static gpioMap_t openPin;
static gpioMap_t sensorHallPin;

/*
 * @brief   Inicializa las GPIO
 * @param   Pin donde se conecta el solenoide para abrir el equipo
 * @param   Pin donde se conecta el sensor de efecto Hall para detectar el estado del equipo
 * @return  void
 */
void GPIO_Init(gpioMap_t _openPin, gpioMap_t _sensorHallPin)
{
	openPin = _openPin;
	sensorHallPin = _sensorHallPin;
}

/*
 * @brief   Activa la salida para realizar la apertura del equipo
 * @return  TRUE si pudo activar la salida, False en caso contrario
 */
bool_t openDevice()
{
	if ( !gpioWrite ( openPin, HIGH ) ) return FALSE;
	if ( gpioRead ( openPin ) == LOW ) return FALSE;
	else return TRUE;
}

/*
 * @brief   Funcion para detectar si el equipo esta abierto o cerrado
 * @return  TRUE si el equipo esta abierto, FALSE si esta cerrado
 */
bool_t isOpen()
{
	if ( gpioRead ( sensorHallPin ) == HIGH ) return TRUE;
	else return FALSE;
}
