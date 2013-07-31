//flash_sine.c Sine generation to illustrate use of flash

#include "dsk6713_aic23.h" //codec-DSK support file
//set sampling rate 8Khz

short loop = 0, gain = 10;
short sine_table[8] = {0,707,1000,707,0,-707,-1000,-707};
interrupt void c_int11() //interrupt service routine
{
output_sample(sine_table[loop]*gain);
if (++loop > 7) //if end of buffer
loop = 0; //reset index
return;
}
void main()
{
comm_intr(); //init DSK, codec, McBSP
while(1); //infinite loop
}
