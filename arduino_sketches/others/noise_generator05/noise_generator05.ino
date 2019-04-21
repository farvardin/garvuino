/**
 * references:
 * white-noise generation: https://code.google.com/p/greennoise/downloads/detail?name=prbsGen.pde&can=2&q=label%3AWhite+Noise
 * sleepduino: http://gareth.halfacree.co.uk/2012/02/introducing-the-sleepduino
 */

//  for noise generation
unsigned long int reg;

//  pin setup
const int buzzerPin = 9;
const int buttonPin = 6;
const int potPin = 6;

//  button controls
boolean noiseOn = false;
boolean stillPushed = false;
long lastUpdateTime = 0;
//const long debounce = 200;
const long debounce = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  // Arbitrary inital value; must not be zero
  reg = 0x55aa55aaL; //The seed for the bitstream. It can be anything except 0.
}

void loop() {
  //  check for button pushed (on/off control)
  boolean buttonPushed = (digitalRead(buttonPin) == LOW);
  if (buttonPushed && millis() - lastUpdateTime > debounce) {  //  wait for noise to clear
    noiseOn = !noiseOn;
    lastUpdateTime = millis();
  }
  
  if (noiseOn) {
    //  check pot value (volume control)
    int potValue = analogRead(potPin);  //  0 to 1023
    int frequency = map(potValue, 0, 1023, 0, 1000);
    generateNoise(frequency);
  }
}

void generateNoise(int frequency) {
  unsigned long int newr;
  unsigned char lobit;
  unsigned char b31, b29, b25, b24;
   
  // Extract four chosen bits from the 32-bit register
  b31 = (reg & (1L << 31)) >> 31;
 // b31 = (reg & (1L << map(analogRead(1), 0, 1023, 26, 31)))  >> map(analogRead(1), 0, 1023, 26, 31);
  b29 = (reg & (1L << 29)) >> 29;
  b25 = (reg & (1L << 25)) >> 25;
  b24 = (reg & (1L << 24)) >> 24;

  // EXOR the four bits together
//  lobit = b31 ^ b29 ^ b25 ^ b24 ;
  lobit = (b31*map(analogRead(0), 0, 1023, 0, 4)) ^ (b29*map(analogRead(1), 0, 1023, 0, 4)) ^ (b25*map(analogRead(2), 0, 1023, 0, 4)) ^ (b24 *map(analogRead(3), 0, 1023, 0, 4)) ;
  
  // Shift and incorporate new bit at bit position 0
  newr = (reg << 1) | lobit;
  
  // Replace register with new value
  reg = newr;
  
  // Drive speaker pin from bit 0 of 'reg'
  digitalWrite(buzzerPin, reg & 1);
  
  // Delay (50) corresponds to 20kHz, but the actual frequency of updates
  // will be lower, due to computation time and loop overhead
  delayMicroseconds(frequency);    // Changing this value changes the frequency.
}
