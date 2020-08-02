for i in *.vgm
do 
 ../../tools/vgm2rsf/vgm2rsf "$i" -o ${i%.*}.rsf
done