#include <iostream>
#include "sounds.h"
#include <signal.h>

int main(){
  signal(SIGCHLD, SIG_IGN); //needed to prevent zombie processes?
  std::cout << "start\n";
  left_sound();
  std::cout << "end\n";
  return 0;
}
