//Phaser1 (digital drums) beeper engine port from ZX Spectrum to Arduino
//by Shiru (shiru@mail.ru) 14.02.17

// version for Garvuino https://github.com/farvardin/garvuino
// Works with Arduino IDE 1.8.3

//include music data generated from the binary Beepola output (without player) and bin2h.py

//#include "music_data_mister_beep.h"
#include "music_data_garvalf.h"

//include sample data

#include "drum_sample_data.h"



//general settings

#define COMPILE_ADR           40000             //address for compiled Beepola module, default is 40000

#define SAMPLE_RATE           (3500000/146*2)   //original Phaser1 runs at 23972 but uses sample interleaving, doubling the output rate

#define SPEAKER_PORT          PORTB             //speaker is on the port B
#define SPEAKER_DDR           DDRB
#define SPEAKER_BIT           (1<<1)            //PD7 (Uno pin 9)

#define MUSIC_FRAME           2048              //now many samples in one tempo unit

#define LED 8


//note table, stored in the PROGMEM
//calculated at compile time depending from the sample rate, to make experimenting with it easier
//the magic numbers is the note frequencies of the 8th octave multiplied by 65536

#define NOTE_C(octave)  ((274334351/SAMPLE_RATE)>>(6-octave))
#define NOTEhC(octave)  ((290646917/SAMPLE_RATE)>>(6-octave))
#define NOTE_D(octave)  ((307929415/SAMPLE_RATE)>>(6-octave))
#define NOTEhD(octave)  ((326240174/SAMPLE_RATE)>>(6-octave))
#define NOTE_E(octave)  ((345639485/SAMPLE_RATE)>>(6-octave))
#define NOTE_F(octave)  ((366192230/SAMPLE_RATE)>>(6-octave))
#define NOTEhF(octave)  ((387967221/SAMPLE_RATE)>>(6-octave))
#define NOTE_G(octave)  ((411037204/SAMPLE_RATE)>>(6-octave))
#define NOTEhG(octave)  ((435478855/SAMPLE_RATE)>>(6-octave))
#define NOTE_A(octave)  ((461373440/SAMPLE_RATE)>>(6-octave))
#define NOTEhA(octave)  ((488808120/SAMPLE_RATE)>>(6-octave))
#define NOTE_B(octave)  ((517873991/SAMPLE_RATE)>>(6-octave))

const unsigned int note_table[5*12] PROGMEM ={
	NOTE_C(1),NOTEhC(1),NOTE_D(1),NOTEhD(1),NOTE_E(1),NOTE_F(1),NOTEhF(1),NOTE_G(1),NOTEhG(1),NOTE_A(1),NOTEhA(1),NOTE_B(1),
	NOTE_C(2),NOTEhC(2),NOTE_D(2),NOTEhD(2),NOTE_E(2),NOTE_F(2),NOTEhF(2),NOTE_G(2),NOTEhG(2),NOTE_A(2),NOTEhA(2),NOTE_B(2),
	NOTE_C(3),NOTEhC(3),NOTE_D(3),NOTEhD(3),NOTE_E(3),NOTE_F(3),NOTEhF(3),NOTE_G(3),NOTEhG(3),NOTE_A(3),NOTEhA(3),NOTE_B(3),
	NOTE_C(4),NOTEhC(4),NOTE_D(4),NOTEhD(4),NOTE_E(4),NOTE_F(4),NOTEhF(4),NOTE_G(4),NOTEhG(4),NOTE_A(4),NOTEhA(4),NOTE_B(4),
	NOTE_C(5),NOTEhC(5),NOTE_D(5),NOTEhD(5),NOTE_E(5),NOTE_F(5),NOTEhF(5),NOTE_G(5),NOTEhG(5),NOTE_A(5),NOTEhA(5),NOTE_B(5)
};

//accumulator and adders for tone channels, channel 1 has two oscillators

unsigned char channel_active;

unsigned int channel_1a_acc;
unsigned int channel_1a_add;
unsigned int channel_1b_acc;
unsigned int channel_1b_add;

unsigned int channel_2_acc;
unsigned int channel_2_add;

unsigned char channel_1_out;
unsigned char channel_2_out;

//previous speaker output state

unsigned char output_state;

//'drum' sample player variables

unsigned int  drum_ptr;
unsigned char drum_sample;

//sync counter to syncronize the music parser with the synth code
//it is volatile because it the main thread reads it back

volatile unsigned int parser_sync;

//song parser variables

const unsigned char *order_list;
unsigned char order_pos;
unsigned char order_loop;
unsigned char order_length;

unsigned int pattern_ptr;

unsigned int instrument_ptr;



//initialize all

void setup()
{
	cli();

	//initialize song parser

	order_pos   =0;
	order_loop  =pgm_read_byte_near(music_data+0); //Beepola stores order loop and length <<1
	order_length=pgm_read_byte_near(music_data+1);

	order_list=music_data+4+(pgm_read_byte_near(music_data+2)+(pgm_read_byte_near(music_data+3)<<8));

	instrument_ptr=4;

	output_state=0;
	parser_sync=0;

	song_new_pattern();

	//initialize tone and drum synth variables

	channel_active=0;

	channel_1a_acc=0;
	channel_1b_acc=0;
	channel_1a_add=0;
	channel_1b_add=0;
	channel_2_acc=0;
	channel_2_add=0;

	channel_1_out=0;
	channel_2_out=0;

	drum_ptr=0;
	drum_sample=0;      //this is not sample number, but bit mask, so 0 means no drum is playing

	//set a port pin as the output

	SPEAKER_DDR|=SPEAKER_BIT; 

	//set timer2 to generate interrupts at the sample rate

	TCCR2A=0;   
	TCCR2B=0;
	TCNT2 =0;

	TCCR2A|=(1<<WGM21);
	TCCR2B|=(0<<CS02)|(1<<CS01)|(0<<CS00);  //prescaler=8
	TIMSK2|=(1<<OCIE2A);

	OCR2A=16000000/8/SAMPLE_RATE;

	sei();
}



ISR(TIMER2_COMPA_vect)
{
	unsigned int acc;

	//output the output state generated in the previous interrupt call as early as possible
	//that's to avoid jitter, because outputting the value near end of the interrupt handler
	//would be happening at slightly different times depending from the counters state

	SPEAKER_PORT=output_state;

	//also decrement sync variable early, although this isn't that important, no noticeable jitter possible

	if(parser_sync) --parser_sync;

	if(!drum_sample)
	{
		if(channel_active)
		{
			acc=channel_1a_acc;
			
			channel_1a_acc+=channel_1a_add;

			if(channel_1a_acc<acc) channel_1_out^=SPEAKER_BIT;
			
			acc=channel_1b_acc;
			
			channel_1b_acc+=channel_1b_add;

			if(channel_1b_acc<acc) channel_1_out^=SPEAKER_BIT;

			output_state=channel_1_out;
		}
		else
		{
			acc=channel_2_acc;
			
			channel_2_acc+=channel_2_add;

			if(channel_2_acc<acc) channel_2_out^=SPEAKER_BIT;

			output_state=channel_2_out;
		}

		channel_active^=1;
	}
	else
	{
		//play drum sample using the drum_sample variable as bit mask
		
		if(pgm_read_byte_near(drum_sample_data+(drum_ptr>>1))&drum_sample) output_state=SPEAKER_BIT; else output_state=0;

		++drum_ptr;
   digitalWrite(LED,HIGH);

		if(drum_ptr>=1024*2) drum_sample=0; //drum_ptr increments in half steps to compensate doubled sample rate
	}
}



void song_new_pattern(void)
{
	pattern_ptr=(pgm_read_byte_near(order_list+order_pos)+(pgm_read_byte_near(order_list+order_pos+1)<<8))-COMPILE_ADR;

	order_pos+=2;

	if(order_pos>=order_length) order_pos=order_loop;
}



unsigned char song_data_read(void)
{
	unsigned char n;

	n=pgm_read_byte_near(music_data+pattern_ptr);

	++pattern_ptr;

	return n;
}



//main loop

void loop()
{
	unsigned char tag,channel;
	unsigned int add,off;
	bool done;

	while(1)
	{
		//update row, parse song data

		channel=0;
		done=false;

		while(!done)
		{
			tag=song_data_read();

			if(!tag)  //end of a pattern, move to the next one
			{
				song_new_pattern();
				continue;
			}
			
			if(tag&0x80)//0=wait for next row, otherwise a parameter
			{
				switch(tag&0x3f)
				{
				case 60:    //key off
					if(!channel)
					{
						channel_1a_add=0;
						channel_1b_add=0;
						channel_1_out=0;

						channel=1;
					}
					else
					{
						channel_2_add=0;
						channel_2_out=0;
					}
					break;
					
				case 61:    //instrument change
					instrument_ptr=4+(song_data_read()<<1); //in the original data instument number stored <<1, one instrument takes 4 bytes
					break;
					
				case 62:    //second channel
					channel=1;
					break;
					
				case 63:    //loop position, not implemented in Beepola version
					break;
					
				default:     //note

					off=note_table+(tag&0x3f);
					
					add=pgm_read_byte_near(off)+(pgm_read_byte_near(off+1)<<8);
					
					if(!channel)
					{
						off=pgm_read_byte_near(music_data+instrument_ptr+1)+(pgm_read_byte_near(music_data+instrument_ptr+2)<<8);
						
						channel_1a_add=add;
						channel_1b_add=(add<<pgm_read_byte_near(music_data+instrument_ptr+0))+off;  //multiple and detune

						if(tag&0x40)  //optional phase reset
						{
							channel_1a_acc=0;
							channel_1b_acc=pgm_read_byte_near(music_data+instrument_ptr+3)<<8; //phase
							channel_1_out=0;
						}

						channel=1;
					}
					else  //always reset phase on the channel 2, to hear note restart instead of continuous tone
					{
						channel_2_acc=0;
						channel_2_add=add;
						channel_2_out=0;
					}
				}
			}
			else
			{
				//end of a row, set the parser sync value according to the wait value (in tempo units)

				if(tag<118)
				{
					parser_sync=MUSIC_FRAME*tag;  //wait for given number of tempo units before playing next row
				}
				else  //118..127 is a drum
				{
					drum_ptr=0;
         //PORTA ^= 0x03;        // turn off one of the LEDs and turn on the other one
        // digitalWrite(LED,LOW);
					drum_sample=1<<(tag-118); //bit mask for the drum sample

					parser_sync=MUSIC_FRAME*1;  //drum always take one tempo unit, so it normally followed by the wait command
				
				}
				
				done=true;
				//digitalWrite(LED,HIGH);
			}
		}

		//wait for the next row
		//delay 1 is important, by some reason sng tempo starts to jump a lot without it

		while(parser_sync>0) delay(1);
	}

}
