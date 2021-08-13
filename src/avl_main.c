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
    boardInit(); // INICIALIZAR Y CONFIGURAR PLATAFORMA

    /* Verificación TICK_RATE rango permitido de tiempo: 1 a 50 ms */
    //if ( ( TICK_RATE < TICK_RATE_MIN ) || ( TICK_RATE > TICK_RATE_MAX ) )  blinkError( ERROR_TIME );
    //if ( !tickConfig ( TICK_RATE ) )  blinkError( ERROR_TIME );
    tickConfig ( TICK_RATE );


   // ----- Repeat for ever -------------------------
    while( true )
    {
    	updateMenuFSM();
    }
    return 0;
}
