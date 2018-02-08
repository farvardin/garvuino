#!/usr/bin/env bash


# garvuino 
for i in garvuino_gerber/*.gm1 ; do cp $i ${i%%.*}.gml ; done
	
for j in garvuino_gerber/*_pnp.txt ; do mv $j $j.old ; done
	
rm -fr garvuino_gerber_v2-10.zip
zip -r garvuino_gerber_v2-10.zip garvuino_gerber	

	
# expansion board
for i in expansion_board_gerber/*.gm1 ; do cp $i ${i%%.*}.gml ; done
	
for j in expansion_board_gerber/*_pnp.txt ; do mv $j $j.old ; done
	
rm -fr expansion_board_gerber_v2-10.zip
zip -r expansion_board_gerber_v2-10.zip expansion_board_gerber

	
