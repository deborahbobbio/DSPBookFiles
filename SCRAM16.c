// SCRAM16k_poll.c Voice Scrambler Program
#include "C6211dsk.h"
#include “C6211dskinit.h” // DSK support file
#include "sine.h" // file with sine data values
#include "coeff.h" // coefficient file
short processdata(short data);
short filter(short inp,short *mem);
short MultSine(short input);
static short filter1[COEFF],filter2[COEFF];
short input, output;
void main()
{
int i;
comm_poll(); // init DSK/codec
for( i=0; i< COEFF; i++)
Input
3 kHz
LP Filter
3 kHz
LP Filter
3.3 kHz
Sine
Generator
Output
Multiplier
A
B
C
{
filter1[i] = 0; // init buffer for filtered input signal - 1
st
filter
filter2[i] = 0; // init buffer for modulated signal - 2
nd
filter
}
while(1)
{
input = input_sample(); // input data from codec
processdata(input); // process the sample twice to upsample
output = processdata(input); // and throw away the first result
output_sample(output); // to decimate, then output
}
}
short processdata(short data)
{
data = filter(data,filter1);
// call filter function – 1
st
filter
data = MultSine(data); // call function to generate sine and modulate
data = filter(data,filter2); // call filter function – 2
nd
filter
return data;
}
short filter(short inp,short *mem) // implements lowpass filter
{
int j;
long acc;
mem[COEFF-1] = inp; // bottom memory for newest sample
acc = mem[0] * coeff[0]; // y(0) = x[n-(COEFF-1)] * h[COEFF-1]
for (j = 1; j < COEFF; j++)
{
acc += mem[j] * coeff[j]; // y(n) = x[n-(COEFF-1-j)] * h[COEFF-1-j]
mem[j-1] = mem[j]; // update delay samples (x(n+1-i)=x(n-i)
}
acc = ((acc)>>15); // scale result
return acc; // return y(n) at time n
}
short MultSine(short input) // sine generation and modulation function
{
static int j=0;
input = (input * sine[j++]) >> 11; // input to multiplier * sine data
if(j>=SINE) j = 0;
return input; // return modulated signal
}
