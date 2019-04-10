#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void left_sound(){
  if(fork() == 0){
    //child
    char* args[] = {(char*)"/usr/bin/aplay",(char*) "./left.wav",(char*)"-q", (char*) NULL};
    execv(args[0], &args[0]);
  }else{
    return;
  }
}
