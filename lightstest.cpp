#include <wiringPi.h>
int main(){
  wiringPiSetup();
  pinMode (0,OUTPUT);
  while(true){
    digitalWrite(0,HIGH); dely(500);
    digitalWrite(0,LOW); dely(500);
  }
  return 0;
}
