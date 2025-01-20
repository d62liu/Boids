all:
	g++ -I src/include -L src/lib -o main main.cpp boid_main.cpp boid_main.h boid_interface.cpp boid_interface.h font8x8_basic.h -lmingw32 -lSDL2main -lSDL2
