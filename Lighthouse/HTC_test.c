#include <stdio.h>
#include "NU32.h"
#include <xc.h>
#include <string.h>
#include <math.h>
#define DEG_PER_CORE 0.0009 // equal to (180 deg) / (8333 us) / (24 core tick per us)
#define LIGHTHOUSEHEIGHT 7.0 // in feet
#define DEG_TO_RAD 0.01745 // pi/180

static char buffer[100] = "";  //define buffer
static int count = 0;  //count how many data has been used



// structure to store the sensor data
typedef struct {
  long changeTime[11];
  long prevMic; // used to detect timer overflow
  double horzAng;
  double vertAng;
  int useMe;
  int collected; // is changeTime full
} viveSensor;


volatile viveSensor V1;
volatile viveSensor V2;
volatile viveSensor V3;

void __ISR(_INPUT_CAPTURE_1_VECTOR, IPL3SOFT) No1SensorIC1(){
	//get the time the interrupt occured
    long mic = _CP0_GET_COUNT();
    int i;
    int unuse = IC1BUF;
    //check for coreTimer overflow
    while (mic < V1.prevMic ){
        mic = mic + 4294967295;  //largest unsigned 32bit int
    }
    //shift the time into buffer
    for(i = 0; i<10; i++){
        V1.changeTime[i] = V1.changeTime[i+1]; //shift every time to front position
    }
    V1.changeTime[10] = mic; //store the mic into the last position
    //if the buffer is not full 
    if(V1.collected < 11){
        V1.collected++;
    }else{//when the buffer is full, check data in buffer conforms to (sync sync sweep sync sweep)
        // if the timer values match the waveform pattern when about 7 feet away from the emitter
        if ((V1.changeTime[1] - V1.changeTime[0] > 7000 * 24) && (V1.changeTime[3] - V1.changeTime[2] > 7000 * 24) && (V1.changeTime[6] - V1.changeTime[5] < 50 * 24) && (V1.changeTime[10] - V1.changeTime[9] < 50 * 24)) {
            V1.horzAng = (V1.changeTime[5] - V1.changeTime[4]) * DEG_PER_CORE;
            V1.vertAng = (V1.changeTime[9] - V1.changeTime[8]) * DEG_PER_CORE;
            V1.useMe = 1; 

         //    double xPos = tan((V1.vertAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
         //    double yPos = tan((V1.horzAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
         //    sprintf(buffer,"x=%f, y= %f\r\n",xPos,yPos);
	        // NU32_WriteUART3(buffer);
        }
    }
    V1.prevMic = mic;
    IFS0bits.IC1IF = 0; //clear the interrupt flag
}
 
void __ISR(_INPUT_CAPTURE_2_VECTOR, IPL4SOFT) No2SensorIC2(){
	//get the time the interrupt occured
    long mic = _CP0_GET_COUNT();
    int i;
    int unuse = IC2BUF;
    // sprintf(buffer,"%d\n\r",_CP0_GET_COUNT());
    // NU32_WriteUART3(buffer);
    //check for coreTimer overflow
    while (mic < V2.prevMic ){
        mic = mic + 4294967295;  //largest unsigned 32bit int
    }
    //shift the time into buffer
    for(i = 0; i<10; i++){
        V2.changeTime[i] = V2.changeTime[i+1]; //shift every time to front position
    }
    V2.changeTime[10] = mic; //store the mic into the last position
    //if the buffer is not full 
    if(V2.collected < 11){
        V2.collected++;
    }else{//when the buffer is full, check data in buffer conforms to (sync sync sweep sync sweep)
        // if the timer values match the waveform pattern when about 7 feet away from the emitter
        if ((V2.changeTime[1] - V2.changeTime[0] > 7000 * 24) && (V2.changeTime[3] - V2.changeTime[2] > 7000 * 24) && (V2.changeTime[6] - V2.changeTime[5] < 50 * 24) && (V2.changeTime[10] - V2.changeTime[9] < 50 * 24)) {
            V2.horzAng = (V2.changeTime[5] - V2.changeTime[4]) * DEG_PER_CORE;
            V2.vertAng = (V2.changeTime[9] - V2.changeTime[8]) * DEG_PER_CORE;
            V2.useMe = 1; 

            double xPos = tan((V2.vertAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
            double yPos = tan((V2.horzAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
            sprintf(buffer,"x=%f, y= %f\r\n",xPos,yPos);
	        NU32_WriteUART3(buffer);
        }
    }
    V2.prevMic = mic;
    IFS0bits.IC2IF = 0; //clear the interrupt flag
}

void __ISR(_INPUT_CAPTURE_3_VECTOR, IPL5SOFT) No3SensorIC3(){
	//get the time the interrupt occured
    long mic = _CP0_GET_COUNT();
    int i;
    int unuse = IC3BUF;
    //check for coreTimer overflow
    while (mic < V3.prevMic ){
        mic = mic + 4294967295;  //largest unsigned 32bit int
    }
    //shift the time into buffer
    for(i = 0; i<10; i++){
        V3.changeTime[i] = V3.changeTime[i+1]; //shift every time to front position
    }
    V3.changeTime[10] = mic; //store the mic into the last position
    //if the buffer is not full 
    if(V3.collected < 11){
        V3.collected++;
    }else{//when the buffer is full, check data in buffer conforms to (sync sync sweep sync sweep)
        // if the timer values match the waveform pattern when about 7 feet away from the emitter
        if ((V3.changeTime[1] - V3.changeTime[0] > 7000 * 24) && (V3.changeTime[3] - V3.changeTime[2] > 7000 * 24) && (V3.changeTime[6] - V3.changeTime[5] < 50 * 24) && (V3.changeTime[10] - V3.changeTime[9] < 50 * 24)) {
            V3.horzAng = (V3.changeTime[5] - V3.changeTime[4]) * DEG_PER_CORE;
            V3.vertAng = (V3.changeTime[9] - V3.changeTime[8]) * DEG_PER_CORE;
            V3.useMe = 1; 

            // double xPos = tan((V3.vertAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
            // double yPos = tan((V3.horzAng - 90.0) * DEG_TO_RAD) * LIGHTHOUSEHEIGHT;
         //    sprintf(buffer,"x=%f, y= %f\r\n",xPos,yPos);
	        // NU32_WriteUART3(buffer);
        }
    }
    V3.prevMic = mic;
    IFS0bits.IC3IF = 0; //clear the interrupt flag
}


int main (){
    // initialize the sensor variables
    //Sensor No.1
    V1.prevMic = 0;
    V1.horzAng = 0;
    V1.vertAng = 0;
    V1.useMe = 0;
    V1.collected = 0;
    //Sensor No.2
    V2.prevMic = 0;
    V2.horzAng = 0;
    V2.vertAng = 0;
    V2.useMe = 0;
    V2.collected = 0;
    //Sensor No.3
    V3.prevMic = 0;
    V3.horzAng = 0;
    V3.vertAng = 0;
    V3.useMe = 0;
    V3.collected = 0;
	NU32_Startup();
	__builtin_disable_interrupts();
	//Sensor No.1
	TRISDbits.TRISD8 = 1; // connect the No.1 sensor's ENV to D8
    IC1CONbits.ICTMR = 1; //capture the time of timer 2, but we can ignore it, 
                           //because we use interrupt to store system time
    IC1CONbits.ICM = 0b110; //IC mode 6, trigger IC3 on every edge
    IC1CONbits.FEDGE = 0;   //first capture edge is falling edge
    IC1CONbits.ICI = 0;     //interrupt on every edge
    IC1CONbits.ON = 1; //trun IC1 on
    IFS0bits.IC1IF = 0; //clear intterupt falg
    IPC1bits.IC1IP = 3; //interrupt priority 3
    IEC0bits.IC1IE = 1; //enable IC1 interrupt
    //Sensor No.2
	TRISDbits.TRISD9 = 1; // connect the No.1 sensor's ENV to D8
    IC2CONbits.ICTMR = 1; //capture the time of timer 2, but we can ignore it, 
                           //because we use interrupt to store system time
    IC2CONbits.ICM = 0b110; //IC mode 6, trigger IC3 on every edge
    IC2CONbits.FEDGE = 0;   //first capture edge is falling edge
    IC2CONbits.ICI = 0;     //interrupt on every edge
    IC2CONbits.ON = 1; //trun IC2 on
    IFS0bits.IC2IF = 0; //clear intterupt falg
    IPC2bits.IC2IP = 4; //interrupt priority 4
    IEC0bits.IC2IE = 1; //enable IC2 interrupt
	//Sensor No.3
    TRISDbits.TRISD10 = 1; // connect the No.3 sensor's ENV to D10
    IC3CONbits.ICTMR = 1; //capture the time of timer 2, but we can ignore it, 
                           //because we use interrupt to store system time
    IC3CONbits.ICM = 0b110; //IC mode 6, trigger IC3 on every edge
    IC3CONbits.FEDGE = 0;   //first capture edge is falling edge
    IC3CONbits.ICI = 0;     //interrupt on every edge
    IC3CONbits.ON = 1; //trun IC3 on
    IFS0bits.IC3IF = 0; //clear intterupt falg
    IPC3bits.IC3IP = 5; //interrupt priority 5
    IEC0bits.IC3IE = 1; //enable IC3 interrupt
    __builtin_enable_interrupts();
    while (1){
    	;
    }
    return 0;
}