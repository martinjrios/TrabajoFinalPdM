/*=====uart.h================================================================
 * Author: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/
#ifndef TPFINALPDM_TRABAJOFINALPDM_INC_AVL_UART_H_
#define TPFINALPDM_TRABAJOFINALPDM_INC_AVL_UART_H_

#include "sapi.h"

#define MENU_UART_BAUDRATE		115200
#define UART_TIMEOUT			20000 // ms

#define MAX_STR_OPTION_INDEX	6
#define MAX_INPUT_LINE			3

#define INVALID_OPTION			0xFF

// Comandos para el manejo de la consola
#define ESC						0x1b  // ESC character
#define CLEAR_SCREEN			"[2J" // Clear all screen
#define CLEAR_SCREEN_DOWN		"[J"  // Clear screen from cursor down
#define CLEAR_LINE_RIGHT		"[0K" // Clear line from cursor right
#define CURSOR_HOME				"[H"  // Move cursor home
#define GET_CURSOR_POS			"[6n" // Get cursor position
#define MOVE_DOWN				"M"	// Move/scroll window down one line


//FUNCIONES PARA LA UART
void UART_Init(uartMap_t _uart);
uint8_t UART_readOption();
uint8_t UART_ReadLine(char *lineRead, uint8_t maxLength);
void UART_WriteLine(const char *line);
void UART_Write(const char *str);
bool_t UART_Available();
void UART_ShowOptions(const char **menuOptions, uint8_t lastOptionIndex);
void UART_clearTerminal();
void UART_cursorHome();
void UART_getCursorPosition();
void UART_setCursorPosition(uint8_t v, uint8_t h);
void UART_moveCursorNDown(uint8_t n);
void UART_sendTerminalCommand(const char *command);

#endif /* TPFINALPDM_TRABAJOFINALPDM_INC_AVL_UART_H_ */
