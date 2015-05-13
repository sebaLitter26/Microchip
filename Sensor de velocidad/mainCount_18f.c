#include <xc.h> //incluimos el archivo de libreria generico
#include <stdio.h>
#include <delays.h>
#include <string.h>
 
#include <18F46K20.H>  
#fuses XT, NOWDT, BROWNOUT, PUT, NOLVP 
#use delay(clock=4000000) 
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS) 

#include "count_18f.c" 
 
#pragma config FOSC = XT
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = ON
#pragma config BOREN = OFF
#pragma config WDTEN = OFF
//#pragma config CCP2MX = PORTC
#pragma config PBADEN = OFF
#pragma config LPT1OSC = OFF
#pragma config HFOFST = OFF
#pragma config MCLRE = ON
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config XINST = OFF
#pragma config DEBUG = OFF

//definiciones del Programa
#define LEDRojo1 PORTAbits.RA0 //anodo1 del led rojo
#define LEDRojo2 PORTAbits.RA1 //anodo2 del led rojo
#define SENSOR PORTAbits.RA2 
#define FOTO PORTEbits.RE0;  //RELE para la camara de fotos

//funciones prototipo
void Print_LED(void);//imprime el LED
float Scan(void);//escanea el teclado de funciones
//void Setup_TMR1(void);
//void Init(void);//Inicializacion

//Variables globales
 
int vel, repeticion, repet;
float medicion=0;
int contador,duplicado=0,dup=0;
int dig1,dig2;
float tiempo=1;  //tiempo entre cada medicion. Debe ser menor o igual a 1 segundo
float k=0.022;  //constante para cambio de unidades  (Km/hora, M/seg, etc)
  

/*
void interrupt ISR_Timer1(void)
{
	TMR1IF=0;
	//TMR1H=0b10010000;//precargamos el TMR1 con 32768 para que desborde en 0.5 seg
	TMR1H=0b10000000;
	TMR1L=0;
	//segundo=segundo+1;
	Print_LED();
	contador=0;
	
}


void Setup_TMR1(void)
{
	TMR1ON=0;//apagamos el TMR1
	T1CONbits.RD16=1;//activamos modo lectura 16 bits
	T1CONbits.T1CKPS=0;//sin prescaler (1:1)
	T1OSCEN=1;//habilitamos el OSCILADOR del TMR1
	nT1SYNC=1;//modo asincronico
	TMR1CS=1;//fuente externa
	TMR1H=0b10000000;//precargamos el TMR1 con 32768 para que desborde en 0.5 seg
	//TMR1H=0b10010000;
	TMR1L=0;
	TMR1ON=1;//encendemos el timer
}
*/

float Scan(void)
{
	repeticion= (int)(1/tiempo);
	repet=repeticion;
	medicion=0;
	while(repet>0){
		medicion += count/tiempo;
		repet--;
	}
	return (medicion/repeticion)*k;
}

void Print_LED(void)
{
	vel=(int)Scan();
	
	if(vel>30){
		//if(!duplicado)
		//	FOTO=1;
		//PORTVerde=0;
		LEDRojo1=1;
		LEDRojo2=1;
	}
	else if (vel<=30 && vel>8){
		LEDRojo1=1;
		LEDRojo2=1;
		//PORTVerde=1;
	}
	else{
		//PORTVerde=0;
		//PORTRojo=0;
		LEDRojo1=0;
		LEDRojo2=0;
		duplicado=0;
		return;
	}
	
	dig1=vel/10;
	dig2=vel%10;
		
	switch(dig1)
	{
		case 0:
			LATB=0b01111111;
			break;
		case 1:
			LATB=0b00000110;
			break;
		case 2:
			LATB=0b01011011;
			break;
		case 3:
			LATB=0b01001111;
			break;
		case 4:
			LATB=0b01100110;
			break;
		case 5:
			LATB=0b01101101;
			break;
		case 6:
			LATB=0b01111101;
			break;
		case 7:
			LATB=0b00000111;
			break;
		case 8:
			LATB=0b01111111;
			break;
		case 9:
			LATB=0b01101111;
			break;
	}
		
	switch(dig2)
	{
		case 0:
			LATD=0b01111111;
			break;
		case 1:
			LATD=0b00000110;
			break;
		case 2:
			LATD=0b01011011;
			break;
		case 3:
			LATD=0b01001111;
			break;
		case 4:
			LATD=0b01100110;
			break;
		case 5:
			LATD=0b01101101;
			break;
		case 6:
			LATD=0b01111101;
			break;
		case 7:
			LATD=0b00000111;
			break;
		case 8:
			LATD=0b01111111;
			break;
		case 9:
			LATD=0b01101111;
			break;
	}
	
	
	//TOMA FOTO
/*	if(FOTO && !duplicado)
	{
		Delay1KTCYx(300);
		FOTO=0;
		duplicado=1;
	}
*/	
	
}

void main(void)
{
	ANSEL=0;		//desconfiguramos los canales analogicos
	ANSELH=0;
	TRISA2=1;
	TRISA1=0;
	TRISA0=0;
	TRISD=0;
	//TRISC=0;
	TRISB=0;
	/*
	Setup_TMR1();//configuramos el TMR1
	TMR1IF=0;//borramos el flag del Timer 1
	TMR1IE=1;//habilitamos la INT del Timer1
	PEIE=1;//habilitamos interrupciones de perifericos
	GIE=1;//habilitamos el global de interrupciones
	 */
	 
	while(1)
	{
	
		count = count(PIN_B0, tiempo*1000); // Get count on Pin B0 for 1000 ms 
		//printf("\r%lu us ", pulse_width/5 );
		//printf("\r%lu us ", count );
		Print_LED();
		
		count=0;
		/*
		if(SENSOR==1)
		{
			//contador++;
			if(dup!=1)
			{
				contador++;
				dup=1;
			}
		}else
			dup=0;
		*/
	}
	
}
