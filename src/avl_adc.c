/*=====avl_adc.c=============================================================
 * Author: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/
#include "sapi.h"
#include "avl_adc.h"

static adcMap_t readBattPin;

/*
 * @brief   Inicializa el ADC
 * @param   Canal del ADC
 * @return  void
 */
void ADC_Init(adcMap_t _adcPin)
{
	adcConfig( ADC_ENABLE ); /* ADC */
	readBattPin = _adcPin;
}

/*
 * @brief   Lee la tension de bateria
 * @return  void
 */
uint16_t ADC_ReadBattVoltage()
{
	uint16_t rawValue, battVoltage;
	float voltageRead;

	rawValue = adcRead(readBattPin);
	voltageRead = ADC_V_MAX * (float)rawValue / ADC_RESOLUTION;
	battVoltage = (uint16_t)(RDIV_COEF * voltageRead * 1000); // En mV

	return battVoltage;
}

