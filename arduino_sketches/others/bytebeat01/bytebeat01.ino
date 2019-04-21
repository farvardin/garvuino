// Based on http://arduino.cc/en/Tutorial/Fading by David A. Mellis
// and Tom Igoe, http://www.arduino.cc/playground/Code/PwmFrequency,
// and viznut's, skurk's, and raer's discoveries documented in
// http://countercomplex.blogspot.com/2011/10/algorithmic-symphonies-from-one-line-of.html

#define INTERACTIVE


int speakerPin = 9; // Speaker connected to digital pin 9

void setup() {
// use 0x01 for divisor of 1, i.e. 31.25kHz
// or 0x02 for divisor of 8, or 0x03 for divisor of 64 (default)
TCCR1B = TCCR1B & 0xf8 | 0x01;
randomSeed(analogRead(0));
}

int t = 0;
#ifdef INTERACTIVE
  char i = 0;
  char k = 0;
#else
  int i = 0;
  int k = 0;
#endif




void loop() {
#ifdef INTERACTIVE
  if (++i != map(analogRead(0),0, 1023, 2, 4)) return;
  //if (++i != 64) return;
#else 
  if (++i != 64) return;
#endif

i = 1;
t++;
//k = random(8,32);
//  (t*(t>>5|t>>8)>>(t>>16))
// (t*(t>>5|t>>8))>>(t>>16)
// 2*t^(t*t>>8|(t*14)>>8|t*2&t*3) & t*((t>>45)+1)
#ifdef INTERACTIVE
  analogWrite(speakerPin, /*votre symphonie entre là -->*/ (t*(t>>5|t>>map(analogRead(0),0, 1023, 8, 2)))>>(t>>16)  /*<-- et là */  );
#else
  analogWrite(speakerPin, /*votre symphonie entre là -->*/ (t*(t>>5|t>>8))>>(t>>16)  /*<-- et là */  );
#endif
}
