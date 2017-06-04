/*============================================================================
 * Licencia: BSD de 3 clausulas.
 * Autor: Gastón Daniel Zapata
 * Fecha: 01/06/2017
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI
#include <math.h>       // <= Funciones matematicas
#include "ff.h"       // <= Biblioteca FAT FS

/*==================[definiciones y macros]==================================*/
#define FILENAME "Muestras.txt"
/*==================[definiciones de datos internos]=========================*/

//==================[Referidas al magnetometro]=============================
static int16_t hmc5883l_x_raw;
static int16_t hmc5883l_y_raw;
static int16_t hmc5883l_z_raw;
/* Buffers */
static char magnetometrotBuff[1];// no usamos uart

//===================[Referidas a la memoria SD]=============================
static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/
int Configuraciones_generales (void);
/*==================[declaraciones de funciones externas]====================*/

//==================[Referidas al magnetometro]=============================
void sendHmc5883lToSd( int16_t axis, uint8_t axisName );
//=======================[Referidas a la memoria SD]========================
// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr );
/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

      
   Configuraciones_generales ();
   // ---------- REPETIR SOLO 10 VECES --------------------------
    //uint8_t k =0;
   //for( k=0; k<10; k++ )
   {      
     if( delayRead( &miDalay ) ){
         
         //============[MAGNETOMETRO]======================
         hmc5883lRead( &hmc5883l_x_raw, &hmc5883l_y_raw, &hmc5883l_z_raw );
         // Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
         // para leer un nuevo valor del magnetómetro
         //delay(67);
         
         
         itoa(int16_t hmc5883l_x_raw, char magnetometrotBuff, 10 );
         
         
         // ------ PROGRAMA QUE ESCRIBE EN LA SD -------

   UINT nbytes;
   
   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
      // If this fails, it means that the function could
      // not register a file system object.
      // Check whether the SD card is correctly connected
   }

   // Create/open a file, then write a string and close it
   if( f_open( &fp, FILENAME, FA_WRITE | FA_CREATE_ALWAYS ) == FR_OK ){
      f_write( &fp, char magnetometroBuff[1], 1, &nbytes );

      f_close(&fp);

      if( nbytes == 1 ){
         // Turn ON LEDG if the write operation was successful
         gpioWrite( LEDG, ON );
      }
   } else{
      // Turn ON LEDR if the write operation was fail
      gpioWrite( LEDR, ON );
   }
         /*sendHmc5883lToSd( hmc5883l_x_raw, 'x' );
         sendHmc5883lToSd( hmc5883l_y_raw, 'y' );
         sendHmc5883lToSd( hmc5883l_z_raw, 'z' );
         uartWriteString( UART_USB, "\r\n");*/
       
         //==================[MAGNETOMETRO]==========================
         hmc5883l_x_raw=0;
         hmc5883l_y_raw=0;
         hmc5883l_z_raw=0;
   } 

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta 
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/
Configuraciones_generales ()
{
    // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();
    
    //=============[Referidas al magetometro]===================
    // Inicializar HMC5883L
   HMC5883L_config_t hmc5883L_configValue;

   hmc5883lPrepareDefaultConfig( &hmc5883L_configValue );

   hmc5883L_configValue.mode = HMC5883L_continuous_measurement;
   hmc5883L_configValue.samples = HMC5883L_8_sample;

   hmc5883lConfig( hmc5883L_configValue );
    //================[Referidas a la memoria SD]==============
    // SPI configuration
   spiConfig( SPI0 );
    // Inicializar el conteo de Ticks con resolución de 10ms, 
   // con tickHook diskTickHook
   tickConfig( 10, diskTickHook );
    
}
/*==================[definiciones de funciones externas]=====================*/


//==================[Del magnetometro]============================
void sendHmc5883lToSd( int16_t axis, uint8_t axisName ){
   
   /* Envio la primer parte dle mensaje a la memoria SD */
   uartWriteString( UART_USB, "HMC5883L eje ");
   uartWriteByte( UART_USB, axisName );
   uartWriteString( UART_USB, ": ");

   /* Conversion de muestra entera a ascii con base decimal */
   itoa( (int) axis, uartBuff, 10 ); /* 10 significa decimal */
   
   /* Envio el valor del eje */
   uartBuff[4] = 0;    /* NULL */
   uartWriteString( UART_USB, uartBuff );
   
   /* Envio un 'enter' */
   uartWriteString( UART_USB, "\r\n");
}

//=================[Referida a la memoria SD]==============================
// FUNCION que se ejecuta cada vezque ocurre un Tick
bool_t diskTickHook( void *ptr ){
   disk_timerproc();   // Disk timer process
   return 1;
}
/*==================[fin del archivo]========================================*/
