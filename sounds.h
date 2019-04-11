#pragma once

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

void right_sound(){
  if(fork() == 0){
    //child
    char* args[] = {(char*)"/usr/bin/aplay",(char*) "./right.wav",(char*)"-q", (char*) NULL};
    execv(args[0], &args[0]);
  }else{
    return;
  }
}

void sirene_sound(){
  if(fork() == 0){
    //child
    char* args[] = {(char*)"/usr/bin/aplay",(char*) "./SirenePi.wav",(char*)"-q", (char*) NULL};
    execv(args[0], &args[0]);
  }else{
    return;
  }
}
