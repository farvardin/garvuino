from xmlib import *



# original Octore runs at Z80 @ 3.5 Mhz, the main loop takes 275 cycles, that gives 12727 Hz sample rate

sample_rate=(3500000.0/275.0)
#sample_rate=(3500000.0/145.0)

# note frequencies for a single octave, to generate table of counter values

note_frequency=[2093.0,2217.4,2349.3,2489.0,2637.0,2793.8,2959.9,3135.9,3322.4,3520.0,3729.3,3951.0]



# load file

if len(sys.argv)<2:
	raise ValueError("Usage: convert_arduino_octode.py filename.xm")

module=XM()
module.load(sys.argv[1])

if module.order_len==0:
	raise ValueError("Error: Module should have at least one order position")

if module.channels<8:
	raise ValueError("Error: Module should have at least eight channels")

	
	
# generate frequency table
	
frequency_table=[0]*8*12

div=32.0

for octave in range(0,8):

	for note in range(0,12):
	
		step=sample_rate*2.0/(note_frequency[note]/div)
		
		if step>=240:
			step=0
			
		frequency_table[octave*12+note]=int(step)

	div/=2


	
# convert module

row_val=[0]*32
row_pre=[0]*32

# default tempo and BPM

tempo=module.tempo
bpm  =module.bpm
	
# convert pattern data first

for ptn in range(0,module.patterns):

	# seek for speed change effect in the first row of a pattern
	
	for chn in range(0,module.channels):
		
		row_val[chn]=0
		row_pre[chn]=0
		
		if module.get_effect(ptn,0,chn)==0x0f:
		
			param=module.get_parameter(ptn,0,chn)
			
			if param<0x20:
				tempo=param
			else:
				bpm=param
	
	# calculate current speed value, measured in samples at the player's sample rate
	
	speed=int(2500.0*tempo*(sample_rate/1000.0)/bpm)
		
	if (speed<1) or (speed>=65535):
		print 'Warning: Tempo or BPM is out of range (ptn:%s)' % ptn

	print 'const unsigned char music_pattern_%2.2x[] PROGMEM ={' % ptn
	print '\t0x%2.2x,0x%2.2x,' % (speed&255,speed/256)
	
	# convert pattern text
		
	for row in range(0,module.get_pattern_length(ptn)):
	
		line=''
	
		drum=0
		
		for chn in range(0,module.channels):
		
			note      =module.get_note      (ptn,row,chn)
			instrument=module.get_instrument(ptn,row,chn)
			volume    =module.get_volume    (ptn,row,chn)
			effect    =module.get_effect    (ptn,row,chn)
			parameter =module.get_parameter (ptn,row,chn)
			
			# only check the first 8 channels, although module can have extra channels to place drum notes
			
			if chn<8:
			
				row_val[chn]=row_pre[chn]

				# a new note
				
				if (note>0) and (note<97) and (instrument==1):

					row_val[chn]=frequency_table[note-1]
					
					# check for the detune effect
					
					if (effect==0x0e) and ((parameter&0xf0)==0x50):
					
						row_val[chn]+=(parameter&15)-8
						
						if (row_val[chn]<0) or (row_val[chn]>=240):
						
							if row_val[chn]<0:
								row_val[chn]=0
							if row_val[chn]>239:
								row_val[chn]=239;
								
							print 'Warning: Note out of range (ptn:%2.2 row:%2.2 chn:%i)' % ptn,row,chn

				# key off
				
				if note==97:
					row_val[chn]=0
					
				row_pre[chn]=row_val[chn]
				
			# drums can be placed into any channel
			
			if instrument==2:
			
				if note==49:		# C-4
					drum=240
				if note==51:		# D-4
					drum=241
				if note==53:		# E-4
					drum=242
				if note==54:		# F-4
					drum=243
				if note==56:		# G-4
					drum=244
				if note==58:		# A-4
					drum=245
				if note==60:		# B-4
					drum=246
				if note==61:		# C-5
					drum=247

		if drum!=0:
			line+= '\t0x%2.2x,' % drum
		else:
			line+= '\t     '

		line+='0x%2.2x,0x%2.2x,0x%2.2x,0x%2.2x,0x%2.2x,0x%2.2x,0x%2.2x,0x%2.2x,' % (row_val[0],row_val[1],row_val[2],row_val[3],row_val[4],row_val[5],row_val[6],row_val[7])
		
		print line
	
	print '\t0xff'
	print '};\n'

# convert order list

print('const unsigned char* const music_order_list[] PROGMEM ={')

for pos in range(0,module.order_len):

	print '\tmusic_pattern_%2.2x,' % module.get_order(pos);

print '\tNULL'
print '};\n'

print 'const unsigned char music_loop_position=%i;\n' % module.order_loop
