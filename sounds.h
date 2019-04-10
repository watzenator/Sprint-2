#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


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
    char* args[] = {(char*)"/usr/bin/aplay > /dev/null",(char*) "-q ./left.wav", NULL};
    execve(args[0], args, NULL);
    //kill(getpid(), SIGKILL);
    std::cout << "testing\n";
    //exit(0);
  }else{
    //parent
    //wait(NULL);
    return;
  }
}
