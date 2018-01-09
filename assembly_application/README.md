# 48-bit Multiplication on the PIC24

In this project, I wrote some assembly and C code that does the following:

1. Write a 48-bit unsigned multiplication subroutine in PIC24F assembly
language.

2. Write a subroutine which implements the algorithm for the rand48 pseudo-
random number generators in PIC24F assembly language (the methodology is given below.)

3. Do the same thing in C.

The program initially starts with all LED’s off. Then, for each press
of the switch, it displays a pattern on the LED’s indicating, in binary the number it generated. Note that for proper operation, the switch must be debounced with either a delay in the code or a small capacitor.

## The rand48 Function
rand48 functions work by generating a sequence of 48-bit integers, X\_i , according to the linear congruential formula:
`X\_n+1 = (a\*X\_n + c)\*mod\_(m)`, where `n >= 0`
The parameter `m = 2^48`, hence 48-bit integer arithmetic is performed. `a` and
`c` are given by:

```
	a = 0x5DEECE66D;
	c = 11;
```

X\_0 is initialized using setseed48, given here as a C function:
	
```
void setSeed(unsigned long int seed)
{
	seedx = seed;
	seedx = seedx << 16;
	seedx = seedx + 0x330E;
}
```

By default, setSeed is called at initialization with `seed = 1`.