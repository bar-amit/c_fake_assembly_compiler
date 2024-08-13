#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#include<stdio.h>

#include "../include/util.h"
#include "../include/constants.h"
#include "../include/allocate_memory.h"


char* remove_space(char* str){
    char c = *(str + strlen(str) - 1);
    if(isspace(c))
        *(str + strlen(str) - 1) = 0;
    return str;
}

char *clean_whitespace(char *str){
    int i=0, j=0, space_flag=0;
    char *tmp = (char *) allocate_memory(sizeof(char) * MAX_LINE_LENGTH, "clean_whitespace");
    while(str[i]){
        if((isspace(str[i])!=0) && space_flag==0){
            space_flag = 1;
            if(j>0){
                tmp[j] = ' ';
                j++;
            }
        }
        else if(isspace(str[i])==0){
            tmp[j] = str[i];
            j++;
            space_flag = 0;
        }
        i++;
    }
    if(space_flag==1 && j>0)
        j--;
    tmp[j] = '\n';
    tmp[j+1] = 0;
    strcpy(str, tmp);
    free(tmp);
    return str;
}
