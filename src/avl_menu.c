/*=====menu.c================================================================
 * Authors: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/
/**/

#include "../inc/avl_menu.h"
#include "../inc/avl_uart.h"
#include "string.h"

const char MENU_PASSWORD[] = "MENU123";

const char mainMenuHeader[] = "******* MENU DE CONFIGURACION *******";
const char menuFooter[] = "Ingrese el numero de una de las opciones y presione ENTER...";
const char lockedWarning[] = "Menu bloqueado. Ingrese el password para desbloquear...";
const char invalidPassword[] = "Password invalido!";
const char invalidOption[] = "Opcion invalida!";
const char exitMenuText[] = "Saliendo del menu...";
const char confirmExitMenuText[] = "Desea salir del menu?";
const char configIdOptionText[] = "Ingrese el nombre del dispositivo y presione ENTER...";
const char configServerOptionText[] = "Ingrese la IP o DNS del servidor y presione ENTER...";
const char receivedText[] = "Se ha ingresado: ";
const char errorIdText[] = "Formato de ID de dispositivo invalido.";
const char errorText[] = "Ha ocurrido un error. Intente nuevamente...";

const char *OptionsMenu[] =
{
		"CONFIGURAR ID",
		"CONFIGURAR SERVIDOR",
		"CONFIGURAR SIM",
		"MOSTRAR ESTADO EQUIPO",
		"ABRIR",
		"GUARDAR",
		"SALIR"
};

const char *ConfirmOptions[] =
{
		"SI",
		"NO"
};

void updateMenuFSM()
{
	static stateMenu_t stateMenu = START;
	static char devId[DEVID_LENGTH], serverAddress[SERVER_LENGTH];
	char password[PASS_LENGTH], outputLine[MAX_TEXT];
	uint8_t len, menuOption = 0;

	switch(stateMenu)
	{
	case START:
		UART_Init(UART_USB);
#ifdef REAL_TERMINAL
		UART_clearTerminal();
		UART_cursorHome();
#endif
		UART_WriteLine(lockedWarning);
		stateMenu = LOCKED;
		break;

	case LOCKED:
		if(UART_Available())
		{
			if(UART_ReadLine(password, PASS_LENGTH) > 0)
			{
				if(checkPassword(password))
				{
					UART_clearTerminal();
					showMainMenu();
					stateMenu = MAIN_MENU;
				}
				else
				{
					UART_WriteLine(invalidPassword);
				}
			}
		}
		break;

	case MAIN_MENU:
		if(UART_Available())
		{
			menuOption = UART_waitOption();
			sprintf(outputLine, "Opcion ingresada: %d", menuOption);
			UART_WriteLine(outputLine);

			switch((optionMainMenu_t)menuOption)
			{
			case OPTION_CONFIG_ID:
				UART_WriteLine(configIdOptionText);
				stateMenu = CONFIG_ID;
				break;

			case OPTION_CONFIG_SERVER:
				UART_WriteLine(configServerOptionText);
				stateMenu = CONFIG_SERVER;
				break;

			case OPTION_CONFIG_SIM:
				stateMenu = CONFIG_SIM;
				break;

			case OPTION_SHOW_DEVICE_STATUS:
				stateMenu = SHOW_DEVICE_STATUS;
				break;

			case OPTION_OPEN:
				stateMenu = OPEN;
				break;

			case OPTION_SAVE:
				stateMenu = SAVE;
				break;

			case OPTION_EXIT:
				UART_WriteLine(confirmExitMenuText);
				UART_ShowOptions(ConfirmOptions, sizeof(ConfirmOptions)/sizeof(*ConfirmOptions));
				stateMenu = EXIT;
				break;

			default:
				UART_WriteLine(invalidOption);
				break;
			}

		}
		break;

	case CONFIG_ID:
		if(UART_Available())
		{
			len = UART_ReadLine(devId, DEVID_LENGTH);
			if(len == DEVID_LENGTH - 1)
			{
				UART_Write(receivedText);
				UART_WriteLine(devId);
//#ifdef REAL_TERMINAL
//				delay(UART_TIMEOUT);
//#endif
				showMainMenu();
				stateMenu = MAIN_MENU;
			}
			else UART_WriteLine(errorIdText);
		}
		break;

	case CONFIG_SERVER:
		if(UART_Available())
		{
			len = UART_ReadLine(serverAddress, SERVER_LENGTH);
			if(len < SERVER_LENGTH)
			{
				UART_Write(receivedText);
				UART_WriteLine(serverAddress);
#ifdef REAL_TERMINAL
				delay(UART_TIMEOUT);
#endif
				showMainMenu();
				stateMenu = MAIN_MENU;
			}
			else UART_WriteLine(errorText);
		}
		break;

	case CONFIG_SIM:
		break;

	case SHOW_DEVICE_STATUS:
		break;

	case OPEN:
		break;

	case SAVE:
		break;

	case EXIT:
		if(UART_Available())
		{
			menuOption = UART_waitOption();
			if(menuOption != 0xFF)
			{
				switch((optionConfirm_t)menuOption)
				{
				case OPTION_YES:
					UART_clearTerminal();
					UART_cursorHome();
					UART_WriteLine(exitMenuText);
					UART_WriteLine(lockedWarning);
					stateMenu = LOCKED;
					break;
				case OPTION_NO:
					showMainMenu();
					stateMenu = MAIN_MENU;
					break;
				}
			}
		}
		break;

	default:
		stateMenu = START;
		break;
	}
}

static void showMainMenu()
{
#ifdef REAL_TERMINAL
	UART_cursorHome();
#endif
	UART_WriteLine("");
	UART_WriteLine(mainMenuHeader);
	UART_ShowOptions(OptionsMenu, sizeof(OptionsMenu)/sizeof(*OptionsMenu));
	UART_WriteLine(menuFooter);
	UART_WriteLine("");
}

static bool_t checkPassword(char *password)
{
	if(strstr(password, MENU_PASSWORD) != NULL)
	{
		return TRUE;
	}
	else return FALSE;
}
