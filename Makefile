# default: g++ main.cpp rgbe/rgbe.c image_rgbe.cpp image_tga.cpp -o hdrComp
hdrComp: ./src/rgbe/rgbe.o ./src/image_tga.o ./src/image_rgbe.o ./src/main.o
	g++ -std=c++11 -o hdrComp ./src/rgbe/rgbe.o ./src/image_tga.o ./src/image_rgbe.o ./src/main.o

rgbe.o: ./src/rgbe/rgbe.c
	g++ -std=c++11 -c -o ./src/rgbe/rgbe.o ./src/rgbe/rgbe.c

image_rgbe.o: ./src/image_rgbe.c
	g++ -std=c++11 -c -o ./src/image_rgbe.o ./src/image_rgbe.c

image_tga.o: ./src/image_tga.c
	g++ -std=c++11 -c -o ./src/image_tga.o ./src/image_tga.c

main.o: ./src/main.c
	g++ -std=c++11 -c -o ./src/main.o ./src/main.c

clean:
	rm -rf *o hdrComp