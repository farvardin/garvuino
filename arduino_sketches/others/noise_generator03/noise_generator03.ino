/* noise-lfsr.ino
 *  
 *  White Noise Generator
 *  
 *  approximating 16-bit, using 2 PWM outputs
 *  
 *  uses the skeleton code from adc_to_pwm.pde for the 16 bit output (see below), 
 *  using the pseudorandom noise generator algorithm at:
 *  https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Galois_LFSRs
 *  
 *  write-up at : https://elfquake.wordpress.com/2018/08/08/arduino-white-noise-generator/
 *  
 *  demo video at: https://youtu.be/RBWBqGr3Lzc
 *  
 *  Circuit:
 *  
 
D9 ---- 1k --------------|
D10 --- 200k --- 56k ----|---> OUT
                         |
                    10n ===
                         |
                         |
                        GND
 *  
 *  
 *  danny.ayers@gmail.com
 *  
// ADC to PWM converter
// guest - openmusiclabs.com - 1.9.13
// options table at http://wiki.openmusiclabs.com/wiki/PWMDAC
// takes in audio data from the ADC and plays it out on
// Timer1 PWM.  16b, Phase Correct, 31.25kHz - although ADC is 10b.
 *  */
 
#define PWM_FREQ 0x00FF // pwm frequency - see table
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 1 // number of pwms, either 1 or 2

uint16_t start_state = 0xACE1u;  /* Any nonzero start state will work. */
uint16_t lfsr = start_state;
unsigned period = 0;

uint16_t ramp = 0;

void setup() {


  // setup ADC
  ADMUX = 0x60; // left adjust, adc0, internal vcc
  ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
  ADCSRB = 0x07; // t1 capture for trigger
  DIDR0 = 0x01; // turn off digital inputs for adc0

  // setup PWM
  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
  TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK1 = 0x20; // interrupt on capture interrupt
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02); // turn on outputs

  sei(); // turn on interrupts - not really necessary with arduino
}

void loop() {
  while (1); // gets rid of jitter
}

ISR(TIMER1_CAPT_vect) {

  // get ADC data
  unsigned int temp1 = ADCL; // you need to fetch the low byte first
  unsigned int temp2 = ADCH;
  // although ADCH and ADCL are 8b numbers, they are represented
  // here by unsigned ints, just to demonstrate how you would
  // use numbers larger than 8b.  also, be sure you use unsigned
  // ints for this operation.  if you have a signed int (a regular
  // int), add 0x8000 and cast it to an unsigned int before sending
  // it out to OCR1AH or OCR1AL.
  // example:
  // int temp3 = 87;
  // unsigned int temp4 = temp3 + 0x8000;
  // OCR1AH = temp4 >> 8;
  // OCR1AL = temp4;

  unsigned lsb = lfsr & 1;   /* Get LSB (i.e., the output bit). */
  lfsr >>= 1;                /* Shift register */
  lfsr ^= (-lsb) & 0xB400u;

  temp1 = lfsr & 0xFF00u;
  temp2 = lfsr & 0x00FFu;


  // output high byte on OC1A
 // OCR1AH = temp2 >> 8; // takes top 8 bits
  OCR1AH = temp2 >> map(analogRead(0),0, 1023, 0, 16); // takes top 8 bits
  //OCR1AL = temp2; // takes bottom 8 bits
  OCR1AL = temp2*2/map(analogRead(1),0, 1023, 1, 16); // takes bottom 8 bits

  // output low byte on OC1B
  OCR1BH = temp1 >> 8;
  OCR1BL = temp1;
}
