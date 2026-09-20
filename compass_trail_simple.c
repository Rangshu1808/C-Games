#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAIL_LENGTH 8
#define TOTAL_ROUNDS 2
#define SECONDS_PER_STEP 5   


void flush_line(void) {
    int c;
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}


void wait_for_enter(void) {
    printf("(Press ENTER to continue) ");
    flush_line();
}

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

char opposite_direction(char d) {
    if (d == 'N') return 'S';
    if (d == 'S') return 'N';
    if (d == 'E') return 'W';
    if (d == 'W') return 'E';
    return '?';
}


void print_full_name(char d) {
    if (d == 'N') printf("NORTH");
    else if (d == 'S') printf("SOUTH");
    else if (d == 'E') printf("EAST");
    else if (d == 'W') printf("WEST");
}

int main() {
    
    char directions[4] = {'N', 'S', 'E', 'W'};
    char trail[MAX_TRAIL_LENGTH];
    char correct_return[MAX_TRAIL_LENGTH];
    char player_answer[MAX_TRAIL_LENGTH];
    char input[10];
    int round;
    int trail_length;
    int i;
    int random_index;
    int mistakes;
    int correct;
    double accuracy;
    double time_taken;
    double max_time;
    double accuracy_score;
    double time_score;
    double score;
    time_t start_time;
    time_t end_time;
    time_t now;
    struct tm *t;
    FILE *file;
    

    srand(time(NULL));

    printf("===================================\n");
    printf("      COMPASS TRAIL - FOLLOW ME\n");
    printf("===================================\n\n");
    printf("Remember the directions shown.\n");
    printf("Memorise it and walk it back in REVERSE later.\n");
    printf("Each step waits for YOU to press ENTER, so take\n");
    printf("as much time as you need.\n\n");
    printf("There are %d rounds, and the path gets LONGER each\n", TOTAL_ROUNDS);
    printf("round: Round 1 has 4 directions, Round 2 has 6.\n\n");
    printf("You also get a SCORE out of 100: 70 marks come from your\n");
    printf("accuracy, and 30 marks come from your speed. This way,\n");
    printf("answering fast but wrong won't score well, and being slow\n");
    printf("even if correct won't get full marks either.\n\n");
    wait_for_enter();

    for (round = 1; round <= TOTAL_ROUNDS; round++) {

        trail_length = 4 + (round - 1) * 2;   
        if (trail_length > MAX_TRAIL_LENGTH) trail_length = MAX_TRAIL_LENGTH;

        
        for (i = 0; i < trail_length; i++) {
            random_index = rand() % 4;
            trail[i] = directions[random_index];
        }

        clear_screen();
        printf("===================================\n");
        printf("   ROUND %d  (%d directions)\n", round, trail_length);
        printf("===================================\n\n");

        
        for (i = 0; i < trail_length; i++) {
            printf("\nStep %d: Go ", i + 1);
            print_full_name(trail[i]);
            printf(" ...\n");
            wait_for_enter();
        }

        printf("\nYou have reached the destination! Get ready...\n");
        wait_for_enter();

        
        clear_screen();

        printf("===================================\n");
        printf("   ROUND %d - RETURN TRIP\n", round);
        printf("===================================\n\n");
        printf("The path is gone. Now try to get back to original point.\n");
        printf("Remember: reverse the order AND flip each direction\n");
        printf("(N becomes S, S becomes N, E becomes W, W becomes E, while returning)\n\n");
        wait_for_enter();

        
        for (i = 0; i < trail_length; i++) {
            correct_return[i] = opposite_direction(trail[trail_length - 1 - i]);
        }

        
        printf("\n");
        start_time = time(NULL);

        for (i = 0; i < trail_length; i++) {
            printf("Return step %d (type N, S, E or W): ", i + 1);
            scanf("%9s", input);
            flush_line();   
            player_answer[i] = input[0];
            
            if (player_answer[i] >= 'a' && player_answer[i] <= 'z') {
                player_answer[i] = player_answer[i] - 32;
            }
        }

        end_time = time(NULL);
        time_taken = difftime(end_time, start_time);

       
        mistakes = 0;
        printf("\n----- RESULT -----\n");
        printf("Correct return path : ");
        for (i = 0; i < trail_length; i++) {
            printf("%c ", correct_return[i]);
        }
        printf("\nYour return path    : ");
        for (i = 0; i < trail_length; i++) {
            printf("%c ", player_answer[i]);
            if (player_answer[i] != correct_return[i]) {
                mistakes++;
            }
        }
        printf("\n");

        correct = trail_length - mistakes;
        accuracy = (correct / (double)trail_length) * 100;

        
        max_time = trail_length * SECONDS_PER_STEP;   

        accuracy_score = (accuracy / 100.0) * 70.0;

        time_score = ((max_time - time_taken) / max_time) * 30.0;
        if (time_score < 0) time_score = 0;     
        if (time_score > 30) time_score = 30;   

        score = accuracy_score + time_score;
        

        printf("\nRound %d Result -> Correct: %d/%d | Mistakes: %d | Accuracy: %.1f%% | Time: %.2f seconds\n",
               round, correct, trail_length, mistakes, accuracy, time_taken);
        printf("Score (Accuracy 70%% + Speed 30%%): %.2f out of 100\n", score);

        
        file = fopen("compass_trail_results.txt", "a");
        if (file != NULL) {
            now = time(NULL);
            t = localtime(&now);
            fprintf(file,
                "%04d-%02d-%02d %02d:%02d | Round=%d TrailLength=%d Correct=%d Mistakes=%d Accuracy=%.1f%% Time=%.2fs Score=%.2f\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min,
                round, trail_length, correct, mistakes, accuracy, time_taken, score);
            fclose(file);
        }

        if (round < TOTAL_ROUNDS) {
            printf("\nReady for the next round? The path will be longer this time.\n");
            wait_for_enter();
        }
    }

    printf("\n\nAll rounds complete! Your results are saved in compass_trail_results.txt\n");
    printf("Play again another day to see how your orientation recall changes over time.\n");

    return 0;
}
