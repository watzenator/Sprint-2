#include "base_values.h" //BrickPi3.cpp is included here
// #include "BrickPi3.h"
#include "signalhandler.h"
#include "pid.h"
#include "gyro_mov.h"
#include "grid.h"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>

#define MOTORSPEED 0

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, exit_signal_handler); // register the exit function for ctrl+c
    BP.detect(); // make sure that the brickpi3 is communicating and that the firmware is compatible with the drivers.
    if(!voltageIsSafe()){
	std::cout << "battery almost empty, exiting program...";
	BP.reset_all();
	exit(-5);
    }


    BP.set_sensor_type(PORT_1, SENSOR_TYPE_EV3_GYRO_ABS);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);

    sensor_ultrasonic_t Ultrasonic3;
    sensor_gyro_t gyro4;

    BP.set_motor_power(PORT_C, MOTORSPEED);
    BP.set_motor_power(PORT_B, MOTORSPEED);

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
	fprintf(stderr,"ERROR, no port provided\n");
	exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
	error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
	error("ERROR on accept");
    bzero(buffer,256);
    while(true){
	n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	buffer[n] = '\0';
	int i = atoi(buffer);
	i = i/5;
	std::cout << "i: " << i << std::endl;
	BP.set_motor_power(PORT_C, MOTORSPEED - i);
	BP.set_motor_power(PORT_B, MOTORSPEED + i);

	printf("Here is the message: %s n: %i \n", buffer, n);
	if(buffer[0] == '!'){
	    close(newsockfd);
	    close(sockfd);
	    return 0;
	}
    }
}
