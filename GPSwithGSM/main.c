// Genera un COM virtual y se comunica bidireccionalmente con el a través del PIC18F26j50.
// Ultima version 1/11/2012
// CCs: Version 4.124
// Probado: OK
// Autor: Ing. Fernando R. Tagliaferri
//solo para ser utilizado como ejemplo. Prohibida su comercializacion sin la conformidad por escrito del autor. Derechos registrados.


#include <18F26j50.h> // Definición de registros internos.

#device adc=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES HSPLL                    //High Speed Crystal/Resonator with PLL enabled
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NOPROTECT                //Code not protected from reading
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES IOL1WAY                  //Allows only one reconfiguration of peripheral pins
#FUSES NOWPCFG               
#FUSES WPEND                 
#FUSES WPDIS                 
#FUSES NOCPUDIV              
#FUSES LPT1OSC                  //Timer1 configured for low-power operation
#FUSES T1DIG                 
#FUSES MSSPMSK7              
#FUSES PLL3               
#FUSES DSWDT2147483648       
#FUSES DSWDT                 
#FUSES DSBOR                 
#FUSES RTCOSC_T1             
#FUSES DSWDTOSC_INT          


#use delay(clock=48000000)
#pin_select U2TX=PIN_A1  
#pin_select U2RX=PIN_A0 


#use rs232 (UART2, BAUD=9600, PARITY=N, BITS=8, stream=SERIAL_GSM)
#use rs232(baud=4800,parity=N,rcv=PIN_B0,bits=8,stream=SERIAL_GPS)


#include "usb_cdc.h" // Descripción de funciones del USB.
#include "usb_desc_cdc.h" // Descriptores del dispositivo USB.

#byte OSCCON = 0xFD3 
#byte UCFG  = 0xF39 
#byte OSCTUNE= 0xF9B 

#define PIN_ON  output_high
#define PIN_OFF output_low
#define POWERKEYGSM  PIN_C1
#define LDR  PIN_B3
#define PANIC  PIN_C0
#define LED  PIN_B2
#define POWERKEYGSM  PIN_C1
#define RELAY  PIN_B3
#define X_ADC  2
#define Y_ADC  3
#define Z_ADC  4
#define TEMP_ADC   11


int flag = 0, flag2 = 0;
long eje_x, eje_y, eje_z, temperature;
float volt_x, g_x, volt_y, g_y, volt_z, g_z, volt_temp, grados;
char string_g_x[15], string_g_y[15], string_g_z[15], string_temp[15];

//************************************************************************************************************************************* 
//--------------------------------------Interrupcion serie
//************************************************************************************************************************************* 
#INT_RDA

void RDA_isr(void) {
    int8 d;
    d = 0;
    if (flag == 1) {
        d = getchar(SERIAL_GSM);
        printf(usb_cdc_putc, "%c", d);
    }
}

#INT_RDA2

void RDA2_isr(void) {
    int8 d;
    d = 0;
    d = getchar(SERIAL_GSM);
    printf(usb_cdc_putc, "%c", d);
}
//--------------------------------------------------------------------------------------------------------------------

float conver_volt(long adc) {
    float volt;
    volt = (float) ((adc / 1024.0)*3.3);
    return (volt);
}

float conver_ejes(float eje) {
    float value;
    value = (float) ((eje - 1.65) / 0.8);
    return (value);
}

float conver_grados(float volt_adc) {
    float grados;
    grados = (float) ((volt_adc - 0.5) / 0.01);
    return (grados);
}

void inicializacionSIM900(void) {
    int cont;
    delay_ms(1000);
    puts("A", SERIAL_GSM); //para sincronizar el baudrate del SIM 900
    delay_ms(300);
    putchar(0x0d, SERIAL_GSM);
    delay_ms(3000);

    for (cont = 0; cont < 5; cont++) // mando varios AT para el autobauding
    {
        puts("AT", SERIAL_GSM);
        delay_ms(300);
        putchar(0x0d, SERIAL_GSM);
        delay_ms(200);
        output_high(LED);
        delay_ms(200);
        output_low(LED);
    }
    //-----------------------------------seteos de configuracion del SIM900------------------------         
    puts("AT+CMGF=1\r", SERIAL_GSM); // configuro para que trabaje en modo texto y no PDU
    delay_ms(100);
    putchar(0x0d, SERIAL_GSM);
    delay_ms(100);
    return;
}


void mandarGETMethod(void)
{
    printf(SERIAL_GSM, "AT\r\n");
    delay_ms(300);
    printf(SERIAL_GSM, "AT+CGPSPWR=1\r\n");
    delay_ms(300);
    printf(SERIAL_GSM, "AT+CGPSRST=1\r\n");
    delay_ms(300);
}

void GSM(void) {
    do {
        usb_task();
        if (usb_enumerated()) // Espera a que el dispositivo sea enumerado por el host.
        {
            if (usb_cdc_kbhit()) // En espera de nuevos caracteres en el buffer de recepción.
            {
                printf(SERIAL_GSM, "%c", usb_cdc_getc());
            }
        }

    } while (TRUE); // bucle infinito.
}

void GPS(void) {
    printf(SERIAL_GSM, "AT\r\n");
    delay_ms(300);
    printf(SERIAL_GSM, "AT+CGPSPWR=1\r\n");
    delay_ms(300);
    printf(SERIAL_GSM, "AT+CGPSRST=1\r\n");
    delay_ms(300);
    do {
        if (kbhit(SERIAL_GPS)) {
            pin_on(led);
            printf(usb_cdc_putc, "%c", getchar(SERIAL_GPS));
        }
    } while (TRUE); // bucle infinito.
}

void OPERACION_MANUAL(void) {
    long temperature;
    float volt_temp, grados;
    float ADC1;
    char valor;
    float i, a, w;


    printf(usb_cdc_putc, "Mannual Operation\n\r");
    delay_ms(50);
    printf(usb_cdc_putc, "Menu:");
    delay_ms(50);
    printf(usb_cdc_putc, "1-2=ON/OFF LED");
    delay_ms(50);
    printf(usb_cdc_putc, "3=LDR");
    delay_ms(50);
    printf(usb_cdc_putc, "4=PANIC Button Status");
    delay_ms(50);
    printf(usb_cdc_putc, "5=TEMP");
    delay_ms(50);
    printf(usb_cdc_putc, "6=X/Y/Z Accelerometer");
    delay_ms(50);

    do {

        if (usb_cdc_kbhit()) { // En espera de nuevos caracteres en el buffer de recepción.
            valor = usb_cdc_getc();
            if (valor == '1') {
                PIN_ON(LED);
            }
            if (valor == '2') {
                PIN_OFF(LED);
            }
            if (valor == '3') {
                set_adc_channel(9); //the next read_adc call will read channel 0
                read_adc(ADC_START_ONLY); //only starts the conversion
                while (adc_done() == False);
                ADC1 = read_adc();
                printf("LDR Analog Value: %.0f \n\r", ADC1);
            }
            if (valor == '4') {
                printf("PANIC Status: %d", input(PANIC));
            }
            if (valor == '5') {
                set_adc_channel(11); //the next read_adc call will read channel 0
                read_adc(ADC_START_ONLY); //only starts the conversion
                while (adc_done() == False);
                temperature = read_adc();
                volt_temp = conver_volt(temperature);
                grados = conver_grados(volt_temp);
                printf(usb_cdc_putc, "TEMP Analog Value:");
                printf(usb_cdc_putc, " %.0f ", grados);
            }
            if (valor == '6') {
                set_adc_channel(2); //the next read_adc call will read channel 0
                read_adc(ADC_START_ONLY); //only starts the conversion
                while (adc_done() == False);
                i = read_adc();
                delay_ms(1); //a small delay is required after setting the channel and before read
                set_adc_channel(3); //the next read_adc call will read channel 0
                read_adc(ADC_START_ONLY); //only starts the conversion
                while (adc_done() == False);
                a = read_adc();
                set_adc_channel(4); //the next read_adc call will read channel 0
                delay_ms(1); //a small delay is required after setting the channel and before read
                read_adc(ADC_START_ONLY); //only starts the conversion
                while (adc_done() == False);
                w = read_adc();

                printf(usb_cdc_putc, "%.0f : %.0f : %.0f  ", i, a, w);
            }


        }

    } while (TRUE); // bucle infinito.
}

void Mensaje(void) {



    if (flag2 == 0) {
        delay_ms(1000);
        flag2 = 1;

        printf(usb_cdc_putc, "Trackme Board 2013 v1.0\n \r");
        delay_ms(50);
        printf(usb_cdc_putc, "MCelectronics\n \r");
        delay_ms(50);
        printf(usb_cdc_putc, "www.mcelectronics.com.ar\n \r");
        delay_ms(50);
        printf(usb_cdc_putc, "Options:\n \r");
        delay_ms(50);
        printf(usb_cdc_putc, "1- GSM SIM908\n \r");
        delay_ms(50);
        printf(usb_cdc_putc, "2- GPS SIM908\n \r");
        delay_ms(50);
        printf(usb_cdc_putc, "3- OP. MANUAL\n \r");
        delay_ms(50);
        printf(usb_cdc_putc, "4- Credits\n \r");
    }
}

void main() {
    setup_adc_ports(sAN2 | sAN3 | sAN4 | sAN11 | VSS_VDD);
    setup_adc(ADC_CLOCK_DIV_16 | ADC_TAD_MUL_12);
    set_tris_a(0b00101101);
    set_tris_b(0b00001001);
    set_tris_c(0b00000101);
    enable_interrupts(INT_RDA);
    enable_interrupts(INT_RDA2);
    enable_interrupts(GLOBAL);
    PIN_OFF(LED);
    PIN_ON(POWERKEYGSM);
    char opcion;
    OSCTUNE = 0xC0;
    OSCCON = 0xF0;
    UCFG |= 0x4;
    PIN_OFF(led);
    inicializacionSIM900(); //Inicializo el SIM900
    /*
    usb_cdc_init(); // Configuramos al puerto virtual.
    usb_init(); // Inicializamos el stack USB.
    while (!usb_cdc_connected()) {
    }

    do {
        usb_task();
        if (usb_enumerated()) { // Espera a que el dispositivo sea enumerado por el host.

            Mensaje();
            if (usb_cdc_kbhit()) { // En espera de nuevos caracteres en el buffer de recepción.
                opcion = usb_cdc_getc();
                if (opcion == '1' && flag == 0) { //lo que llegó fué el caracter 1

                    printf(usb_cdc_putc, "GSM Connected");
                    delay_ms(50);
                    flag = 1;
                    GSM();
                }
                if (opcion == '2' && flag == 0) { //lo que llegó fué el caracter 2

                    printf(usb_cdc_putc, "GPS Connected");
                    delay_ms(50);
                    flag = 2;
                    GPS();
                }
                if (opcion == '3' && flag == 0) { //lo que llegó fué el caracter 3

                    flag = 2;
                    OPERACION_MANUAL();
                }
                if (opcion == '4') { //lo que llegó fué el caracter 2

                    printf(usb_cdc_putc, "Autor: Ing Fernando R. Tagliaferri \r\n");
                    delay_ms(50);

                }
            }
        }
    } while (TRUE); // bucle infinito.
     * 
    */
    
    
    while (true) // bucle infinito 
    {
        set_adc_channel(X_ADC);
        delay_ms(1);
        eje_x = read_adc();
        set_adc_channel(Y_ADC);
        delay_ms(1);
        eje_y = read_adc();
        set_adc_channel(Z_ADC);
        delay_ms(1);
        eje_z = read_adc();

        //Convertir datos:

        volt_x = conver_volt(eje_x);
        g_x = conver_ejes(volt_x);

        volt_y = conver_volt(eje_y);
        g_y = conver_ejes(volt_y);

        volt_z = conver_volt(eje_z);
        g_z = conver_ejes(volt_z);

        //if((g_x>1.5) || (g_x<-1.5))
        if (input(PANIC) == 0) // Si presiono el boton manda un SMS indicando que se preciono el boton
        {
            
            
            sprintf(string_g_x, "EjeX=%1.3f g.", g_x);
            sprintf(string_g_y, "EjeY=%1.3f g.", g_y);
            sprintf(string_g_z, "EjeZ=%1.3f g.", g_z);
            
            mandarGETMethod();
            fprintf(SERIAL_GSM, "AT+CMGS=\"1165404122\"\r"); // aca poner el numero de celular que recibe el SMS
            delay_ms(200);
            fprintf(SERIAL_GSM, "TrackMe(GPS) ha detectado un movimiento brusco:\r");
            fputs(string_g_x, SERIAL_GSM);
            fputs(string_g_y, SERIAL_GSM);
            fputs(string_g_z, SERIAL_GSM);
            delay_ms(100);
            fprintf(SERIAL_GSM, "%c", 0x1a);
            output_high(LED);
        }
        delay_ms(1000);
        output_low(LED);


        if (kbhit(SERIAL2)) // Espero un nuevo dato en la USART2
        {
            e = 0;
            e = getchar(SERIAL2); // En la variable e se guardanlos caracteres
            switch (e) {
                case 10: // Espero el new line feed
                    if (cbuffGPS[0] == '$' && cbuffGPS[1] == 'G' && cbuffGPS[2] == 'P' && cbuffGPS[3] == 'R' && cbuffGPS[4] == 'M' &&
                            cbuffGPS[5] == 'C' && cbuffGPS[18] == 'A') //esta linea valida si el dato es valido(detecto al menos 3 satelites)
                    {
                        PIN_ON(LED);
                        strcpy(cbuffGPS2, cbuffGPS); // La trama $GPRMC completa queda guardada en cbuffGPS2
                    }
                    xbuff = 0;
                    flag = 0;
                    break;
                default:
                    cbuffGPS[xbuff++] = e;
                    if (xbuff > (lenbuffGPS - 1))
                        xbuff = lenbuffGPS;
            }
        }

        if (cbuffGPS2[0] == '$' && cbuffGPS2[1] == 'G' && cbuffGPS2[2] == 'P' && cbuffGPS2[3] == 'R' && cbuffGPS2[4] == 'M' && cbuffGPS2[5] == 'C') {
            strncpy(latg, &cbuffGPS[20], 2); // convierto en coordenadas googlemaps
            strncpy(latm, &cbuffGPS[22], 7);
            strncpy(longg, &cbuffGPS[32], 3);
            strncpy(longm, &cbuffGPS[35], 7);
            latitudgoogle = (atof(latm)) / 60;
            longitudgoogle = (atof(longm) / 60);
            sprintf(&latitud[1], "%.6f", latitudgoogle);
            latitud[0] = latg[0];
            latitud[1] = latg[1];
            sprintf(&longitud[2], "%.6f", longitudgoogle);
            longitud[0] = longg[0];
            longitud[1] = longg[1];
            longitud[2] = longg[2];
        }
    }
    
}
