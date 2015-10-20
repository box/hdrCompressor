# default: g++ main.cpp rgbe/rgbe.c image_rgbe.cpp image_tga.cpp -o hdrComp
hdrCompressor: ./src/rgbe/rgbe.o ./src/image_tga.o ./src/image_rgbe.o ./src/main.o
	g++ -std=c++11 -o hdrCompessor ./src/rgbe/rgbe.o ./src/image_tga.o ./src/image_rgbe.o ./src/main.o

rgbe.o: ./src/rgbe/rgbe.c
	g++ -std=c++11 -c -o ./src/rgbe/rgbe.o ./src/rgbe/rgbe.c

image_rgbe.o: ./src/image_rgbe.cpp
	g++ -std=c++11 -c -o ./src/image_rgbe.o ./src/image_rgbe.cpp

image_tga.o: ./src/image_tga.cpp
	g++ -std=c++11 -c -o ./src/image_tga.o ./src/image_tga.cpp

main.o: ./src/main.cpp
	g++ -std=c++11 -c -o ./src/main.o ./src/main.cpp

clean:
	rm -rf *o hdrCompressor