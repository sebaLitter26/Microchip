//#include <18F452.H>
#include <18F46K20.H>  
#fuses XT, NOWDT, BROWNOUT, PUT, NOLVP 
#use delay(clock=4000000) 
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS) 

#include "count_18f.c" 

//====================================== 
void main() 
{ 
int16 result; 

// Create a test signal from the same PIC that is 
// measuring the signal.  Then we don't need an 
// external frequency generator.  The PIC supplies it. 
// Use the hardware PWM module to generate the signal. 
// Connect a jumper from pin C2 to pin B0 for this test. 
// This will put a 244 Hz squarewave on pin B0. 
//setup_ccp1(CCP_PWM); 
//setup_timer_2(T2_DIV_BY_16, 255, 1);  // PWM freq = 244 Hz (@ 4 MHz) 
//set_pwm1_duty(128);  // 50% duty cycle. 

// Call the COUNT function to measure the frequency. 
// Measure the number of counts in one second 
// from the input signal on Pin B0.  This will 
// be the frequency (in Hz) of that signal. 
while(1) 
  { 
   result = count(PIN_B0, 1000); // Get count on Pin B0 for 1000 ms 
   printf("%lu\n\r", result); 
   delay_ms(500); 
  } 

}