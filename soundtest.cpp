#include <iostream>
#include "sounds.h"
#include <signal.h>

int main(){
  signal(SIGCHLD, SIG_IGN);
  // std::cout << "start\n";
  // left_sound();
  // std::cout << "end\n";
  system("aplay left.wav");
  std::cout << "end\n";
  return 0;
}
