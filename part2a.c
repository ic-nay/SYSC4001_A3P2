#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define RUBRICK_SIZE 5

void set_rubrick(char *rubrick, char *token){
    int index = atoi(strtok(token, ","));
    if (index == 0 || index > RUBRICK_SIZE){
        printf("Could not parse rubrick file!\n Please remember that only rubrick indexes between 1 and %i are permitted.\n", RUBRICK_SIZE);
        exit(1);
    }
    char* value = strtok(NULL, "\n");
    rubrick[index-1] = *value;
}

int main(int argc, char *argv[]){

    //used often for loops;
    int i = 0;
    
    if (argc != 4){
        printf("Incorrectly formatted arguments! Format is %s [NUMBER_OF_TAs] [PATH_TO_RUBRIC.TXT] [PATH_TO_GRADES_DIRECTORY]\n", argv[0]);
        return 1;
    }
    
    int TA_COUNT = atoi(argv[1]);

    if (TA_COUNT == 0){
        printf("Invalid NUMBER_OF_TAs! Must be an integer greater than 0.\n");
        return 1;
    }

    FILE* rubric_file_ptr;

    rubric_file_ptr = fopen(argv[2], "r");

    if (rubric_file_ptr == NULL) {
        printf("Unable to open rubrick file!\n");
        return 1;
    }
    
    char buffer[50]; //Should be more than enough, but better safe than sorry

    /*
    Okay I want to iterate over each line of this file, splitting it by 
    newlines. I'm gonna
    - allocate an array [x]
    - split by newlines [x]
    - split by comma [x]
    - convert first portion to an int (add zero check) [x]
    - assign second portion to index of first portion [x]
    */

    //It'd be nice if this could be dynamic, but that is nto possible in C
    char rubrick[RUBRICK_SIZE];
    rubrick[0] = *"Q";
    rubrick[1] = *"W";
    rubrick[2] = *"X";
    rubrick[3] = *"Y";
    rubrick[4] = *"Z";


    char* token;

    while (fgets(buffer, 50, rubric_file_ptr)){
        token = strtok(buffer, "\n");
        while (token != NULL){
            //printf("Token (in main): %s\n", token);
            set_rubrick(rubrick, token);
            token = strtok(NULL, "\n");
        }
    }
    
    
    for (i = 0; i < RUBRICK_SIZE; i ++){
        printf("Rubrick item %i is %c\n", i+1, rubrick[i]);
    }

    

    return 0;
}