
 /* =============================================================================
 /* Fecha de creacion: 2017-03-30    Gastón Daniel Zapata
 /* =============================================================================

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();
        
   uartConfig( UART_USB, 115200 ); // Inicializar la UART2 a 115200 baudios  
   
   // --------configuramos el delay no bloqueante---------------
   int tiempo_parpadeo=0;
   delay_t delay_parpadeo;
   delayConfig(&tiempo_parpadeo,tiempo_parpadeo);
   //---------------------------------------------------------
	int tecla_1; //variables para el estado de las teclas
	int tecla_2;
	int tecla_3;
	int tecla_4;
	
	int led_B;//variables para el estado de los leds
	int led_1;
	int led_2;
	int led_3;	
	
	gpioWrite( LEDB, OFF );//apago todos los leds para asegurarme que comiencen apagados
	gpioWrite( LED1, OFF );
	gpioWrite( LED2, OFF );
	gpioWrite( LED3, OFF );
	
	led_B = 0;// inicio el estado de los leds en cero
	led_1 = 0;
	led_2 = 0;
	led_3 = 0;
	
	uint8_t byteRecibido; // variable donde recibo caracteres de la compu
          
   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {    
   //-------------Lectura de los estados de las teclas------------
	tecla_1 = !gpioRead(TEC1);
	tecla_2 = !gpioRead(TEC2);
	tecla_3 = !gpioRead(TEC3);
	tecla_4 = !gpioRead(TEC4);
	
		
	//--------Lectura de la UART_USB para saber que led parpadea--------
	if(uartReadByte(UART_USB, & byteRecibido)  ){
	switch(byteRecibido){
	case 'a':
	led_parpadea=led_B;
	break;
		
	case '1':
	led_parpadea=led_1;
	break;
	
	case '2':
	led_parpadea=led_2;
	break;
	
	case '3':
	led_parpadea=led_3;
	break;
	}
	
	}
	//-----------Escritura de los estados de las teclas en la UART_USB-----
	
	if(tecla_1==1){
	tiempo_parpadeo=50;// el led estara parpadeando cada 50 milisegundos
	}
	else
		tiempo_parpadeo=0;
		
	
	
	if(tecla_2==1){
	tiempo_parpadeo=100;// el led estara parpadeando cada 100 milisegundos
	}
	else
		tiempo_parpadeo=0;
	
	
	if(tecla_3==1){
	tiempo_parpadeo=200;// el led estara parpadeando cada 200 milisegundos
	}
	else
		tiempo_parpadeo=0;
	
	
	if(tecla_4==1){
	tiempo_parpadeo=500;// el led estara parpadeando cada 500 milisegundos
	}
	else
		tiempo_parpadeo=0;
	
	//--------------hacemos el parpadeo------------------------
	if(delayRead(&tiempo_parpadeo)&&tiempo_delay ¡=0){
		toggle(led_parpadea,tiempo_parpadeo);
	}
		
	
         
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}


/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
