//fir3lp.c FIR using 3 lowpass coefficients with different BW

#include "lp600.cof" //coeff file LP @ 600 Hz
#include "lp1500.cof" //coeff file LP @ 1500 Hz
#include "lp3000.cof" //coeff file LP @ 3000 Hz
#include "dsk6713_aic23.h" //codec-dsk support file
//set sampling rate 8Khz


short LP_number = 0; //start with 1st LP filter
int yn = 0; //initialize filter output
short dly[N]; //delay samples
short h[3][N]; //filter characteristics
interrupt void c_int11() //ISR
{
short i;
dly[0] = input_left_sample(); //newest input
yn = 0; //initialize filter output
for (i = 0; i< N; i++)
yn +=(h[LP_number][i]*dly[i]); //y(n) += h(LP#,i)*x(n-i)
for (i = N-1; i > 0; i--) //start @ bottom of buffer
dly[i] = dly[i-1]; //update delays
output_left_sample(yn >> 15); //output filter
return; //return from interrupt
}
void main()
{
short i;
for (i=0; i<N; i++)
{
dly[i] = 0; //init buffer
h[0][i] = hlp600[i]; //start of LP600 coeffs
h[1][i] = hlp1500[i]; //start of LP1500 coeffs
h[2][i] = hlp3000[i]; //start of LP3000 coeffs
}
comm_intr(); //init DSK, codec, McBSP
while(1); //infinite loop
}
