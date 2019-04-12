IDIR =../include
CC=g++

PIDgrid: PIDgrid.o sounds.o signalhandler.o base_values.o gyro_mov.o grid.o
	CC -o PIDgrid PIDgrid.o sounds.o signalhandler.o base_values.o gyro_mov.o grid.o

Brickpi3.o: Brickpi3.h Brickpi3.cpp
	CC -c Brickpi3.cpp

sounds.o: sounds.hpp sounds.cpp
	CC -c sounds.cpp

signalhandler.o: signalhandler.hpp signalhandler.cpp gyro_mov.o base_values.hpp Brickpi3.o
	CC -c signalhandler.cpp

pid.o: pid.hpp pid.cpp Brickpi3.o base_values.hpp
	CC -c pid.cpp

gyro_mov.o: gyro_mov.hpp gyro_mov.cpp Brickpi3.o base_values.hpp signalhandler.o pid.o sounds.o
	CC -c gyro_mov.cpp

grid.o: grid.hpp grid.cpp base_values.hpp Brickpi3.o pid.o gryo_mov.o
	CC -c grid.cpp

.PHONY: clean

clean:
	rm *.o