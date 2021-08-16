/*=====avl_gpio.h============================================================
 * Author: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/

#ifndef TPFINALPDM_TRABAJOFINALPDM_INC_AVL_GPIO_H_
#define TPFINALPDM_TRABAJOFINALPDM_INC_AVL_GPIO_H_

void GPIO_Init(gpioMap_t _openPin, gpioMap_t _sensorHallPin);
bool_t openDevice();
bool_t isOpen();

#endif /* TPFINALPDM_TRABAJOFINALPDM_INC_AVL_GPIO_H_ */
