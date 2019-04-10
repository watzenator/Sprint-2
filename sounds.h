#include <iostream>
#include <string>
#include <unistd.h>


// void kill_all_sound(){
//
// }
//
// void forward_sound(){
//   if(fork() = 0){
//     //child
//     char* args[] = {(char*)"/usr/bin/aplay",(char*) "forward.wav", NULL}
//     execv(args[0], args);
//     kill(getpid(), SIGKILL);
//   }else{
//     //parent
//     return
//   }
// }

void left_sound(){
  if(fork() == 0){
    //child
    char* args[] = {(char*)"/usr/bin/aplay",(char*) "./left.wav", NULL}
    execv(args[0], args);
    //kill(getpid(), SIGKILL);
    exit(0);
  }else{
    //parent
    return;
  }
}
