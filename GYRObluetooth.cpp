#include "base_values.h" //BrickPi3.cpp is included here
#include "signalhandler.h"
#include "pid.h"
#include "gyro_mov.h"
#include "grid.h"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>
#include <string>

int main(){
    signal(SIGINT, exit_signal_handler); // register the exit function for ctrl+c
	signal(SIGCHLD, SIG_IGN); //needed to prevent zombie processes?
 	BP.detect(); // make sure that the brickpi3 is communicating and that the firmware is compatible with the drivers.

	if(!voltageIsSafe()){
		std::cout << "battery almost empty, exiting program...";
		BP.reset_all();
		exit(-5);
	}

	BP.set_sensor_type(PORT_4, SENSOR_TYPE_EV3_GYRO_ABS);

	sensor_gyro_t Gyro4;

	while(BP.get_sensor(PORT_4, &Gyro4)){}
    //bluetooth
    BluetoothServerSocket serversock(2, 1);  //2 is het channel-number
	while(true) {
		BluetoothSocket* clientsock = serversock.accept();
		std::cout << "accepted from " << clientsock->getForeignAddress().getAddress() << std::endl;
		MessageBox& mb = clientsock->getMessageBox();
			
		std::string input;
		while(mb.isRunning()) {
			input = mb.readMessage();  //blokkeert niet
			if(input){
                if(input == "Left"){
                    goleft();
                }else if(input == "Right"){
                    goright();
                }else if(input == "Up"){
                    //goforward();
                }else if(input == "Down"){
                    turnaround();
                }else if(input == "Fire"){
                    
                }else if(input == "A"){

                }else if(input == "B"){

                }else if(input == "C"){

                }else{
                    std::cout << "ERROR, UNKNOWN MESSAGE RECEIVED\n";
                }
                input.clear();
            }
			usleep(BASE_SLEEP);
		}	
		clientsock->close();	
    }
}