build/truecolorTTY : truecolorTTY.c
	mkdir -p build
	gcc truecolorTTY.c -o build/truecolorTTY `pkg-config --cflags --libs imlib2 glib-2.0` -lm

clear : 
	rm -r build
