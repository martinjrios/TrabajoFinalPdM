/*=====avl_adc.h=============================================================
 * Author: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/

#ifndef TPFINALPDM_TRABAJOFINALPDM_INC_AVL_ADC_H_
#define TPFINALPDM_TRABAJOFINALPDM_INC_AVL_ADC_H_

#define RDIV_COEF		1
#define ADC_RESOLUTION	1024
#define ADC_V_MAX		3.3

void ADC_Init(adcMap_t _adcPin);
uint16_t ADC_ReadBattVoltage();

#endif /* TPFINALPDM_TRABAJOFINALPDM_INC_AVL_ADC_H_ */
