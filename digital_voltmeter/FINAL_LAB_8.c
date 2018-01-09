#include <p24Fxxxx.h>
#include <PPS.h>
#include <xc.h>
#include <math.h>

//#include <libpic30.h>
//#define FCY 16000000UL

#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
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
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)




//unsigned long int aBuff[512];
volatile double aBuff[512];

//unsigned long int scaleFactor = 523776;//1023*512
//unsigned long int d0, d1, d2, d3;

void printADCVal(unsigned short int);
void printADCValDev(unsigned short int);
void PPutch(const unsigned char);
void printCR(void);

double adcAverage = 0;
double sum_dev = 0;

void __attribute__(( __interrupt__,__auto_psv__)) _U1RXInterrupt(void)
{
	IFS0bits.U1RXIF =0;
	printADCVal(adcAverage);
	PPutch('V');
	PPutch(' ');
	printADCValDev(sum_dev);
	PPutch('m');
	PPutch('V');
	printCR();    //printString((unsigned char*)"AVGvoltage STDdeviation");
    printCR();
}

void updateAvg()
{
	int i = 0;
	for (i=0; i<512; i++)
	{
		adcAverage += aBuff[i];
	}
	adcAverage = (adcAverage /512); 
    //adcAverage = (adcAverage) / (scaleFactor);
    //adcAverage = 3150;
}

void updateDev(){
	int i;
	sum_dev = 0;
	for(i=0; i<512; i++){
		sum_dev += (aBuff[i]-adcAverage)*(aBuff[i]-adcAverage);
	}
	sum_dev = sqrt(sum_dev/512);
}


void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void)
{
	static unsigned short int loc = 0;
	IFS0bits.AD1IF = 0;
	aBuff[loc++] = ADC1BUF0 * 3.2265625;
	updateAvg();
	updateDev();
	loc &=511;
	IFS0bits.U1RXIF = 1;
}
void PPutch(const unsigned char c){
	unsigned short int tmp = 5000;
	while (tmp--) tmp=tmp;
	while(!IFS0bits.U1TXIF);
	IFS0bits.U1TXIF=0;
	U1TXREG = c;
}

void printCR(void)
{
	PPutch('\r');
	//PPutch('\n');
}

void printString(unsigned char *s)
{
	while (*s) PPutch(*s++);
}

void printADCVal(unsigned short int V)
{
    
	unsigned char digits[4] = {'0','0','0','0'};
    /*
    V /= 1000;
    V % 1000;
    digits[0] = V;
    
    V /= 100;
    V % 100;
    digits[1] = V;
    
    V /= 10;
    V % 100;
    digits[2] = V;
   
    V /= 1;
    V % 100;
   digits[3] = V; 
    */
    
	digits[0] += (V) % 10;//digits[3] += (V/1000) % 10;
	V /= 10;
	digits[1] +=  (V) % 10;//digits[2] +=  (V/100) % 10;
	V /= 10;
	digits[2] += (V) % 10;//digits[1] += (V/10) % 10;
	V /= 10;
	digits[3] += (V) % 10;//digits[0] += (V/10) % 10;
    
	PPutch(digits[3]);
    PPutch('.');
	PPutch(digits[2]);
	PPutch(digits[1]);
    PPutch(digits[0]);//	PPutch('0'+ digits[3]);
  
}
void printADCValDev(unsigned short int V)
{
        
	unsigned char digits[4]= {'0','0','0','0'};
    
	digits[0] += (V) % 10;//digits[3] += (V/1000) % 10;
	V /= 10;
	digits[1] +=  (V) % 10;//digits[2] +=  (V/100) % 10;
	V /= 10;
	digits[2] += (V) % 10;//digits[1] += (V/10) % 10;
	V /= 10;
	digits[3] += (V) % 10;//digits[0] += (V/10) % 10;
    
	PPutch(digits[3]);
	PPutch(digits[2]);
	PPutch(digits[1]);
    PPutch('.');
    PPutch(digits[0]);//	PPutch('0'+ digits[3]);
}

void setup(void){
	CLKDIVbits.RCDIV = 0;
	AD1PCFG = 0x9fff;
	
	AD1CON1 = 0;
	AD1CON2=0;
	AD1CON3=0;
	
	TRISAbits.TRISA0=1;
	AD1PCFGbits.PCFG0 = 0;
	
	AD1CSSL = 0;
	AD1CSSLbits.CSSL0=1;
	AD1CHS=0;
	AD1CON3bits.ADCS=1;
	AD1CON3bits.SAMC=1;
	AD1CON2bits.SMPI=15;
	AD1CON1bits.SSRC=0b010;
	AD1CON1bits.ASAM=1;
	AD1CON1bits.ADON =1;
	IPC3bits.AD1IP = 2;
	IFS0bits.AD1IF=0;
	IEC0bits.AD1IE=1;
	
	T3CON =0;
	PR3 = 100;
	T3CONbits.TON =1;
	TMR3 = 0;
	IFS0bits.T3IF = 0;
	
	U1MODE=0;
	U1BRG =34;
	U1MODEbits.BRGH =1;
	U1MODEbits.UEN=0;
	U1MODEbits.UARTEN=1;
	U1STAbits.UTXEN = 1;
	PPSUnLock;
	
	PPSOutput(PPS_RP3, PPS_U1TX);
	PPSLock;
	IFS0bits.U1RXIF =0;
	IEC0bits.U1RXIE=1;
}
int main(int argc, char *argv[]){

	
	setup();
	AD1CON1bits.SAMP = 1;
	
	while(1){}
	return 0;
}
