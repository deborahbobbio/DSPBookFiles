#include "dsk6713_aic23.h" //codec support
//set sampling rate 8KHz
#define N 2400000 //buffer size 300 secs
long i;
short buffer[N];
#pragma DATA_SECTION(buffer,".EXT_RAM") //buffer in ext memory
void main()
{
short recording = 0;
short playing = 0;
for (i=0 ; i<N ; i++) buffer[i] = 0;
DSK6713_DIP_init();
DSK6713_LED_init();
comm_poll(); //init DSK, codec
while(1) //infinite loop
{
if(DSK6713_DIP_get(3) == 0) //if SW#3 is pressed
{
i=0;
recording = 1; //start recording
while (recording == 1)
{
DSK6713_LED_on(3); //turn on LED#3
buffer[i++] = input_left_sample(); //input data
if (i>2000)
if (DSK6713_DIP_get(3)==1) //if SW#3 lifted
{
recording = 0; //stop recording
DSK6713_LED_off(3); //turn LED#3 off
}
}
}
if(DSK6713_DIP_get(0)==0) //if SW#0 is pressed
{
i=0;
playing = 1; //start playing
while (playing == 1)
{
DSK6713_LED_on(0); //turn on LED#0
output_left_sample(buffer[i++]); //output data
if (i>2000)
if (DSK6713_DIP_get(0) == 1) //if SW#1 is lifted
{
playing = 0; //stop playing
DSK6713_LED_off(0); //turn LED#0 off
}
}
}
}
}
