#include <p24Fxxxx.h>

#include <xc.h>

#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRC           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#pragma config SOSCSEL = SOSC           // Sec Oscillator Select (Default Secondary Oscillator (SOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                 // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

void setup(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;
    PORTB = 0xffff;
    TRISB = 3;
    TRISA = 1;
    CNPU1bits.CN2PUE = 1;
}

unsigned long long int multiply(unsigned long long int randomnumber)
{
    int c = 11;
    unsigned long long int a =  0x5DEECE66D;
    randomnumber=(a*randomnumber+11)% 0x1000000000000 ;//1000000000000 // 1 and 12 zeros in hex is 2^48
    
    return randomnumber;
}

int main(int argc, char *argv[])
{
    unsigned long long int randomnumber = 0x1330E;
    
    //unsigned short int count = 0;
    unsigned long int delay;

    setup();

    while (1)
    {
        if(!PORTAbits.RA0)
        {
         randomnumber = multiply(randomnumber);
         
        delay = 29999;
        while (delay--) ;
        asm("nop");
        delay=delay;
        asm("nop");
        
         PORTB = randomnumber<<2;
        }
    }
    return 0;
}
