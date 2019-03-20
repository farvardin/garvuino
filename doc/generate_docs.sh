

txt2tags readme.t2t
sed -i "s/|---------------|/|--------|-------|/g" ../README.md
make all
sed -i "s/|---------------|/|--------|-------|/g" garvuino_manual.md

pdfjoin --a4paper --fitpaper false --rotateoversize false garvuino_manual_cover.pdf garvuino_manual.pdf -o garvuino_manual-joined.pdf 
mv garvuino_manual-joined.pdf garvuino_manual.pdf

