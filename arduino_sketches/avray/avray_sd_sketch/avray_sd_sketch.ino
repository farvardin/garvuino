#include <SPI.h>
#include <SD.h>

const int CS_Pin = 4;

//int switchInPinBack = 7;  
int switchInPin = 6;   
int LedPin = 8;

File dataFile;
File entry,root;

void setup()
{

  //pinMode(switchInPinBack, INPUT_PULLUP);
  pinMode(switchInPin, INPUT_PULLUP);
  pinMode(LedPin, OUTPUT);


  pinMode(CS_Pin, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(CS_Pin)) return;

  root = SD.open("/",FILE_READ);  

  Serial.begin(57600);
}


#define BUF_MAX 16*15
static byte buf[BUF_MAX];
static byte buf2[29];
unsigned long t;


/// PLAY YRG FILE ==============================
void play_yrg()
{
   byte regbuf[28];
   byte bframes;
   
   while(dataFile.available())
   {
     // read 16 x16 regs
     bframes = dataFile.read(buf,BUF_MAX)/16;
     for (byte frame = 0; frame < bframes; frame++)
     {
       // send diff registers from current frame
       byte buf_p = 0;
       for (byte reg = 0; reg < 14; reg++)
       {
           if (reg == 13 && buf[frame*16+reg]==255) break;
           if(reg == 13 || buf2[reg] != buf[frame*16+reg]) {
             regbuf[buf_p++] = reg;
             regbuf[buf_p++] = buf[frame*16+reg];
           }
       }
       Serial.write(regbuf,buf_p); SwitchSong();
       memcpy(buf2,&buf[frame*16],14);
       //delay(20);
       while(millis() - t < 20); 
       t = millis();
     }
   }
}

/// PLAY RSF FILE ==============================
void play_rsf()
{
   //unsigned long frames, loopframe,curframe=0;
   word freq, offset;
   byte val, zeroes, ptr = 0, count, mask2, mask1, delay_time, skip;
   
   if( dataFile.read(buf,4) <= 0 ) return; // short file

   if(buf[0] != 'R' || buf[1] != 'S' || buf[2] != 'F') return; //not RSF
   
   switch(buf[3])
   { // reading RSF HEADER v3 only supported!
     case 3: // RSF ver.3
           if( dataFile.read(buf,14) == 0 ) return; // short file
           memcpy(&freq,&buf[0],sizeof(word));
           memcpy(&offset,&buf[2],sizeof(word));
           //memcpy(&frames,&buf[4],sizeof(unsigned long));
           //memcpy(&loopframe,&buf[8],sizeof(unsigned long));
           break;
     default:
           return;
   }

   if( freq > 1000 ) return; // frequency is too fast
   
   delay_time = 1000 / freq;
  
   // skip text info
   dataFile.seek(offset);

   // play song data
   if( (count = dataFile.read(buf,BUF_MAX)) <= 0 ) return;
   for(;;) {   
         if(ptr > count>>1)
         { // half buffer is already played move it and load more
           byte msize = count - ptr;
           memmove(buf,&buf[ptr], msize);
           if( (count = dataFile.read(&buf[msize],ptr)) <= 0 ) return;
           count += msize;
           ptr = 0;
         }
         val = buf[ptr++];
         skip = 1;       
         switch(val)
         {
           case 255:
               break;
           case 254:
               skip = buf[ptr++];
               if(ptr >= count) return;
               break;
           default:
               mask2 = val;
               mask1 = buf[ptr++];
               byte reg = 0, reg_p = 0;
               while( mask1 != 0)
               {
                   if(mask1 & 1) {
                     buf2[reg_p++] = reg;
                     buf2[reg_p++] = buf[ptr++];
                   }
                   mask1 >>= 1;
                   reg++;
               }
               reg = 8;
               while(mask2 != 0)
               {
                   if(mask2 & 1) {
                     buf2[reg_p++] = reg;
                     buf2[reg_p++] = buf[ptr++];
                   }
                   mask2 >>= 1;
                   reg++;
               }
               Serial.write(buf2,reg_p);
                SwitchSong();
         }             
         //curframe += skip;
         while(millis() - t < delay_time * skip); // delay 
         t = millis();
   }
}

void loop()
{  

  ReadFiles();
  
}



void SwitchSong() {
  
 //  int sensorVal = digitalRead(switchInPinBack); // push button
   int sensorVal = digitalRead(switchInPin); // push button
   // push button
   if (sensorVal == HIGH) {
    digitalWrite(LedPin, LOW);
    //entry = root.openNextFile();
  } else {
    digitalWrite(LedPin, HIGH);
delay(100);    
ReadFiles();
    delay(200);

  }
} 

void ReadFiles() {

  byte file_type;
    // reset AY
 memset(buf2,0,29);
 buf2[0] = 255;
 for(byte i = 0; i < 14; i++) { buf2[i*2+1]=i; }
 Serial.write(buf2,29);
 memset(buf2,0,29);

 for(;;) { // find file
    file_type = 0;
    if(entry) entry.close();
    entry = root.openNextFile();
    if(!entry) { // end of files on SD
       root.rewindDirectory();
       entry = root.openNextFile();
    }
    
    if(entry.isDirectory()) continue;

    if(strcasestr(entry.name(),".yrg")) {
      file_type = 1;
      break;
    }
    if(strcasestr(entry.name(),".rsf")) {
      file_type = 2;
      break;
    }
 }

 dataFile = SD.open(entry.name(),FILE_READ);

t = millis();
 
 switch(file_type)
 {
    case 1:
        play_yrg();
        break;
    case 2:
        play_rsf();
        break;
 }

 dataFile.close();
 entry.close();
 
}
