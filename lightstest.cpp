#include <wiringPi.h>
int main(){
  wiringPiSetup();
  pinMode (GPIO17,OUTPUT);
  while(true){
    digitalWrite(GPIO17,HIGH); dely(500);
    digitalWrite(GPIO17,LOW); dely(500);
  }
  return 0;
}
