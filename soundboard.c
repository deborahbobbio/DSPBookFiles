//Soundboard.c Core C program for sound effects

union {Uint32 uint; short channel[2];} AIC23_data;
union {Uint32 uint; short channel[2];} AIC23_input;
short EchoLengthB = 8000; //echo delay
short EchoBuffer[8000]; //create buffer
short echo_type = 1; //to select echo or delay
short Direction = 1; //1->longer echo,-1->shorter
short EchoMin=0,EchoMax=0; //shortest/longest echo time
short DistMag=0,DistortionVar=0,VolSlider=100,PreAmp=100,DistAmp=10;
short HarmBuffer[3001]; //buffer
short HarmLength=3000; //delay of harmonics
float output2;
short DrumOn=0,iDrum=0,sDrum=0; //turn drum sound when = 1
int DrumDelay=0,tempo=40000; //delay counter/drum tempo
short ampDrum=40; //volume of drum sound
.. //addtl casting
interrupt void c_int11() //ISR
{
AIC23_input.uint = input_sample(); //newest input data
input=(short)(AIC23_input.channel[RIGHT]+AIC23_input.channel[LEFT])/2;
input = input*.0001*PreAmp*PreAmp;
output=input;
output2=input; //distortion section
if (output2>0)
output2=0.0035*DistMag*DistMag*DistMag*((12.35975*(float)input)
- (0.359375*(float)input*(float)input));
else output2 =0.0035*DistMag*DistMag*DistMag*(12.35975*(float)input
+ 0.359375*(float)input*(float)input);
output2/=(DistMag+1)*(DistMag+1)*(DistMag+1);
if (output2 > 32000.0) output2 = 32000.0 ;
else if (output2 < -32000.0 ) output2 = -32000.0;
output= (output*(1/(DistMag+1))+output2); //overall volume slider
input = output; //echo/reverb section
iEcho++; //increment buffer count
if (iEcho >= EchoLengthB) iEcho = 0; //if end of buffer reinit
output=input + 0.025*EchoAmplitude*EchoBuffer[iEcho];//newest+oldest
if(echo_type==1) EchoBuffer[iEcho] = output; //for decaying echo
else EchoBuffer[iEcho]=input; //for single echo (delay)
EchoLengthB += Direction; //alter the echo length
if(EchoLengthB<EchoMin+100){Direction=1;} //echo delay is shortest->
if(EchoLengthB>EchoMax){Direction=-1;} //longer,if longest->shorter
input=output; //output echo->harmonics gen
if(HarmBool==1) { //everyother sample...
HarmBool=0; //switch the count
HarmBuffer[iHarm]=input; //store sample in buffer
if(HarmBool2==1){ //everyother sample...
HarmBool2=0; //switch the count
HarmBuffer[uHarm] += SecHarmAmp*.025*input;//store sample in buffer
}
else{HarmBool2=1; uHarm++; //or just switch the count,
if(uHarm>HarmLength) uHarm=0; //and increment the pointer
}
}
