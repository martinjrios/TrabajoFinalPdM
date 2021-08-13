/*=====menu.h================================================================
 * Authors: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/

#ifndef TPFINALPDM_TRABAJOFINALPDM_INC_AVL_MENU_H_
#define TPFINALPDM_TRABAJOFINALPDM_INC_AVL_MENU_H_

#include "sapi.h"

#define REAL_TERMINAL

#define PASS_LENGTH		10
#define MAX_TEXT		32
#define DEVID_LENGTH	9
#define SERVER_LENGTH	32

typedef enum
{
	START,
	LOCKED,
	MAIN_MENU,
	CONFIG_ID,
	CONFIG_SERVER,
	CONFIG_SIM,
	SHOW_DEVICE_STATUS,
	OPEN,
	SAVE,
	EXIT,
}stateMenu_t;

typedef enum
{
	OPTION_CONFIG_ID = 0,
	OPTION_CONFIG_SERVER,
	OPTION_CONFIG_SIM,
	OPTION_SHOW_DEVICE_STATUS,
	OPTION_OPEN,
	OPTION_SAVE,
	OPTION_EXIT,
}optionMainMenu_t;

typedef enum
{
	OPTION_YES = 0,
	OPTION_NO,
}optionConfirm_t;

void updateMenuFSM();
static void showMainMenu();
static bool_t checkPassword(char *password);

#endif /* TPFINALPDM_TRABAJOFINALPDM_INC_AVL_MENU_H_ */
