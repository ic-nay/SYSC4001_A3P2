#include <stdio.h>

int main(int argc, char *argv[]){
    
    if (argc != 3){
        printf("Incorrectly formatted arguments! Format is %s [NUMBER_OF_TAs] [PATH_TO_RUBRIC.TXT] [PATH_TO_GRADES_DIRECTORY]\n", argv[0]);
        return 1;
    }
    
    int TA_COUNT = atoi(argv[1]);
    if (TA_COUNT == 0){
        printf("Invalid NUMBER_OF_TAs! Must be an integer greater than 0.\n", argv[0])
        return 1;
    }

    return 0;
}