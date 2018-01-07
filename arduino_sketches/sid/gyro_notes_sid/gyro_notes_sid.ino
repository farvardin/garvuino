/* Author = helscream (Omer Ikram ul Haq)
Last edit date = 2014-10-08
Website: http://hobbylogs.me.pn/?p=47
Location: Pakistan
Ver: 0.1 beta --- Start
Ver: 0.2 beta --- Bug fixed for calculating "angle_y_accel" and "angle_x_accel" in "Gyro_header.ino" file

// version for Garvuino https://github.com/farvardin/garvuino
// Works with Arduino IDE 1.8.1

Use a MpU6050 gyroscope
connect:
Gyro    Arduino
VCC     5V on bottom right pin in the group of 6 pins on the top
GND     GND on bottom left pin in the group of 6 pins on the top
SCL     A5
SDA     A4
INT     should be on D2 but can be left unconnected

*/
#include <SID.h>
#include "noteList.h"
#include "pitches.h"

#define OFF 0
#define SETTRIANGLE_1	4,0x11,5,0xBB,6,0xAA,
#define SETRECTANGLE_1  4,0x41,5,0x24,6,0xAA, 
#define SETSAWTOOTH_1 4,0x21,5,0xBB,6,0xAA, 
#define C4_1	1,0x11,0,0x25,
#define CONTROLREG 4 // SID control register address

// waveforms
#define SETNOISE_1 	4,0x81,5,0xBB,6,0xAD, // SID register setup to create noise

#define SETNOSOUND_1  4,0x00,5,0xBB,6,0xAD, // SID register setup to create no sound

byte inChannel = 1; // we can't change channel here at the moment
byte inChannel2 = 2; // we can't change channel here at the moment
byte inChannel3 = 3; // we can't change channel here at the moment

SID mySid;

int noteVolume=12;

#include <Wire.h>
#include "gyro_accel.h"
// Defining constants
#define dt 20                       // time difference in milli seconds
#define rad2degree 57.3              // Radian to degree conversion
#define Filter_gain 0.6             // e.g.  angle = angle_gyro*Filter_gain + angle_accel*(1-Filter_gain)
// *********************************************************************
//    Global Variables
// *********************************************************************
unsigned long t=0; // Time Variables
float angle_x_gyro=0,angle_y_gyro=0,angle_z_gyro=0,angle_x_accel=0,angle_y_accel=0,angle_z_accel=0,angle_x=0,angle_y=0,angle_z=0;
// *********************************************************************
// Main Code
void setup(){
  

  mySid.begin();
  
  
  Serial.begin(115200);
  Wire.begin();
  MPU6050_ResetWake();
  MPU6050_SetGains(0,1);// Setting the lows scale
  MPU6050_SetDLPF(0); // Setting the DLPF to inf Bandwidth for calibration
  MPU6050_OffsetCal();
  MPU6050_SetDLPF(6); // Setting the DLPF to lowest Bandwidth
  

  
  Serial.print("gyro_x_scalled");
  Serial.print("\tgyro_y_scalled");
  Serial.print("\tgyro_z_scalled");
  
  Serial.print("\taccel_x_scalled");
  Serial.print("\taccel_y_scalled");
  Serial.print("\taccel_z_scalled");
  
  Serial.print("\tangle_x_gyro");
  Serial.print("\tangle_y_gyro");
  Serial.print("\tangle_z_gyro");
  
  Serial.print("\tangle_x_accel");
  Serial.print("\tangle_y_accel");
  Serial.print("\tangle_z_accel");
  
  Serial.print("\tangle_x");
  Serial.print("\tangle_y");
  Serial.print("\tangle_z");
  
  Serial.println("\tLoad");

  
    
  t=millis(); 

}


void setwaveform_triangle(uint8_t channel)
{
  uint8_t dataset[]={ SETTRIANGLE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
  }
}


void setwaveform_rectangle(uint8_t channel)
{
  uint8_t dataset[]={ SETRECTANGLE_1 C4_1 0xFF };
//uint8_t dataset[]={ SETTRIANGLE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
     mySid.set_register(24,noteVolume); // SET VOLUME
  }
}

void setwaveform_sawtooth(uint8_t channel)
{
  uint8_t dataset[]={ SETSAWTOOTH_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
     mySid.set_register(24,noteVolume); // SET VOLUME
  }
}

void setwaveform_noise(uint8_t channel)
{
  uint8_t dataset[]={ SETNOISE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
     mySid.set_register(24,noteVolume); // SET VOLUME
  }
}


void setwaveform_nosound(uint8_t channel)
{
  uint8_t dataset[]={ SETNOSOUND_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
  }
}



  /************************************************************************
	
    void set_frequency(uint16_t pitch,uint8_t channel)
    
    Set sid frequncy registers.
    
    The pitch has to be:
    
    pitch=16.77*frequency
    
    The SID has 3 channels, which should be accesed according to this
    definition:
    
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    
************************************************************************/
// pitch=16.77*frequency
void set_frequency(uint16_t pitch,uint8_t channel)
{
    mySid.set_register(channel, pitch&0xFF); // low register adress
    mySid.set_register(channel+1, pitch>>8); // high register adress
    }

uint8_t zufall()
{
	static uint16_t startwert=0x0AA;

	uint16_t temp;
	uint8_t n;
		
	for(n=1;n<8;n++)
	{
		temp = startwert;
		startwert=startwert << 1;
	
		temp ^= startwert;
		if ( ( temp & 0x4000 ) == 0x4000 ) 
		{ 
			startwert |= 1;
		}
	}
	
	return (startwert);
}
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    
    
void loop(){
  
    uint16_t ny;
    uint16_t nz;  
    uint16_t nx;
    
  uint8_t flag,k;
  uint16_t soundindex=0;

  

 // setwaveform_triangle(CHANNEL1);
  //setwaveform_triangle(CHANNEL2);
  setwaveform_triangle(CHANNEL3);


  //setwaveform_rectangle(CHANNEL1);
  setwaveform_rectangle(CHANNEL2);
  //setwaveform_rectangle(CHANNEL3);


  setwaveform_sawtooth(CHANNEL1);
 //setwaveform_sawtooth(CHANNEL2);
  //setwaveform_sawtooth(CHANNEL3);

/*
  setwaveform_noise(CHANNEL1);
  setwaveform_noise(CHANNEL2);
  setwaveform_noise(CHANNEL3);
*/
    
  while(1)
  {
    byte currentNote = 0;
    
    ny=angle_y*8;
    // set_frequency(ny*17,CHANNEL1); // change noise generator frequency
    currentNote = map(ny, -90, 90, 40, 60);
    //currentNote = constrain(currentNote, 40, 60);
    set_frequency(sNotePitches[currentNote],CHANNEL1); 
    //Serial.print(ny);
    //Serial.print("\t");
    
    nz=angle_z*8;
    //set_frequency(nz*17,CHANNEL2); // change noise generator frequency
    currentNote = map(nz, -90, 90, 60, 80);
    //currentNote = constrain(currentNote, 60, 80);
    set_frequency(sNotePitches[currentNote],CHANNEL2); 
    
    
    nx=angle_x*8;
    //set_frequency(nx*17,CHANNEL3); // change noise generator frequency
    currentNote = map(nx, -90, 90, 60, 90);
    //currentNote = constrain(currentNote, 60, 90);
    set_frequency(sNotePitches[currentNote],CHANNEL3); 

    delay(100);
    t=millis(); 
  
  MPU6050_ReadData();

    digitalWrite(13, HIGH); //turn on debugging LED
    digitalWrite(8, HIGH); //turn on debugging LED
 
  angle_x_gyro = (gyro_x_scalled*((float)dt/1000)+angle_x);
  angle_y_gyro = (gyro_y_scalled*((float)dt/1000)+angle_y);
  angle_z_gyro = (gyro_z_scalled*((float)dt/1000)+angle_z);

  angle_z_accel = atan(accel_z_scalled/(sqrt(accel_y_scalled*accel_y_scalled+accel_x_scalled*accel_x_scalled)))*(float)rad2degree;
  angle_y_accel = -atan(accel_y_scalled/(sqrt(accel_y_scalled*accel_y_scalled+accel_z_scalled*accel_z_scalled)))*(float)rad2degree;
  angle_x_accel = atan(accel_x_scalled/(sqrt(accel_x_scalled*accel_x_scalled+accel_z_scalled*accel_z_scalled)))*(float)rad2degree;

  angle_x = Filter_gain*angle_x_gyro+(1-Filter_gain)*angle_x_accel;
  angle_y = Filter_gain*angle_y_gyro+(1-Filter_gain)*angle_y_accel;
  angle_z = Filter_gain*angle_z_gyro+(1-Filter_gain)*angle_z_accel;
  
  

  Serial.print(gyro_x_scalled);
  Serial.print("\t");
  Serial.print(gyro_y_scalled);
  Serial.print("\t");
  Serial.print(gyro_z_scalled);
  Serial.print("\t");


  Serial.print(accel_x_scalled);
  Serial.print("\t");
  Serial.print(accel_y_scalled);
  Serial.print("\t");
  Serial.print(accel_z_scalled);
  Serial.print("\t");
 
  Serial.print(angle_x_gyro);
  Serial.print("\t");
  Serial.print(angle_y_gyro);
  Serial.print("\t");
  Serial.print(angle_z_gyro);
  Serial.print("\t"); 
   
  Serial.print(angle_x_accel);
  Serial.print("\t");
  Serial.print(angle_y_accel);
  Serial.print("\t");
  Serial.print(angle_z_accel);
  Serial.print("\t"); 


  Serial.print(angle_x);
  Serial.print("\t");
  Serial.print(angle_y);
  Serial.print("\t");
  Serial.print(angle_z);
  Serial.print("\t");
  
   

  Serial.println(((float)(millis()-t)/(float)dt)*100);


   // digitalWrite(8, LOW); //turn off debugging LED

  
  while((millis()-t) < dt){ // Making sure the cycle time is equal to dt
  // Do nothing
  }
  
  
  }
  

}




  
