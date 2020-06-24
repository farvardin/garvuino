
// Simple beep generator for Garvuino
// https://github.com/farvardin/garvuino/



int switchInPin = 6; // Switch connected to digital pin 6
int speakerPin = 9; // Speaker connected to digital pin 9 (connect the other end of the speaker to the GND)

#define LED 13      // on board led
#define LED2 8      // external led

int reading;   // variable to store the button's state

// define your own values there:

int pre_delay = 2000;      // delay before sending the beep (in ms)
int beep_frequency = 432;  // frequency of the beep. A4 = 440
int beep_duration = 1400;   // how long will last the beep (in ms)
int beep_repetition = 4;   // how many time shall we repeat the beep
int beep_interval = 500;   // time in ms between two beeps

 


void setup() {
  // put your setup code here, to run once:
  pinMode(switchInPin, INPUT_PULLUP); // uses internal arduino resistor
}

void loop() {
  // put your main code here, to run repeatedly:
  reading = digitalRead(switchInPin);

  // switch
  if (reading == HIGH  ) {
    // do nothing while switch is not activated
  }
    else
      { led_blink(4);
        delay(pre_delay-200); // we remove 200 which is the delay added by the led blinking
        for (byte j=0;j<beep_repetition;j++)
        {
          make_beep();
          delay(beep_interval+beep_duration);  // tone is running in parallel with delay so we add them together
        }
      }
}


void make_beep() {
  
  for (byte j=0;j<beep_repetition;j++)  // make a loop
  {
    digitalWrite(speakerPin,LOW);       // make sure the speaker is off
    tone(speakerPin, beep_frequency, beep_duration);   // turn speaker on
  }
}


void led_blink(byte num)         // Basic blink function
{
    for (byte i=0;i<num;i++)
    {
        digitalWrite(LED,HIGH);
        digitalWrite(LED2,HIGH);
        delay(20);
        digitalWrite(LED,LOW);
        digitalWrite(LED2,LOW);
        delay(30);
    }
}


