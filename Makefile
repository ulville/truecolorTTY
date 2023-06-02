build/truecolorTTY : truecolorTTY.c
	mkdir -p build
	gcc -lm truecolorTTY.c -o build/truecolorTTY `imlib2-config --cflags` `imlib2-config --libs`

clear : 
	rm -r build
