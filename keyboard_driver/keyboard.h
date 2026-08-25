#pragma once

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 256
#define KEY_MAX 128 


void execute_command(char*);

extern bool CAPS_LOCK;
extern bool SHIFT_PRESS;

extern bool input;



static const char lower_keyboard_map[] = {
      0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
      '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
      0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
      0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
      '*', 0, ' '
  };

    static const char upper_keyboard_map[] = {
      0,  27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
      '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
        0,  'A','S','D','F','G','H','J','K','L',':','"','~',
        0, '|','Z','X','C','V','B','N','M','<','>','?', 0,
        '*', 0, ' '
  };



  void keyboard_isr();
  char key_dequeue();
  void clear_ring();
  char get_pressed_char();