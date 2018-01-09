# Digital Voltmeter with the PIC24

In this project, I tried to make a digital voltmeter that samples voltages and report them via a serial interface on the computer via a
terminal emulator (hyperterminal.)

To do this I used the ADC and the UART. I operated the ADC
at its maximum speed and the UART at 115,200 baud. This application which will samples the analog input (from AN0) and keeps a running average of the last 512 samples. The
sample standard deviation (the square root of the sample variance) is also calculated. I output the decimal representation of the voltage to the host computer
using the UART interface on the PIC. You
should sample the analog input as fast as possible, but (because of the serial
interface) you can’t report the results that fast (see below).
I decided to print the results as: ”X.XXXXV xxx.xmV” where the first
number is the voltage and the second number is the standard deviation (in
millivolts). Note that, because we are taking samples in groups of 512, the
resolution of our measurement would be .006295 millivolt. Our actual accu-
racy won’t be that good, but it will be on the order of .1 mV, as a goal.

After each result is printed, I print a carriage return (hex 0x0D) but no
line feed. (This means that the terminal will just display the current value
at any instant. The next value will overwrite the last value.)

I use the timer3 conversion trigger method, and assume a minimum sample
period (including the acquisition time, the conversion time -12 Tad- and
overhead) of 2.00 microseconds to 2.125 microseconds. That means it will
take from 1.024 to 1.088 milliseconds.
I decided to send a total of 16 characters for each result, which took
take 1.38 milliseconds.

Finally I used PuTTY as the terminal emulator in this project.
