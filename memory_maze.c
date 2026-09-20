#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_ROUNDS 3
#define SECONDS_PER_OBJECT 6   


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


void print_round_banner(int round, int count) {
    printf("\n-----------------------------------\n");
    printf("   ROUND %d  (%d objects)\n", round, count);
    printf("-----------------------------------\n\n");
}

int main() {
    
    char objects[8][20] = {"Pen", "Book", "Clock", "Key",
                            "Cup", "Chair", "Phone", "Bag"};
    int round;
    int i, j;
    int count;
    int order[8];
    int answer[8];
    int temp;
    int correct;
    int mistakes;
    double accuracy;
    double time_taken;
    double max_time;
    double accuracy_score;
    double time_score;
    double score;
    time_t now;
    time_t start_time;
    time_t end_time;
    struct tm *t;
    FILE *file;

    
    int history_count[TOTAL_ROUNDS];
    int history_correct[TOTAL_ROUNDS];
    int history_mistakes[TOTAL_ROUNDS];
    double history_accuracy[TOTAL_ROUNDS];
    double history_time[TOTAL_ROUNDS];
    double history_score[TOTAL_ROUNDS];
    

    srand(time(NULL));

    
    printf("===================================\n");
    printf("     MEMORY MAZE - VANISHING ROOM\n");
    printf("===================================\n\n");
    printf("HOW TO PLAY:\n");
    printf("1. A list of everyday objects will appear on screen.\n");
    printf("2. Read them and remember the ORDER they are shown in.\n");
    printf("3. Press ENTER when you are ready - the objects will vanish.\n");
    printf("4. A numbered reference list will appear (each object has\n");
    printf("   a fixed number). Type the NUMBERS in the same order you\n");
    printf("   saw the objects.\n");
    printf("5. Your accuracy, mistakes, and response time are recorded.\n");
    printf("6. You also get a SCORE out of 100: 70 marks come from your\n");
    printf("   accuracy, and 30 marks come from your speed. This way,\n");
    printf("   answering fast but wrong won't score well, and being slow\n");
    printf("   even if correct won't get full marks either.\n\n");
    printf("There are %d rounds, and each round is HARDER than the last:\n", TOTAL_ROUNDS);
    printf("   Round 1 -> 3 objects\n");
    printf("   Round 2 -> 4 objects\n");
    printf("   Round 3 -> 5 objects\n\n");
    printf("At the end, you will see a summary of all 3 rounds together,\n");
    printf("and your full history will be saved in memory_maze_results.txt\n\n");
    wait_for_enter();
    

    for (round = 1; round <= TOTAL_ROUNDS; round++) {

        count = round + 2;          
        if (count > 8) count = 8;   

        for (i = 0; i < count; i++) {
            order[i] = i;
        }

        
        for (i = count - 1; i > 0; i--) {
            j = rand() % (i + 1);
            temp = order[i];
            order[i] = order[j];
            order[j] = temp;
        }

       
        clear_screen();
        print_round_banner(round, count);

        printf("Remember these objects IN ORDER:\n\n");
        for (i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, objects[order[i]]);
        }

        printf("\nTake your time reading the list above.\n");
        wait_for_enter();   

        
        clear_screen();

        printf("Objects hidden! Here is the reference list (each object\n");
        printf("has a fixed number, NOT the order they were shown in):\n");
        for (i = 0; i < count; i++) {
            printf("%d = %s\n", i, objects[i]);
        }

        printf("\nType the NUMBER of each object in the order you saw them.\n");

        start_time = time(NULL);

        for (i = 0; i < count; i++) {
            printf("Position %d: ", i + 1);
            scanf("%d", &answer[i]);
            flush_line();   
        }

        end_time = time(NULL);
        time_taken = difftime(end_time, start_time);

       
        correct = 0;
        mistakes = 0;

        printf("\nCorrect order was : ");
        for (i = 0; i < count; i++) printf("%s ", objects[order[i]]);

        printf("\nYou answered      : ");
        for (i = 0; i < count; i++) {
            if (answer[i] >= 0 && answer[i] < count)
                printf("%s ", objects[answer[i]]);
            else
                printf("? ");

            if (answer[i] == order[i]) correct++;
            else mistakes++;
        }
        printf("\n");

        accuracy = (correct / (double)count) * 100;

        
        max_time = count * SECONDS_PER_OBJECT;   

        accuracy_score = (accuracy / 100.0) * 70.0;

        time_score = ((max_time - time_taken) / max_time) * 30.0;
        if (time_score < 0) time_score = 0;     
        if (time_score > 30) time_score = 30;   

        score = accuracy_score + time_score;
       

        printf("\nRound %d Result -> Correct: %d/%d | Mistakes: %d | Accuracy: %.1f%% | Time: %.2f seconds\n",
               round, correct, count, mistakes, accuracy, time_taken);
        printf("Score (Accuracy 70%% + Speed 30%%): %.2f out of 100\n", score);

       
        history_count[round - 1]    = count;
        history_correct[round - 1]  = correct;
        history_mistakes[round - 1] = mistakes;
        history_accuracy[round - 1] = accuracy;
        history_time[round - 1]     = time_taken;
        history_score[round - 1]    = score;

       
        file = fopen("memory_maze_results.txt", "a");
        if (file != NULL) {
            now = time(NULL);
            t = localtime(&now);
            fprintf(file,
                "%04d-%02d-%02d %02d:%02d | Round=%d Objects=%d Correct=%d Mistakes=%d Accuracy=%.1f%% Time=%.2fs Score=%.2f\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min,
                round, count, correct, mistakes, accuracy, time_taken, score);
            fclose(file);
        }

        if (round < TOTAL_ROUNDS) {
            printf("\nReady for the next round? It will be a little harder.\n");
            wait_for_enter();
        }
    }

    
    printf("\n\n===================================\n");
    printf("      FINAL SUMMARY - ALL ROUNDS\n");
    printf("===================================\n\n");
    for (round = 1; round <= TOTAL_ROUNDS; round++) {
        printf("Round %d (%d objects): Correct %d/%d | Mistakes %d | Accuracy %.1f%% | Time %.2f sec | Score %.2f/100\n",
               round,
               history_count[round - 1],
               history_correct[round - 1], history_count[round - 1],
               history_mistakes[round - 1],
               history_accuracy[round - 1],
               history_time[round - 1],
               history_score[round - 1]);
    }

    printf("\nYour whole history across every session you have ever played\n");
    printf("is saved in memory_maze_results.txt\n");
    printf("Play again another day to see how your memory performance changes over time.\n");

    return 0;
}
