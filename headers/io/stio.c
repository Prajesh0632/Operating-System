#include "../../system/system_calls.h"
#include "stio.h"
#include <stdarg.h>



void int_to_str(char* int_buffer, int n) {

    char temp[32];
    int index = 0;
    int neg = (n < 0);

    if (neg) {
        index = 2;
        n = -n;
    }

    while(n != 0) {

        int rem = n % 10;
        temp[index++] = rem + '0';
        n /= 10;



    }
    
    
    int i = 0;
    if(neg)  i = 1;

    int j = index;

    while(i < index) {
        int_buffer[i++] = temp[--j];
    }

    if(neg) int_buffer[0] = '-';

    int_buffer[i] = '\0';


}



int print(char* format, ...){

   

    char buffer[1024];
    int index = 0;

    va_list args;
    va_start(args, format);


    for(int i = 0; format[i] != '\0'; i++) {

        if(format[i] == '%') {
            i++;

            if(format[i] == 'd') {
                int num = va_arg(args, int);
                char s_num[24];
                int_to_str(s_num,num);
                 for(int j = 0; s_num[j] != '\0'; j++) {
                    buffer[index++] = s_num[j];
                }

            } 

            else if(format[i] == 'c') {
                char c = (char)va_arg(args, int);
                buffer[index++] = c;
            }

            else if(format[i] == 's') {
                char* s = va_arg(args, char*);
                for(int j = 0; s[j] != '\0'; j++) {
                    buffer[index++] = s[j];
                }
            }

            else if(format[i] == '%') {
                buffer[index++] = '%';
            }
        }

        else {
            buffer[index++] = format[i]; 
        }
    }


    buffer[index] = '\0';
    va_end(args);

    sys_write(buffer);
   
   

    return index;

}
