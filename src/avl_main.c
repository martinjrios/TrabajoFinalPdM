/*=====avl_main.c============================================================
 * Authors: Martin Rios <jrios@fi.uba.ar>
 * Date: 2021/08/10
 * Version: 1.0
 *===========================================================================*/
/**/

#include "sapi.h"
#include "../inc/avl_main.h"
#include "../inc/avl_menu.h"

/* FUNCION PRINCIPAL */
int main( void )
{
	stateMain_t stateMain = MENU;
	uint8_t i = 0;  // Para mostrar que se esta realizando una tarea

    boardInit(); // INICIALIZAR Y CONFIGURAR PLATAFORMA

    /* Verificación TICK_RATE rango permitido de tiempo: 1 a 50 ms */
    if ( ( TICK_RATE < TICK_RATE_MIN ) || ( TICK_RATE > TICK_RATE_MAX ) )  printf("ERROR: Configuracion del tick fuera de rango\r\n");
    if ( !tickConfig ( TICK_RATE ) )  printf("ERROR: No se pudo configurar tick\r\n");

   // ----- Repeat for ever -------------------------
    while( true )
    {
    	switch(stateMain)
    	{
    	case MENU:
    		if(!updateMenuFSM())
    		{
    			printf("\r\nSe ha salido del MENU.\r\n");
    			printf("Ejecutando otras tareas...\r\n");
    			stateMain = OTHER_TASKS;
    		}
    		break;
    	case OTHER_TASKS:
    		printf("%d\r\n", i);
    		i++,
			delay(1000);
    		break;
    	}
    }
    return 0;
}
