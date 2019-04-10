#include <iostream>
#include "sounds.h"
int main(){
  std::cout << "start";
  if(fork() == 0){
    execve();
  }
}
