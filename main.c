/*
 * Nombre del archivo:   main.c
 * Autor:
 *
 * Descripción: 
 *      El sistema debe contar cuantas veces se presiona cada tecla (TEC1, TEC2,
 *      TEC3 o TEC4).
 *      Si recibe la letra 'Q' por puerto serie, debe responder enviando un
 *      informe con esas cantidades.
 *      Por ejemplo:
 *      > Informe de teclas:
 *      > ---------------------
 *      >     TEC1: 12
 *      >     TEC2: 4
 *      >     TEC3: 6
 *      >     TEC4: 17
 *      > ---------------------
 *      Recuerda que para que cualquier terminal detecte el fin de línea
 *      se debe enviar "\r\n".
 *      Cuando se recibe la letra 'D' por puerto serie, debe reiniciar los
 *      contadores de las teclas.
 *      Si se recibe un comando válido debe parpadear el led verde (100 ms).
 *      Si se recibe un comando inválido debe parpadear el led rojo (100 ms).
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>

/* ------------------------ Definiciones ------------------------------------ */
#define _XTAL_FREQ  4000000L    // Frecuencia de operación del cristal

/* ------------------------ Definición de salidas --------------------------- */
#define PIN_LED_ROJO    PORTEbits.RE2
#define TRIS_LED_ROJO   TRISEbits.TRISE2

#define PIN_LED_VERDE   PORTEbits.RE0
#define TRIS_LED_VERDE  TRISEbits.TRISE0

/* ------------------------ Definición de entradas -------------------------- */
#define PIN_TEC1        PORTBbits.RB0
#define TRIS_TEC1       TRISBbits.TRISB0

#define PIN_TEC2        PORTBbits.RB1
#define TRIS_TEC2       TRISBbits.TRISB1

#define PIN_TEC3        PORTBbits.RB2
#define TRIS_TEC3       TRISBbits.TRISB2

#define PIN_TEC4        PORTBbits.RB3
#define TRIS_TEC4       TRISBbits.TRISB3

/* ------------------------ Bits de configuración --------------------------- */
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits
//#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit
#pragma config CP = OFF         // Code Protection bit
#pragma config CPD = OFF        // Data Code Protection bit
#pragma config BOREN = OFF      // Brown Out Reset Selection bits
#pragma config IESO = OFF       // Internal External Switchover bit
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit
#pragma config LVP = OFF        // Low Voltage Programming Enable bit

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits

/* ------------------------ Prototipos de funciones ------------------------- */
void gpio_config();
void uart_config();
uint8_t uart_rx_byte(uint8_t *dato);
void uart_tx_byte(uint8_t dato);

/* ------------------------ Implementación de funciones --------------------- */
void main(void) {                       // Función principal
    char dato_recibido;
    int cont[4] = {0,0,0,0};
    PIN_LED_ROJO=0;
    PIN_LED_VERDE=0;
    
    gpio_config();                      // Inicializo las entradas y salidas
    uart_config();                      // Configuro la UART
    
    printf("Sistema inicializado!\r\n");
    printf("---------------------\r\n");
    
    while(1) {                          // Super loop
        // Ver este link: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg 
        // TODO: Completar las acciones de las teclas
        if(PIN_TEC1 == 0){
            cont[0] = cont[0]+1;
            while(PIN_TEC1 == 0){
            __delay_ms(40);
            }
        }
        if(PIN_TEC2 == 0){
            cont[1] = cont[1]+1;
            while(PIN_TEC2 == 0){
            __delay_ms(40);
            }
        }         
        if(PIN_TEC3 == 0){
            cont[2] = cont[2]+1;
            while(PIN_TEC3 == 0){
            __delay_ms(40);
            }
        }        
        if(PIN_TEC4 == 0){
            cont[3] = cont[3]+1;
            while(PIN_TEC4 == 0){
            __delay_ms(40);
            }
        }
        
        if( uart_rx_byte( &dato_recibido ) ) {
            // TODO: Completar las acciones de los comandos
            if(dato_recibido == 'Q'){
                PIN_LED_VERDE = 1;
                __delay_ms(100);
                PIN_LED_VERDE = 0;
                
                printf("\r\n Informe de las teclas: \r\n");
                printf("\r\n tec1 = %d \r\n tec2 = %d \r\n tec3 = %d \r\n tec4 = %d \r\n",cont[0],cont[1],cont[2],cont[3]);
                
            }
            if(dato_recibido == 'D'){
                PIN_LED_VERDE = 1;
                __delay_ms(100);
                PIN_LED_VERDE = 0;
            
                cont[0]=0;
                cont[1]=0;
                cont[2]=0;
                cont[3]=0;
            }
            
            if(dato_recibido != 'D' && dato_recibido != 'Q') {
                PIN_LED_ROJO = 1;
                __delay_ms(100);
                PIN_LED_ROJO = 0;
            }
            
        }
        
    }
    
    // NO DEBE LLEGAR NUNCA AQUÍ, debido a que este programa se ejecuta
    // directamente sobre un microcontrolador y no es llamado por un ningún
    // sistema operativo, como en el caso de un programa para PC.
    
    return;
}

void gpio_config() {    

    ANSEL = 0;                  // Configuro todas las entradas
    ANSELH = 0;                 //   como digitales

    TRIS_TEC1 = 1;
    TRIS_TEC2 = 1;    
    TRIS_TEC3 = 1;
    TRIS_TEC4 = 1;

    TRIS_LED_ROJO = 0;
    TRIS_LED_VERDE = 0;    
}

void uart_config() {
    // TODO: Completa configuración de la UART
    TXSTAbits.TX9 = 0;      //Transmision de 8 bits
    TXSTAbits.TXEN = 1;     //Habilita Transmision
    TXSTAbits.SYNC = 0;     //Modo Transmision
    
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;             //Baudrante 9600 con Cristal de 4mhz
    
    RCSTAbits.SPEN = 1;     //Habilito el Puerto Serie
    RCSTAbits.RX9 = 0;      //Recepcion de 8 Bits
    RCSTAbits.CREN = 1;     //Habilito de Recepcion
 
}

/**
 * @brief	Envía un byte a la salida stdout en forma bloqueante pero por poco tiempo
 * @param	data    Byte o dato a transmitir
 * @return	Nada
 * @note    Define la salida estandar para la librería stdio
 */
void putch(char data) {
    while (PIR1bits.TXIF == 0)   //Espera que haya espacio en la FIFO
        continue;
    TXREG = data;   //Envía el byte
}

/**
 * @brief	Toma un byte de la entrada stdin en forma bloqueante
 * @return	El byte recibido
 * @note    Define la entrada estandar para la librería stdio
 */
char getch(void) {
    while (PIR1bits.RCIF == 0)   //Espera hasta que haya un byte recibido
        continue;
    return RCREG;   //retorna lo recibido
}

/**
 * @brief	Envía un byte a la salida stdout en forma bloqueante pero por poco tiempo
 * @param	data    Byte o dato a transmitir
 * @return	Nada
 * @note    Define la salida estandar para la librería stdio
 */
void uart_tx_byte( uint8_t dato ) {
    while (PIR1bits.TXIF == 0)   //Espera que haya espacio en la FIFO
        continue;
    TXREG = dato;   //Envía el byte
}

/**
 * @brief       Toma un byte de la FIFO de recepción en forma no bloqueante,
 *              chequea primero si hay un dato disponible
 * @param[out]  dato    Apunta al destino para el byte recibido
 * @return      1 si hay un byte recibido, 0 si no hay dato disponible 
 */
uint8_t uart_rx_byte( uint8_t *dato ) {
    if (PIR1bits.RCIF == 1) {
        *dato = RCREG;
        return 1;
    } else {
        return 0;
    }
}

/* ------------------------ Fin de archivo ---------------------------------- */