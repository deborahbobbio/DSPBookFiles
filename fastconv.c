//fastconv.c
#include "DSK6713_AIC23.h" //codec support
 //set sampling rate 8 KHz
#include "lp55f.cof"
#include <math.h>
#include "fft.h"
#define PI 3.14159265358979
#define PTS 128
short buffercount = 0; //index into frames
short bufferfull=0;
COMPLEX A[PTS], B[PTS], C[PTS]; //three buffers used
COMPLEX twiddle[PTS]; //twiddle factors
COMPLEX coeffs[PTS]; //zero padded freq coeffs
COMPLEX *input_ptr, *output_ptr, *process_ptr, *temp_ptr;
float a,b; //used in complex multiply
interrupt void c_int11(void) //ISR
{
output_left_sample((short)((output_ptr + buffercount)->real));
(input_ptr + buffercount)->real = (float)(input_left_sample());
(input_ptr + buffercount++)->imag = 0.0;
if (buffercount >= PTS/2)
{
bufferfull = 1;
buffercount = 0;
}
}
void main()
{
int n,i;
for (n=0 ; n<PTS ; n++) //set up twiddle factors
{
twiddle[n].real = cos(PI*n/PTS);
twiddle[n].imag = -sin(PI*n/PTS);
}
for (n=0 ; n<PTS ; n++) //set up freq domain coeffs
{
coeffs[n].real = 0.0;
coeffs[n].imag = 0.0;
}
for (n=0 ; n<N ; n++)
{
coeffs[n].real = h[n];
}
fft(coeffs,PTS,twiddle); //transform filter coeffs
//to freq domain
input_ptr = A; //initialise frame pointers
process_ptr = B;
output_ptr = C;
comm_intr();
while(1) //frame processing loop
{
while (bufferfull == 0); //wait for buffer full
bufferfull = 0;
temp_ptr = process_ptr;
process_ptr = input_ptr;
input_ptr = output_ptr;
output_ptr = temp_ptr;
for (i=0 ; i< PTS ; i++) (process_ptr + i)->imag = 0.0;
for (i=PTS/2 ; i< PTS ; i++) (process_ptr + i)->real = 0.0;
fft(process_ptr,PTS,twiddle); //transform samples
//into frequency domain
for (i=0 ; i<PTS ; i++) //filter in frequency domain
{ //i.e. complex multiply
a = (process_ptr + i)->real; //samples by coeffs
b = (process_ptr + i)->imag;
(process_ptr + i)->real = coeffs[i].real*a
- coeffs[i].imag*b;
(process_ptr + i)->imag = -(coeffs[i].real*b
+ coeffs[i].imag*a);
}
fft(process_ptr,PTS,twiddle);
for (i=0 ; i<PTS ; i++)
{
(process_ptr + i)->real /= PTS;
(process_ptr + i)->imag /= -PTS;
}
for (i=0 ; i<PTS/2 ; i++) //overlap add (real part only)
{
(process_ptr + i)->real += (output_ptr + i + PTS/2)->real;
}
} // end of while
} //end of main()
