# SYSC4001_A3P2

## Instructions
To compile, please run:
`gcc part2a_101150157_101220575.c -o a.out`

And then to run the program, run: 
`./a.out [NUMBER_OF_TAs] [PATH_TO_RUBRIC.TXT] [PATH_TO_GRADES_DIRECTORY]`

For example:
`./a.out 5 rubric.txt .`

Exam files should be named with the format "exam_x.txt", and contain only a student number. The included python script will generate these for you if you do not have any available.

## Design discussion
While you will notice a surreptitious lack of semaphores within this program, I will tell you that I would have:
- had TA processes acquire a semaphore relating to a particular question, so as to prevent two TAs from grading the same question
- checked for *any* remaining semaphors before cycling on to the next student's exam