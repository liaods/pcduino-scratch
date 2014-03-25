#ifndef _ARDUINO_CORE_H_
#define _ARDUINO_CORE_H_
#include <stdio.h>
#include <stdlib.h>

/************************  digital I/O begin **********************/
#undef INPUT
#define INPUT 0

#undef OUTPUT
#define OUTPUT 1

#undef INPUT_PULLUP
#define INPUT_PULLUP 8

#undef HIGH
#define HIGH 1

#undef LOW
#define LOW 0

/*
Syntax
 
pinMode(pin, mode) 


Parameters
 
pin: the number of the pin whose mode you wish to set 


mode: INPUT, OUTPUT, or INPUT_PULLUP. (see the digital pins page for a more complete description of the functionality.)
 

Returns
 
None 
*/
void pinMode(int pin, unsigned int mode);


/*
Syntax
 
digitalWrite(pin, value) 


Parameters
 
pin: the pin number 


value: HIGH or LOW 


Returns
 
none 
*/
void digitalWrite(int pin, int value);


/*
Syntax
 
digitalRead(pin) 


Parameters
 
pin: the number of the digital pin you want to read (int) 


Returns
 
HIGH or LOW 
*/
int digitalRead(int pin);
/************************  digital I/O end **********************/


/************************  time begin **********************/
/*
Syntax
 
delay(ms) 


Parameters
 
ms: the number of milliseconds to pause (unsigned long) 


Returns
 
nothing 
*/
void delay(unsigned long ms);

/*
Syntax
 
delayMicroseconds(us) 


Parameters
 
us: the number of microseconds to pause (unsigned int) 


Returns
 
None 
*/
void delayMicroseconds(unsigned int us);

/************************  time end **********************/



/*
Syntax
 
analogRead(pin) 


Parameters
 
pin: the number of the analog input pin to read from (0-5 on a10 board )
 

Returns
 
int (0 to 63) 


Note
 
If the analog input pin is not connected to anything, the value returned by analogRead() will fluctuate based on a number of factors (e.g. the values of the other analog inputs, how close your hand is to the board, etc.).
*/
int analogRead(int pin);

/*
Syntax
 
analogWrite(pin, value) 


Parameters
 
pin: the pin to write to. 


value: the duty cycle: between 0 (always off) and 255 (always on). 


Returns
 
nothing 
*/
void analogWrite(int pin, int value); 
#define MAX_PWM_LEVEL 255

static const char *pwm_dev = "/dev/pwmtimer";
typedef struct tagPWM_Config {
    int channel;
    int dutycycle;
} PWM_Config,*pPWM_Config;

typedef struct tagPWM_Freq {
    int channel;
    int step;
    int pre_scale;
    unsigned int freq;
} PWM_Freq,*pPWM_Freq;

#define PWMTMR_START    (0x101)
#define PWMTMR_STOP     (0x102)
#define PWMTMR_FUNC     (0x103)
#define PWMTMR_TONE     (0x104)
#define PWM_CONFIG      (0x105)
#define HWPWM_DUTY      (0x106)
#define PWM_FREQ        (0x107)

#define MAX_PWMTMR_FREQ (2000)   //2kHz pin 3,9,10,11
#define MIN_PWMTMR_FREQ (126)    //126Hz pin 3,9,10,11
#define MAX_PWMHW_FREQ  (20000)  //20kHz pin 5,6

#endif
