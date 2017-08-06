#!/usr/bin/env bash

for i in garvuino_gerber/*.gm1 ; do mv $i $i.gml ; done
	
for j in garvuino_gerber/*_pnp.txt ; do mv $j $j.old ; done
	
zip -r garvuino_gerber_v9.zip garvuino_gerber	

	