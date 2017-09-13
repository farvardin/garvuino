# a very basic XM music module parsing library, by Shiru
# v1.01 09.02.17 faster
# v1.0  08.02.17

import array
import os
import sys



class XM:

	c_size=5				#256 patterns by 256 rows by 32 channels by 5 params
	r_size=256*c_size
	p_size=256*r_size
	s_size=256*p_size
	
	song=[0]*s_size


	pattern_len=array.array('i')

	data=array.array('B')
	size=0
	
	order_len=0
	order_loop=0
	channels=0
	patterns=0
	tempo=0
	bpm=0

	def read_word(self,off):
		return self.data[off]+(self.data[off+1]<<8)
		
	def read_dword(self,off):
		return self.read_word(off)+(self.read_word(off+2)<<16)

	def load(self,filename):

		# load file
		
		try:
			self.size=os.path.getsize(filename)
		except:
			raise ValueError('File not found')

		file=open(filename,'rb')
		self.data.fromfile(file,self.size)
		file.close()
		
		# check module signature
		
		if self.data[0:17]!=array.array('B','Extended Module: '):
			raise ValueError('Not an XM module')

		# read module parameters
		
		self.order_len =self.read_word(60+4)
		self.order_loop=self.read_word(60+6)
		self.channels  =self.read_word(60+8)
		self.patterns  =self.read_word(60+10)
		self.tempo	   =self.read_word(60+16)
		self.bpm	   =self.read_word(60+18)

		# convert pattern data into array for fast access
		
		pattern_len=array.array('i')
		for ptn in range(0,self.patterns):
		
			# get pattern offset
			
			off=60+20+256
			
			for pto in range(0,ptn):
			
				off=off+self.read_dword(off)+self.read_word(off+7)
				
			len=self.read_word(off+5)
			
			self.pattern_len.append(len)
			
			off=off+self.read_dword(off)

			# parse pattern data
			
			for row in range(0,len):
			
				for chn in range(0,self.channels):
				
					if self.data[off]&0x80:
					
						tag=self.data[off]
						off+=1
						
						note      =0
						instrument=0
						volume    =0
						effect    =0
						parameter =0
						
						if tag&0x01:
							note=self.data[off]
							off+=1
							
						if tag&0x02:
							instrument=self.data[off]
							off+=1
							
						if tag&0x04:
							volume=self.data[off]
							off+=1
							
						if tag&0x08:
							effect=self.data[off]
							off+=1
							
						if tag&0x10:
							parameter=self.data[off]
							off+=1
					else:
					
						note      =self.data[off+0]
						instrument=self.data[off+1]
						volume    =self.data[off+2]
						effect    =self.data[off+3]
						parameter =self.data[off+4]
						off+=5

					self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+0]=note
					self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+1]=instrument
					self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+2]=volume
					self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+3]=effect
					self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+4]=parameter
		
		return
		
	# get pattern ID number for given order list position
	
	def get_order(self,pos):
		if (pos<0) or (pos>=self.order_len):
			raise ValueError('Wrong order position')
		return self.data[60+20+pos]

	# get pattern length for given pattern ID
	
	def get_pattern_length(self,ptn):
		if (ptn<0) or (ptn>=self.patterns):
			raise ValueError('Wrong pattern number')
		return self.pattern_len[ptn]

	# get note number for given field
	# the number is 0..97, 0 means no note, 1...96 is C-1 to B-8, 97 is the key off

	def get_note(self,ptn,row,chn):
		return self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+0]

	# instrument number is 0...128, 0 means no instrument change
	
	def get_instrument(self,ptn,row,chn):
		return self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+1]

	# volume value is 16-80 (sub 16 to get actual value), 0 means no volume change
	
	def get_volume(self,ptn,row,chn):
		return self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+2]

	# effect number is 0..something
	
	def get_effect(self,ptn,row,chn):
		return self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+3]

	# effect parameter is 0..255

	def get_parameter(self,ptn,row,chn):
		return self.song[ptn*self.p_size+row*self.r_size+chn*self.c_size+4]
