# Digital to Analog Conversion

In this project I attemted to generate an analog singal using an MCP4821 digital-to-analog converter along with the pic24.

As an example, the waveform will be this sum of sine waves: 
x(t) = 1.024 × sin(2 × π ×f × t) + sin(5 × π × f × t)

Note that this has maximum amplitude 2.048 Volts.
To make the waveform smooth, a new sample has to be generated as fast as possible.
I think that a good goal in our case would be to generate a good
wave for f in the range 100-10000 Hz, with a sample rate in the 50-100KHz
range.

I connected to the MCP4821 via the SPI interface.
I used two pins (the SDO and SCK of the SPI interface – the DAC is an SPI receiver only, so there’s no input on SDI) plus
a third pin, the DAC (CS)'. The way it sends data to the DAC is to drive
the (CS)' low (active), then send 16 bits of data (two bytes), then drive the
(CS)' back high.

The way the DAC is interfaced, you can just write a bit pattern to it. To make it work right, send a 16-bit value whose most-significant 4 bits are 0001, and whose
least-significant 12 bits are the 12-bit encoding of the voltage you wish to
see on the output, with 0 producing 0V and 0xfff producing about 2V.

The communication works like this:

1. Initialize the MSSP for SPI operation.
2. To send a new value to the DAC, drive the chip select to 0, then send
data, then set chip select high. There are 16 bits per
sample.
3. Start with a sawtooth wave with a 100 Hz frequency in order to debug.

Note:
- All work is done via the ISR.
- To get the desired accuracy in the time domain used one
of the timers to provide a time base.
