/*=====avl_uart.c============================================================
 * Author: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/

#include "../inc/avl_uart.h"
#include "sapi.h"
#include "stdlib.h"
#include "ctype.h"

static uartMap_t menuUart;

void UART_Init(uartMap_t _uart)
{
	menuUart = _uart;
	uartInit ( menuUart, MENU_UART_BAUDRATE );
}

uint8_t UART_waitOption()
{
	char optionAscii;
	char inputLine[MAX_INPUT_LINE];
	uint8_t optionNumber = INVALID_OPTION, len;

	len = UART_ReadLine(inputLine, MAX_INPUT_LINE);

	if(len > 0)
	{
		if(!isalpha(inputLine[0])) optionNumber = atoi(inputLine);
		else optionNumber = INVALID_OPTION;
	}
	else optionNumber = INVALID_OPTION;
	
	return optionNumber;
}

uint8_t UART_ReadLine(char *lineRead, uint8_t maxLength)
{
	uint8_t byteRead, byteCount = 0;
	delay_t uartTimeout;

	if(maxLength < 1) return FALSE;

	delayInit ( &uartTimeout, UART_TIMEOUT);

	while(byteCount < maxLength)
	{
		if(uartReadByte(menuUart, &byteRead))
		{
			if('\n' == byteRead || '\r' == byteRead || '\0' == byteRead) break;
			*lineRead++ = (char) byteRead;
			byteCount++;
		}
		else if(delayRead(&uartTimeout)) break;
	}

	if(byteCount < maxLength) lineRead[byteCount] = '\0'; // Coloco el \0 para finalizar el string
	else lineRead[byteCount-1] = '\0'; // Coloco el \0 para finalizar el string

	delay(1);  // Espero 1 ms para que se termine de cargar en el buffer de la UART lo que haya quedado pendiente de lectura
	//UART_RxFlush();
	uartRxFlush(menuUart); // Limpio lo que haya quedado en el buffer

	return byteCount;
}

void UART_WriteLine(const char *line)
{
	uartWriteString(menuUart, line);
	uartWriteString(menuUart, "\r\n");
}

void UART_Write(const char *str)
{
	uartWriteString(menuUart, str);
}

bool_t UART_Available()
{
	return(uartRxReady(menuUart));
}

void UART_ShowOptions(const char **menuOptions, uint8_t lastOptionIndex)
{
	uint8_t i;
	char strOptionIndex[MAX_STR_OPTION_INDEX];

	for(i = 0; i < lastOptionIndex; i++)
	{
		sprintf(strOptionIndex, "[%d]-", i);
		uartWriteString( menuUart, strOptionIndex);
		UART_WriteLine(menuOptions[i]);
	}
}

void UART_clearTerminal()
{
	uartWriteByte(menuUart, 27); // ESC
	uartWriteString(menuUart, "[2J"); // Clear Screen
}

void UART_cursorHome()
{
	uartWriteByte(menuUart, 27); // ESC
	uartWriteString(menuUart, "[H"); // Cursor to home
}

void UART_RxFlush()
{
   while ( UART_Available() ) {
      uartRxRead(menuUart);
      delay(1);
   }
}
