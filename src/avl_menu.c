/*=====menu.c================================================================
 * Authors: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/
/**/

#include "../inc/avl_menu.h"
#include "../inc/avl_uart.h"
#include "../inc/avl_adc.h"
#include "../inc/avl_gpio.h"
#include "string.h"

static const char MENU_PASSWORD[] = "MENU123";

static const char menuHeader[] = "+================================================================+";
static const char mainMenuHeader[] = "|                      MENU DE CONFIGURACION                     |";
static const char showParametersHeader[] = "|                     PARAMETROS CONFIGURADOS                    |";
static const char exitHeader[] = "|                        SALIR DEL MENU?                         |";
static const char stateHeader[] = "|                        ESTADO DEL EQUIPO                       |";
static const char openHeader[] = "|                       ABRIR EL EQUIPO?                         |";
static const char configIdOptionText[] = "|      Ingrese el nombre del dispositivo y presione ENTER...     |";
static const char configServerOptionText[] = "|       Ingrese la IP o DNS del servidor y presione ENTER...     |";
static const char mainMenuFooter[] = "Ingrese el numero de una de las opciones y presione ENTER: ";
static const char showParametersFooter[] = "Presione 0 + ENTER para volver al menu principal: ";
static const char subMenuFooter[] = "Presione 0 para volver al menu principal o 1 para igresar nuevamente el texto: ";
static const char lockedWarning[] = "Menu bloqueado. Ingrese el password para desbloquear: ";
static const char invalidPassword[] = "Password invalido!";
static const char invalidOption[] = "Opcion invalida!";
static const char exitMenuText[] = "Saliendo del menu...";
static const char receivedText[] = "Se ha ingresado: ";
static const char errorIdText[] = "Formato de ID de dispositivo invalido.";
static const char errorText[] = "Ha ocurrido un error. Intente nuevamente...";
static const char emptyID[] = "No se ha asignado un ID al equipo";
static const char emptyServer[] = "No se ha asignado un servidor al equipo";

static const char *OptionsMenu[] =
{
		"CONFIGURAR ID",
		"CONFIGURAR SERVIDOR",
		"MOSTRAR PARAMETROS CONFIGURADOS",
		"MOSTRAR ESTADO EQUIPO",
		"ABRIR",
		"GUARDAR",
		"SALIR"
};

static const char *ConfirmOptions[] =
{
		"SI",
		"NO"
};

static char devId[DEVID_LENGTH];
static char serverAddress[SERVER_LENGTH];

/*
 * @brief   Actualiza la maquina de estados del Menu
 * @return  FALSE cuando se ingresa la opcion salir
 */
bool_t updateMenuFSM()
{
	static stateMenu_t stateMenu = START;
	static stateSubmenu_t stateSubmenu = WAITING_INPUT;
	char password[PASS_LENGTH], outputLine[MAX_TEXT];
	uint8_t len, menuOption = 0;
	static delay_t delayStatus;
	bool_t ret = TRUE;

	switch(stateMenu)
	{
	case START: // Estado inicial
		// Inicializacion de perifericos
		UART_Init(UART_USB);
		GPIO_Init(LED1, TEC1);
		ADC_Init(CH1);

		UART_clearTerminal();
		UART_cursorHome();
		UART_Write(lockedWarning);
		stateMenu = LOCKED;
		break;

	case LOCKED: // Menu bloqueado
		if(UART_Available())
		{
			if(UART_ReadLine(password, PASS_LENGTH) > 0)
			{
				if(checkPassword(password))
				{
					showMainMenu();
					stateMenu = MAIN_MENU;
				}
				else
				{
					UART_clearTerminal();
					UART_cursorHome();
					UART_WriteLine(invalidPassword);
					UART_Write(lockedWarning);
				}
			}
		}
		break;

	case MAIN_MENU: // Menu principal
		if(UART_Available())
		{
			menuOption = UART_readOption();
			sprintf(outputLine, "%d\r\n", menuOption);

			switch((optionMainMenu_t)menuOption)
			{
			case OPTION_CONFIG_ID:
				showMenu(configIdOptionText, NULL, NULL, 4);
				UART_setCursorPosition(OPTIONS_START_Y_POS,OPTIONS_START_X_POS);
				stateMenu = CONFIG_ID;
				break;

			case OPTION_CONFIG_SERVER:
				showMenu(configServerOptionText, NULL, NULL, 3);
				UART_setCursorPosition(OPTIONS_START_Y_POS,OPTIONS_START_X_POS);
				stateMenu = CONFIG_SERVER;
				break;

			case OPTION_SHOW_PARAMETERS:
				showMenu(showParametersHeader, showParametersFooter, NULL, 2);
				UART_setCursorPosition(OPTIONS_START_Y_POS,OPTIONS_START_X_POS);

				if(devId[0] != '\0')
				{
					UART_Write("ID del equipo: "); UART_WriteLine(devId);
				}
				else UART_WriteLine(emptyID);
				if(serverAddress[0] != '\0')
				{
					UART_Write("Servidor: "); UART_WriteLine(serverAddress);
				}
				else UART_WriteLine(emptyServer);
				UART_moveCursorNDown(3);

				stateMenu = SHOW_PARAMETERS;
				break;

			case OPTION_SHOW_DEVICE_STATUS:
				delayConfig(&delayStatus, STATUS_REFRESH_TIME);
				showMenu(stateHeader, showParametersFooter, NULL, 2);
				stateMenu = SHOW_DEVICE_STATUS;
				break;

			case OPTION_OPEN:
				showMenu(openHeader, mainMenuFooter, ConfirmOptions, sizeof(ConfirmOptions)/sizeof(*ConfirmOptions));
				stateMenu = OPEN;
				break;

			case OPTION_SAVE:
				stateMenu = SAVE;
				break;

			case OPTION_EXIT:
				showMenu(exitHeader, mainMenuFooter, ConfirmOptions, sizeof(ConfirmOptions)/sizeof(*ConfirmOptions));
				stateMenu = EXIT;
				break;

			default:
				UART_sendTerminalCommand(CLEAR_LINE);
				UART_WriteLine(invalidOption);
				break;
			}
		}
		break;

	case CONFIG_ID: // Configuracion del ID del equipo
		switch(stateSubmenu)
		{
		case WAITING_INPUT: // Esperando que se ingrese el texto
			if(UART_Available())
			{
				len = UART_ReadLine(devId, DEVID_LENGTH);
				if(len == DEVID_LENGTH - 1)  // Se muestra lo ingresado
				{
					UART_setCursorPosition(OPTIONS_START_Y_POS,OPTIONS_START_X_POS);
					UART_Write(receivedText);
					UART_WriteLine(devId);
					UART_Write(subMenuFooter);
					stateSubmenu = WAITING_OPTION;
				}
				else // Se muestra que hubo un error
				{
					UART_sendTerminalCommand(CLEAR_LINE);
					UART_sendTerminalCommand(MOVE_NEXT_LINE);
					UART_sendTerminalCommand(CLEAR_LINE);
					UART_WriteLine(errorIdText);
					UART_moveCursorNUp(2);
				}
			}
			break;
		case WAITING_OPTION:  // Esperando que se ingrese la opcion
			if(UART_Available())
			{
				menuOption = UART_readOption();
				if(menuOption != INVALID_OPTION) // Si es una opcion valida se procede a ejecutar la opcion
				{
					switch((optionConfirm_t)menuOption)
					{
					case OPTION_YES:
						UART_clearTerminal();
						showMainMenu();
						stateSubmenu = WAITING_INPUT;
						stateMenu = MAIN_MENU;
						break;
					case OPTION_NO:
						UART_sendTerminalCommand(CLEAR_LINE);
						UART_moveCursorNUp(1);
						UART_sendTerminalCommand(CLEAR_LINE);
						stateSubmenu = WAITING_INPUT;
						break;
					}
				}
			}
			break;
		}
		break;

	case CONFIG_SERVER:  // Configuracion del Servidor
		switch(stateSubmenu)
		{
		case WAITING_INPUT: // Esperando que se ingrese el texto
			if(UART_Available())
			{
				len = UART_ReadLine(serverAddress, SERVER_LENGTH);
				if(len < SERVER_LENGTH - 1)  // Se muestra lo ingresado
				{
					UART_setCursorPosition(OPTIONS_START_Y_POS,OPTIONS_START_X_POS);
					UART_Write(receivedText);
					UART_WriteLine(serverAddress);
					UART_Write(subMenuFooter);
					stateSubmenu = WAITING_OPTION;
				}
				else // Se muestra que hubo un error
				{
					UART_sendTerminalCommand(CLEAR_LINE);
					UART_sendTerminalCommand(MOVE_NEXT_LINE);
					UART_sendTerminalCommand(CLEAR_LINE);
					UART_WriteLine(errorText);
					UART_moveCursorNUp(2);
				}
			}
			break;
		case WAITING_OPTION: // Esperando que se ingrese la opcion
			if(UART_Available())
			{
				menuOption = UART_readOption();
				if(menuOption != INVALID_OPTION)
				{
					switch((optionConfirm_t)menuOption)
					{
					case OPTION_YES:
						UART_clearTerminal();
						showMainMenu();
						stateSubmenu = WAITING_INPUT;
						stateMenu = MAIN_MENU;
						break;
					case OPTION_NO:
						UART_sendTerminalCommand(CLEAR_LINE);
						UART_moveCursorNUp(1);
						UART_sendTerminalCommand(CLEAR_LINE);
						stateSubmenu = WAITING_INPUT;
						break;
					}
				}
			}
			break;
		}
		break;

	case SHOW_PARAMETERS:  // Se muestran los parametros configurados
		if(UART_Available())
		{
			menuOption = UART_readOption();
			if(menuOption != INVALID_OPTION)
			{
				switch((optionConfirm_t)menuOption)
				{
				case OPTION_YES:  // Se sale al menu principal
					UART_clearTerminal();
					showMainMenu();
					stateMenu = MAIN_MENU;
					break;
				case OPTION_NO: // Sigue mostrando los parametros configurados
					break;
				}
			}
		}
		break;

	case SHOW_DEVICE_STATUS:  // Se muestra el estado del equipo: Tension de bateria y si esta abierto o cerrado
		if(UART_Available())
		{
			menuOption = UART_readOption();
			if(OPTION_YES == menuOption)  // Si se ingreso la opcion de salir se sale al menu principal
			{
				showMainMenu();
				stateMenu = MAIN_MENU;
			}
		}
		else if(delayRead(&delayStatus))  // Muestra el estado del equipo
		{
			UART_setCursorPosition(OPTIONS_START_Y_POS,OPTIONS_START_X_POS);
			UART_sendTerminalCommand(CLEAR_LINE_RIGHT);
			sprintf(outputLine, "Tension Bateria: %u mV", ADC_ReadBattVoltage());
			UART_WriteLine(outputLine);
			UART_sendTerminalCommand(CLEAR_LINE_RIGHT);
			UART_Write("Estado: ");
			if(isOpen()) UART_WriteLine("Abierto");
			else UART_WriteLine("Cerrado");
			UART_moveCursorNDown(3);
		}
		break;

	case OPEN:  // Para abrir el equipo
		if(UART_Available())
		{
			menuOption = UART_readOption();
			if(menuOption != INVALID_OPTION)
			{
				switch((optionConfirm_t)menuOption)
				{
				case OPTION_YES:
					openDevice();
					showMainMenu();
					stateMenu = MAIN_MENU;
					break;
				case OPTION_NO:
					showMainMenu();
					stateMenu = MAIN_MENU;
					break;
				}
			}
		}
		break;

	case SAVE: // Guarda en memoria no volatil los parametros configurados
		// TO DO: Guardar en EEPROM los parametros configurados
		UART_clearTerminal();
		showMainMenu();
		stateMenu = MAIN_MENU;
		break;

	case EXIT:  // Sale del menu principal
		if(UART_Available())
		{
			menuOption = UART_readOption();
			if(menuOption != INVALID_OPTION)
			{
				switch((optionConfirm_t)menuOption)
				{
				case OPTION_YES:
					UART_clearTerminal();
					UART_cursorHome();
					UART_WriteLine(exitMenuText);
					stateMenu = LOCKED;
					ret = FALSE;
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

	return ret;
}

/*
 * @brief   Muestra el menu principal en la terminal serie
 */
static void showMainMenu()
{
	showMenu(mainMenuHeader, mainMenuFooter, OptionsMenu,  sizeof(OptionsMenu)/sizeof(*OptionsMenu));
}

/*
 * @brief   Muestra en la terminal serie un menu con sus opciones
 * @param   El texto del titulo del menu
 * @param	El texto de las indicaciones para comandar el menu
 * @param	Las opciones que posee el menu
 * @param	La cantidad de opciones que posee el menu
 */
static void showMenu(const char *menuText, const char *menuFooter, const char **options, uint8_t nrOptions)
{
	uint8_t i;

	UART_clearTerminal();
	UART_cursorHome();
	UART_WriteLine("");
	UART_WriteLine(menuHeader);
	UART_WriteLine(menuText);
	UART_WriteLine(menuHeader);
	UART_WriteLine("");
	if(options != NULL)  UART_ShowOptions(options, nrOptions);
	else
	{
		for(i = 0; i < nrOptions; i++)	UART_WriteLine(""); // agrego espacio para mostrar la informacion
	}
	UART_WriteLine("");
	UART_WriteLine(menuHeader);
	if(menuFooter != NULL) UART_Write(menuFooter);
}

/*
 * @brief   Valida si la clave ingresada es correcta
 * @param   La clave a validar
 * @return  TRUE si la clave es correcta
 */
static bool_t checkPassword(char *password)
{
	if(strstr(password, MENU_PASSWORD) != NULL)
	{
		return TRUE;
	}
	else return FALSE;
}
