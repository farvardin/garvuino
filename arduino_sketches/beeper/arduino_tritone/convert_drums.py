import array
import os
import sys

wave=array.array('B')
size=0

if len(sys.argv)!=2:
	raise ValueError("Usage: convert_drums.py filename.bin")
	
try:
	size=os.path.getsize(sys.argv[1])
except:
	raise ValueError('File not found')

file=open(sys.argv[1],'rb')
wave.fromfile(file,size)
file.close()

size=wave[40]+(wave[41]<<8)+(wave[42]<<16)+(wave[43]<<24)

ptr=44
drum=0

while ptr<size-3:

	# seek for next drum, which is lack of silence for at least three samples

	while ptr<size-3:
	
		if wave[ptr]!=0x80 and wave[ptr+1]!=0x80 and wave[ptr+2]!=0x80:
			break
			
		ptr+=1
	
	if ptr>=size-3:
		break
		
	# process sample as drum until at least three non-maxed out samples
	
	print 'const unsigned char drum_sample_%i[] PROGMEM ={' % drum
	
	length=0
	out=0
	out_len=1

	str=''
	
	while ptr<size-3:
	
		if wave[ptr]>0x10 and wave[ptr]<0xf0 and wave[ptr+1]>0x10 and wave[ptr+1]<0xf0 and wave[ptr+2]>0x10 and wave[ptr+2]<0xf0:
			break

		if wave[ptr]<0x80:
			out=0
		else:
			out=1
			
		if length==0:
			prev_out=out
		
		if out==prev_out and out_len<255:
			out_len+=1
		else:
			str+='%i,'%out_len
			prev_out=out
			out_len=1
			
		ptr+=1
		length+=1

	str+='0'
	
	print str
	print '};\n'
	
	drum+=1
	
	# skip drum trail
	
	while ptr<size-3:
	
		if wave[ptr]==0x80 and wave[ptr+1]==0x80 and wave[ptr+2]==0x80:
			break
			
		ptr+=1
		
print 'const unsigned char* const drum_sample_list[] PROGMEM ={'

for i in range(0,drum):
	print '\tdrum_sample_%i,' % i

print '};\n'
