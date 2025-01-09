/* Program to compute scores for the 2024 Lecturing Olympics.

   Skeleton written by Alistair Moffat, ammoffat@unimelb.edu.au,
   April 2024, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Kerui Huang 1463475
   Dated:     15/4/24

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**********************************************************************/

#define FAILED -1
#define SUCCESS 0
#define FIRST_INDEX 0
#define ROUNDING    1000
#define ROUNDING_OFFSET 0.500001 /* Added Epsilon to counter rounding errors */
#define ALPHA_OFFSET 'A'
#define COUNT_OFFSET 1

#define COMPETITOR_LIMIT    26
#define SCORE_LIMIT    50
#define LECTURES_LIMIT    500
#define EXPECTED_INPUT_CHARS 2
#define ACCEPTED_LEN 3
#define DISCARDED_SCORES 2
#define TOP_THREE 3

/**********************************************************************/

void    dub_swap(double *p1, double *p2);
void    char_swap(char *p1, char *p2); 
int     same_decimals(double v1, double v2);
double  round_double(double value);

void    sort_dub_array(double A[], int len);
void    sort_parallel_array(double A[], char B[], int len);
double  calculate_score(double A[], int len);
double  get_adjusted_score(double *scores, int scores_len);

int  read_line(char *lect, double *last_score, int *len, double *last_line);
void to_parallel_array(char last_lect, double last_score, char *lect_id, 
                       double lect_scores[][LECTURES_LIMIT], int *lect_scored, 
                       int *lect_count, int *adjusted_count);
void adj_to_parallel_array(double *lect_adj_scores, 
                           double lect_scores[][LECTURES_LIMIT], 
                           char *lect_id, int *lect_scored, int *lect_count);

void do_stage1(int class_count, int adjusted_count, char last_lect, 
               double *last_line, int last_len, double last_score);
void do_stage2(char *lect_id, int *lect_scored, double *lect_adj_scores, 
               int lect_count);
void do_stage3(char *lect_id, double *lect_adj_scores, int lect_count);



/**********************************************************************/

int
main(int argc, char *argv[]) {
    int class_count=0, adjusted_count=0, lect_count=0, last_len=0, 
        lect_scored[COMPETITOR_LIMIT]={0};
    double last_score, last_line[SCORE_LIMIT], 
       lect_adj_scores[COMPETITOR_LIMIT], 
       lect_scores[COMPETITOR_LIMIT][LECTURES_LIMIT];
    char last_lect, lect_id[COMPETITOR_LIMIT];
    
    /* First, we'll read the text one line at a time until the end */
    while(read_line(&last_lect, &last_score, &last_len, last_line) != FAILED){
        class_count++;        
        /* We transfer each line's information to a set of parallel arrays */
        to_parallel_array(last_lect, last_score, lect_id, lect_scores, 
            lect_scored, &lect_count, &adjusted_count); 
    }
    
    /* After reading all lines into arrays, we'll find the adjusted scores*/
    adj_to_parallel_array(lect_adj_scores, lect_scores, lect_id, lect_scored, 
        &lect_count);
    
    /* Finally, we can print out required information using recorded data */
    do_stage1(class_count, adjusted_count, last_lect, last_line, last_len, 
        last_score); 
    do_stage2(lect_id, lect_scored, lect_adj_scores, lect_count);
    do_stage3(lect_id, lect_adj_scores, lect_count); 

    printf("ta daa!\n");
    
    return 0;
}

/**********************************************************************/

/* Swap the doubles 
*/
void
dub_swap(double *p1, double *p2) {
    double tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

/**********************************************************************/

/* Swap the characters 
*/
void
char_swap(char *p1, char *p2) {
    char tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

/**********************************************************************/

/* Compare two doubles and see if they are the same to log_10(ROUNDING)
   decimal places; so when ROUNDING is 1000, to three decimal places
*/
int
same_decimals(double v1, double v2) {
    int r1, r2;
    r1 = (int)(0.5 + ROUNDING * v1);
    r2 = (int)(0.5 + ROUNDING * v2);
    return r1==r2;
}

/**********************************************************************/

/* Round a double value accurately to log_10(ROUNDING)
   decimal places by applying a rounding offset */
double  
round_double(double value){
    return floor(value * ROUNDING + ROUNDING_OFFSET) / ROUNDING;
}

/**********************************************************************/

/* Sort the array with doubles using insertion sort
*/
void    
sort_dub_array(double A[], int len){
    for(int i = 1; i < len; i++){
        for(int j = i - 1; j >= 0 && A[j + 1] < A[j]; j--){
            dub_swap(&A[j], &A[j + 1]);
        }
    }
}

/**********************************************************************/

/* Sort a parallel array with doubles and characters using insertion sort, with 
the sort condition being the array of doubles */
void    
sort_parallel_array(double A[], char B[], int len){
    for(int i = 1; i < len; i++){
        for(int j = i - 1; j >= 0 && A[j + 1] < A[j]; j--){
            dub_swap(&A[j], &A[j + 1]);
            char_swap(&B[j], &B[j + 1]);
        }
    }
}

/**********************************************************************/

/* Calculate the adjusted scores using the given requirements
*/
double  
calculate_score(double A[], int len){
    double sum=0;
    
    for(int i = 0; i < (len - DISCARDED_SCORES); i++){
        sum += A[i];
    }
    
    return sum / len;
}

/**********************************************************************/

/* Check if enough scores are given in input, if so, return the adjusted score 
for all of them */
double  
get_adjusted_score(double *scores, int scores_len){
    
    /* First, we'll sort the classes with 3 or more scores */
    if(scores_len >= ACCEPTED_LEN){
        sort_dub_array(scores, scores_len);

        /* We'll look past the 2 lowest scores */
        double *adjusted_scores = scores;
        adjusted_scores += DISCARDED_SCORES;

        /* Now we'll calculate and output the adjusted score */
        return calculate_score(adjusted_scores, scores_len);
    }else{
        /* There are not enough scores for an adjusted score */
        return FAILED;
    }
}

/**********************************************************************/

/* Read in one line of input, update the information from the last recorded 
class */
int     
read_line(char *last_lect, double *last_score, int *last_len, 
          double *last_line){
    int count=0;
    char spacer;
    double score, scores[SCORE_LIMIT];
    
    /* First, we'll try read in the lecturer id */
    if(scanf("%c%c", &*last_lect, &spacer) != EXPECTED_INPUT_CHARS){
        return FAILED;
    }

    /* Next, we'll go through all the scores and add them to the array */
    do{
        scanf("%lf%c", &score, &spacer);
        scores[count] = score;
        count += 1;
    }while(spacer != '\n');
    
    /* Now, we'll update the last score, last line and its length */
    *last_len = count;
    for(int i = 0; i < *last_len; i++){
        last_line[i] = scores[i]; 
    }
    
    *last_score = get_adjusted_score(scores, *last_len);
    
    return SUCCESS;
}

/**********************************************************************/

/* Pass in the last class information, add them to a parallel array 
*/
void  
to_parallel_array(char last_lect, double last_score, char *lect_id, 
                  double lect_scores[][LECTURES_LIMIT], int *lect_scored, 
                  int *lect_count, int *adjusted_count){
    
    /* Update the key information for a new line */
    int already_in=0;
    char current_lect = last_lect;
    
    if(last_score != FAILED){
        /* Check if the current lecturer had been recorded before */
        for(int i = 0; i < *lect_count; i++){
            if(lect_id[i] == current_lect){
                already_in = 1;
                lect_scores[current_lect - ALPHA_OFFSET][lect_scored[i]] = 
                    last_score;
                lect_scored[i] += 1;
                *adjusted_count += 1;
                break;
            }
        }

        /* Add in new lecturer data if they have not been recorded before */
        if(!already_in){
            lect_id[*lect_count] = current_lect;
            lect_scores[current_lect-ALPHA_OFFSET][FIRST_INDEX] = last_score;
            lect_scored[*lect_count] += 1;
            *adjusted_count += 1;
            *lect_count += 1; 
        }
    }
}

/**********************************************************************/

/* Add in the adjusted scores to the existing parallel array
*/
void  
adj_to_parallel_array(double *lect_adj_scores, 
                      double lect_scores[][LECTURES_LIMIT], char *lect_id, 
                      int *lect_scored, int *lect_count){
    
    // Get the adjusted scores for each lecturer, and add them to the array
    for(int i = 0; i < *lect_count; i++){
        lect_adj_scores[i] = get_adjusted_score(
            lect_scores[lect_id[i] - ALPHA_OFFSET], lect_scored[i]
        );
    }
}

/**********************************************************************/

/* Pass in the necessary values for stage 1, print them according to 
the requirements */
void 
do_stage1(int class_count, int adjusted_count, char last_lect, 
               double *last_line, int last_len, double last_score){
    
    printf("Stage 1, %d class lines in input\n", class_count);
    printf("Stage 1, %d per-class adjusted scores retained\n", adjusted_count);
    
    printf("Stage 1, last line of input was\n    \"%c:", last_lect);
    for(int i = 0; i < last_len; i++){
            printf(" %.1lf", last_line[i]);
    }
    printf("\"\n");
    
    /* Checks if the last recorded score is a valid value */
    if(last_score != FAILED){
        printf("Stage 1, last per-class score was %.3lf\n", last_score);
    }
    
    printf("\n");
}


/**********************************************************************/

/* Pass in the necessary values for stage 2, print them according to 
the requirements */
void 
do_stage2(char *lect_id, int *lect_scored, double *lect_adj_scores, 
          int lect_count){
    
    for(int i = 0; i < lect_count; i++){
        printf("Stage 2, lecturer %c, %d scored lectures", lect_id[i], 
               lect_scored[i]);
        
        double adjusted_score = round_double(lect_adj_scores[i]);
        if(lect_adj_scores[i] != FAILED){
            printf(", adjusted score %.3lf\n", adjusted_score);
        }else{
            printf("\n");
        }
    }
    printf("\n");
}

/**********************************************************************/

/* Pass in the necessary values for stage 3, sort the scores and print the 
medal winners according to the requirements */
void 
do_stage3(char *lect_id, double *lect_adj_scores, int lect_count){
    
    /* First, we'll sort the parallel arrays by their adjusted scores */
    sort_parallel_array(lect_adj_scores, lect_id, lect_count);
    
    /* We'll check if valid medal winners can be printed out */
    if(lect_adj_scores[lect_count - COUNT_OFFSET] != FAILED){
    
        /* Now, we'll output the medal winners */
        printf("Stage 3, here come the medal winners...\n");
        for(int i = COUNT_OFFSET; i <= lect_count; i++){
            printf("Stage 3, lecturer %c, adjusted score %.3lf\n", 
                lect_id[lect_count - i], 
                round_double(lect_adj_scores[lect_count - i]));
            
            /* We'll print out at most three top scores*/
            if(i == TOP_THREE){
                break;
            }
        }
        
        /* Now, we'll check for scores that ties with any medal winners */
        // Need to check to get rid of negative values
        for(int i = TOP_THREE + COUNT_OFFSET; i <= lect_count; i++){
        
            if(same_decimals(lect_adj_scores[lect_count - i], 
               lect_adj_scores[lect_count - TOP_THREE])){
               
                printf("Stage 3, lecturer %c, adjusted score %.3lf\n", 
                       lect_id[lect_count - i], 
                       round_double(lect_adj_scores[lect_count - i]));
            }else{
                break;
            }
        }
    }else{
        printf("Stage 3, sadly, there were not enough valid lectures\n");
    }
    printf("\n");
}

/**********************************************************************/

/* programming is fun */





