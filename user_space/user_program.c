#include "../headers/io/stio.h"


void user_prog() {

char c = 'a';
  print("Enter a Character : ");
  scan("%c", &c);   
  print("\nThe character is %c", c); 
}