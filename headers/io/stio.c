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


void str_to_int(char* str_buffer, int* n) {

   

    int result = 0;
    for(int i = 0; str_buffer[i] != '\0'; i++) {
        result = result * 10 + (str_buffer[i] - 48);
    }

    *n = result;


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



int scan(char* format, ...){




    int count = 0;
    int types[100] = {0};


    for(int i = 0; format[i] != '\0'; i++) {

        if(format[i] == '%') {
            i++;

            if(format[i] == 'd') {
                types[count++] = 0;
                

            } 

            else if(format[i] == 'c') {
                types[count++] = 1;
            }

            else if(format[i] == 's') {
                types[count++] = 2;
                
            }
            
           

            
        }

       
    }







  
    
    int type_idx = 0;

    char line_buffer[128];

    int index = 0;




    sys_clear(); //clears the keyboard buffer and starts fresh after scanf


    int input_count = 0;

    va_list args;
    va_start(args, format);


    for(;;) {

        if(type_idx == count) break;
        char c = sys_read();
        if(c == '\0') continue;
        if(c == '\b') {

            if(input_count == 0) continue;
            else input_count--;
        }
        char s[2] = {c, '\0'};
        sys_write(s);

        
        
        if(types[type_idx] == 0 && (c == ' ' || c == '\n')) {

            int* n = va_arg(args, int*);
            line_buffer[index] = '\0';
            str_to_int(line_buffer, n);
            index = 0;
            type_idx++;
            continue;
        }

        if(types[type_idx] == 1 && (c == ' ' || c == '\n') && index > 0) {

            char* c = (char*)va_arg(args, int*);
            
            *c = line_buffer[0];
            index = 0;
            type_idx++;
            continue;
        }

        if(types[type_idx] == 2 && (c == '\n')) {
            char* s = (char*)va_arg(args, char*);
            line_buffer[index] = '\0';
            for(int i = 0; line_buffer[i] != '\0'; i++) {
                s[i] = line_buffer[i];
            }

            s[index] = '\0';
            index = 0;
            type_idx++;
            input_count = 0;
            continue;
        }

        
        line_buffer[index++] = c;
        if( c != '\b')input_count++;





        






        
    }




    va_end(args);;


}