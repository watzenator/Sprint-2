#include "BrickPi3.cpp"
#include "BluetoothSocket.h"
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include "base_values.h"
#include "pid.h"
#include "gyro_mov.h"
#include <iostream>

BrickPi3 BP;

void forward(int motorspeed){
	BP.set_motor_power(PORT_C, motorspeed);
	BP.set_motor_power(PORT_B, motorspeed);
}

void left(int motorspeed){
	BP.set_motor_power(PORT_C, -motorspeed);
	BP.set_motor_power(PORT_B, motorspeed);
}

void right(int motorspeed){
	BP.set_motor_power(PORT_C, motorspeed);
	BP.set_motor_power(PORT_B, -motorspeed);
}

void brake(){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}



int main() {
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

	int32_t EncoderC = BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	int32_t EncoderB = BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));

	BluetoothServerSocket serversock(2, 1);  //2 is het channel-number
	std::cout << "listening" << std::endl;
	while(true) {
		BluetoothSocket* clientsock = serversock.accept();
		std::cout << "accepted from " << clientsock->getForeignAddress().getAddress() << std::endl;
		MessageBox& mb = clientsock->getMessageBox();

		//motor settings
		int speed = 50;


		std::string input;
		while(mb.isRunning()) {
			input = mb.readMessage();  //blokkeert niet
			//if(input != "") cout << endl << input << endl;
			//doe andere dingen.
			if(input == "UP"){
				forward(speed);
			}else if(input == "DOWN"){
				forward(-speed);
			}else if(input == "LEFT"){
				left(speed);
			}else if(input == "RIGHT"){
				right(speed);
			}else if(input == "FIRE"){
				brake();
			}else if(input == "A"){
				std::cout << "A";
			}else if(input == "B"){
				std::cout << "B";
			}else if(input == "C"){
				std::cout << "C";
			}

			//
			//cout << ".";
			//cout.flush();
			usleep(500000); // wacht 500 ms
		}

		clientsock->close();

	}
}
