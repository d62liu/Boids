all:
	g++ -Ofast -I src/include -L src/lib -o main main.cpp boid_main.cpp boid_interface.cpp -lmingw32 -lSDL2main -lSDL2

