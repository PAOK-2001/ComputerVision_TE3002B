LIBS    = `pkg-config --cflags --libs opencv4`

exercise1:
	g++ exercise1_binarization.cpp -o bin/binarization  $(LIBS)
	g++ exersice1_nosify.cpp -o bin/noise_filtering $(LIBS)

all:
	g++ exercise1_binarization.cpp -o bin/binarization $(LIBS)

clean-ex1:
	rm bin/binarization
	rm bin/noise_filtering
	