#include <p24Fxxxx.h>
// #include <PPS.h> // older versions of the XC16 compiler
#include <xc.h>
#include <stdlib.h>
#include <math.h>

// CONFIG2
#pragma config POSCMOD = NONE // Primary Oscillator Select (Primaryoscillator disabled)
#pragma config I2C1SEL = PRI // I2C1 Pin Location Select (Use defaultSCL1/SDA1 pins)
#pragma config IOL1WAY = OFF // IOLOCK Protection (IOLOCK may bechanged via unlocking seq)
#pragma config OSCIOFNC = OFF // Primary Oscillator Output Function(OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clockswitching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillatorwith PLL module (FRCPLL))
#pragma config SOSCSEL = SOSC // Sec Oscillator Select (DefaultSecondary Oscillator (SOSC))
#pragma config WUTSEL = LEG // Wake-up timer Select (Legacy Wake-upTimer)
#pragma config IESO = ON // Internal External Switch Over Mode(IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (Prescaler ratio of1:128)
#pragma config WINDIS = ON // Watchdog Timer Window (StandardWatchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timeris disabled)
#pragma config ICS = PGx1 // Comm Channel Select (EmulatorEMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF // General Code Segment Write Protect(Writes to program memory are allowed)
#pragma config GCP = OFF // General Code Segment Code Protect (Codeprotection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port isdisabled)

volatile unsigned char buffer[32];
volatile unsigned char front = 0;
volatile unsigned char back = 0;

#define DACMASK 0x3FFF
#define DACBITS 0x3000

volatile unsigned short int sample = DACBITS;
volatile float sinVal;
volatile float time;

void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt(void)
{
    sample &= DACMASK;
    sample |= DACBITS;
    
    // float test = 628*(time/1000);
    // float test2 = sin(test);
    
    IFS0bits.T2IF = 0;
    
    time +=1;
    
    sinVal = 1.024*(sin(628*(time/10000))+sin(1570*(time/10000)));
    
    PORTBbits.RB7 = 1;
    
    sample = (float)(((sinVal+2)/4.096)*4096); // the next "step" value.
    
    sample |= DACBITS;
    
    //SPI1BUF = sample;
    
    PORTBbits.RB7 = 0;
    
    SPI1BUF = sample;
}

void setup(void)
{
    CLKDIVbits.RCDIV = 0; //16mhz
    
    AD1PCFG = 0x9fff;
    
    TRISB = 0xffff; // make all PORTBbits inputs initially
    
    // add your configuration commands below
    
    PORTB = 0xffff;
    
    TRISBbits.TRISB5 = 0;
    
    TRISBbits.TRISB6 = 0;
    
    TRISBbits.TRISB7 = 0; // this will be the DAC /LDAC. I assume /CS is hardwired to GND
    
    T2CON = 0;
    
    PR2 = 15999; // Period of step will be 363 Tcy = 22.7 us
    
    T2CONbits.TON = 1;
    
    // Peripheral Pin Select
    
    // Ver 1.25 or later
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    
    RPOR2bits.RP5R = 7; //RB5->SPI1:SDO1; See Table 10-3 on P109 of the datasheet
    
    RPOR3bits.RP6R = 8; //RB6->SPI1:SCK1OUT;
    
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    SPI1CON1 = 0;
    
    SPI1CON1bits.MSTEN = 1; // master mode
    
    SPI1CON1bits.MODE16 = 1; // sixteen bits
    
    SPI1CON1bits.CKE = 1;
    
    SPI1CON1bits.CKP = 0;
    
    SPI1CON1bits.SPRE = 0b110; // secondary prescaler = 2
    
    SPI1CON1bits.PPRE = 0b11; // primary prescaler = 1;
    
    // SPI1CON1bits.PPRE = 0b01; // primary prescaler = 16;
    
    SPI1CON2 = 0;
    
    SPI1STAT = 0;
    
    SPI1STATbits.SISEL = 0b101; // IF set when last bit is shifted out
    // That means the SPI transfer is complete.
    
    SPI1STATbits.SPIEN = 1;
    
    IFS0bits.T2IF = 0;
    
    TMR2 = 0;
    
    IPC1bits.T2IP = 5;
    
    IEC0bits.T2IE = 1;
}

int main(int argc, char *argv[])
{
    
    setup();
    
    while (1){}
    
    return 0; // never reached (we hope)
}
