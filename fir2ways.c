//fir2ways.c FIR with alternative ways of storing/updating samples
#include "DSK6713_AIC23.h" //codec support
//set sampling rate 8KHz

#include "BP41.cof" //BP coeff centered at Fs/8
#define METHOD 'A' //or change to B
int yn = 0; //initialize filter's output
short dly[N]; //delay samples array
#if METHOD == 'B'
short *start_ptr;
short *end_ptr;
short *h_ptr;
short *dly_ptr;
#endif
interrupt void c_int11() //ISR
{
short i;
yn = 0; //initialize filter's output
#if METHOD == 'A'
dly[0] = input_left_sample();
for (i = 0; i< N; i++) yn += (h[i] * dly[i]);
for (i = N-1; i > 0; i--) dly[i] = dly[i-1];
#elif METHOD == 'B'
*dly_ptr = input_left_sample();
if (++dly_ptr > end_ptr) dly_ptr = start_ptr;
for (i = 0; i < N ; i++)
{
dly_ptr++;
if (dly_ptr > end_ptr) dly_ptr = start_ptr;
yn += *(h_ptr + i)* *dly_ptr;
}
#endif
output_left_sample((short)(yn>>15));
return;
}
void main()
{
#if METHOD == 'B'
dly_ptr = dly;
start_ptr = dly;
end_ptr = dly + N - 1;
h_ptr = h;
#endif
comm_intr();
while(1);
}
