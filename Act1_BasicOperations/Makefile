LIBS    = `pkg-config --cflags --libs opencv4`

ex1:
	g++ exercise1_binarization.cpp -o bin/binarization  $(LIBS)
	g++ exersice1_nosify.cpp -o bin/noise_filtering $(LIBS)

ex2:
	g++ exercise2_edge_detection.cpp -o bin/edge_detection  $(LIBS)

ex3:
	g++ exercise3_video_processing.cpp -o bin/video_processing  $(LIBS)

clean-ex1:
	rm bin/binarization
	rm bin/noise_filtering
	
clean-ex2:
	rm bin/edge_detection

clean-ex3:
	rm bin/video_processing

clean:
	rm bin/binarization
	rm bin/noise_filtering
	rm bin/edge_detection
	rm bin/video_processing

all:
	g++ exercise1_binarization.cpp -o bin/binarization  $(LIBS)
	g++ exersice1_nosify.cpp -o bin/noise_filtering $(LIBS)
	g++ exercise2_edge_detection.cpp -o bin/edge_detection  $(LIBS)
	g++ exercise3_video_processing.cpp -o bin/video_processing  $(LIBS)

