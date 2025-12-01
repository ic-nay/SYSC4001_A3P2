#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define RUBRICK_SIZE 5
#define MEM_SIZE 20*sizeof(char)

struct student_exam {
    int student_id;
    int grades[25]; //I'm tired, this will do.
};

void set_rubrick(char *rubrick, char *token, int *number_of_questions){
    int index = atoi(strtok(token, ","));
    if (index == 0){
        printf("Could not parse rubrick file!\n Please remember that only rubrick indexes between 1 and %i are permitted.\n", RUBRICK_SIZE);
        exit(1);
    }
    char* value = strtok(NULL, "\n");
    rubrick[index-1] = *value;
    if (index > *number_of_questions){
        *number_of_questions = index;
    }
}

int load_exam(char *exam_path, char *file_name, struct student_exam* exam, int number_of_questions){
    
    strcat(exam_path, "/");
    strcat(exam_path, file_name);
    FILE* exam_file_ptr = fopen(exam_path, "r");
    
    if (exam_file_ptr == NULL) {
        printf("Unable to open exam file %s\n", exam_path);
        return 1;
    }

    char buffer[10];

    fgets(buffer, 10, exam_file_ptr);

    exam->student_id = atoi(buffer);

    if (exam->student_id == 0){
        printf("Unable to parse student number (found %c). Please ensure your files only contain a single line with a 10-digit number", buffer);
        exit(1);
    }

    for (int i = 0; i < number_of_questions; i ++){
        exam->grades[i] = 0;
    }
    return 0;
}

//This ended up being a lot of params, whoops.
void grade_exam(char *rubrick, struct student_exam* exam, int number_of_questions, int current_exam, ){
    srand(time(NULL));
    int i;
    //Rubrick regrading
    for (i = 0; i < number_of_questions; i ++){
        sleep(((rand() % 5) + 5) / 10);
        if ((rand() % 2) == 1){
            rubrick[i] += 1;
            printf("%d: Changed rubrick to have question %d be %c\n", getpid(), i, rubrick[i]);
        }
    }
    for (i = 0; i < number_of_questions; i ++){
        if (exam->grades[i] == 0){
            sleep(((rand() % 10) + 10) / 10);
            printf("TA %d: Grading student %d, question %d\n", getpid(), current_exam, i);
            exam->grades[i] = getpid();
        }
    }
    current_exam += 1;

}

int main(int argc, char *argv[]){

    //used often for loops;
    int i = 0;


    //established before forking and unchanging, so does not need to be shared.
    int number_of_questions = 0;
    
    if (argc != 4){
        printf("Incorrectly formatted arguments! Format is %s [NUMBER_OF_TAs] [PATH_TO_RUBRIC.TXT] [PATH_TO_GRADES_DIRECTORY] [NUMBER_OF_EXAMS]\n", argv[0]);
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

    char* GRADE_DIRECTORY = argv[3];
    
    char buffer[50];

    // Shared Memory Creation for Rubrick

    int shmid_rubrick = shmget((key_t)1234, MEM_SIZE, 0666 | IPC_CREAT); //double the amount of memory I think we'll need, to be safe

    if (shmid_rubrick == -1) {
        printf("Failed to acquire shared memory!\n");
        exit(1);
    }

    char* rubrick = shmat(shmid_rubrick, (void *)0, 0);
    
    memset(rubrick,0,MEM_SIZE);

    char* token;

    while (fgets(buffer, 50, rubric_file_ptr)){
        token = strtok(buffer, "\n");
        while (token != NULL){
            set_rubrick(rubrick, token, &number_of_questions);
            token = strtok(NULL, "\n");
        }
    }
    
    fclose(rubric_file_ptr);
    
    for (i = 0; i < RUBRICK_SIZE; i ++){
        printf("Rubrick item %i is %c\n", i+1, rubrick[i]);
    }
    printf("Total of %d questions in rubrick\n", number_of_questions);

    int shmid_student = shmget((key_t)4321, MEM_SIZE, 0666 | IPC_CREAT); //double the amount of memory I think we'll need, to be safe

    if (shmid_student == -1) {
        printf("Failed to acquire shared memory!\n");
        exit(1);
    }

    struct student_exam *exam = shmat(shmid_student, (void *)0, 0);

    load_exam(GRADE_DIRECTORY, "exam_1.txt", exam, number_of_questions);

    printf("Student number in exam_1.txt is %d\n", exam->student_id);

    pid_t pids [TA_COUNT];

    for (i = 0; i < TA_COUNT; i ++){
        pids[i] = fork();
        if (pids[i] == 0){
            grade_exam(rubrick, exam, number_of_questions, 0);
            break;
        }
        printf("Process %d PID: %d\n", i, pids[i]);
    }
    /*

    1. Fork TAs
    2. Every TA, upon being forked, will go over and possibly change the rubrick
    3. Once they have changed the rubrick as they see fit, they will begin marking exams
        1. They open an exam, and begin marking a question. They must mark a question
        which has not already been marked, and is not being marked by another TA
        - For loop with breaks
        - must wait 1-2 seconds when marking
        2. After they mark an exam question, they will print the student number of the exam
        and the question number that was marked. Then they will mark another question.
        3. Once they reach


    - [x] make process
    - [ ] iterate over rubrick
        - [ ] wait random amount of time between 
            - [x] include random library
        - [ ] choose to make change
            - [ ] incremenent char by 1
        - 
    - 
    */

    return 0;
}