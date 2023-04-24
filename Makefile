LIBS    = `pkg-config --cflags --libs opencv4`

exercise1:
	g++ exercise1_binarization.cpp -o bin/binarization  $(LIBS)
	g++ exersice1_nosify.cpp -o bin/noise_filtering $(LIBS)

exercise2:
	g++ exercise2_edge_detection.cpp -o bin/edge_detection  $(LIBS)

all:
	g++ exercise1_binarization.cpp -o bin/binarization  $(LIBS)
	g++ exersice1_nosify.cpp -o bin/noise_filtering $(LIBS)
	g++ exercise2_edge_detetion.cpp.cpp -o bin/edge_detection  $(LIBS)

clean-ex1:
	rm bin/binarization
	rm bin/noise_filtering
	