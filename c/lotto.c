#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define FALSE 0
#define TRUE !FALSE
#define SIZEOF(a) (sizeof a / sizeof a[0])

/*
 * Estimated lotto prizes:
 *   https://www.national-lottery.co.uk/player/p/help/aboutlotto/prizecalculation.ftl
 */
#define MATCH_6_PRIZE 5000000
#define MATCH_5_PLUS_BONUS_PRIZE 50000
#define MATCH_5_PRIZE 1000
#define MATCH_4_PRIZE 100
#define MATCH_3_PRIZE 25
// match types
#define MATCH_5_PLUS_BONUS 7
#define MATCH_6 6
#define MATCH_5 5
#define MATCH_4 4
#define MATCH_3 3
// define lowest and highest ball numbers
#define LOW_BALL 1
#define HIGH_BALL 49
// array sizes
#define MAIN_BALLS_SIZE 6
#define ENTRY_BALLS_SIZE 6
#define MATCHES_COUNT_SIZE 8
#define ENTRIES_SIZE 5

void shuffle(int a[], int size);
int bitSet(long bitMask, int i);
long setBit(long bitmask, int i);
long bitmask(int a[], int size);
int matches(int entry[ENTRY_BALLS_SIZE], long mainBallsBitmask, int bonusBall);
long cost(int matchesCount[MATCHES_COUNT_SIZE]);
void print(
        time_t start, time_t end,
        long minCost, int minCostMainBalls[MAIN_BALLS_SIZE], int minCostBonusBall,
        long maxCost, int maxCostMainBalls[MAIN_BALLS_SIZE], int maxCostBonusBall);

int main(int argc, char** argv) {

  // lotto entries, could be read from a database or file?
  int entries[ENTRIES_SIZE][ENTRY_BALLS_SIZE];
  
  // draw balls for random entries
  int balls[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49
  };

  // create random entries for testing
  int i;
  for (i = 0; i < ENTRIES_SIZE; i++) {
    shuffle(balls, SIZEOF(balls));
    entries[i][0] = balls[0];
    entries[i][1] = balls[1];
    entries[i][2] = balls[2];
    entries[i][3] = balls[3];
    entries[i][4] = balls[4];
    entries[i][5] = balls[5];
  }

  time_t start = time(NULL);

  // minimum values
  long minCost = LONG_MAX;
  int minCostMainBalls[MAIN_BALLS_SIZE];
  int minCostBonusBall = -1;
  
  // maximum values
  long maxCost = LONG_MIN;
  int maxCostMainBalls[MAIN_BALLS_SIZE];
  int maxCostBonusBall = -1;

  // iterate over main ball combinations
  int mainBalls[MAIN_BALLS_SIZE];
  for (mainBalls[0] = LOW_BALL; mainBalls[0] <= HIGH_BALL - 6; mainBalls[0]++) {
    for (mainBalls[1] = mainBalls[0] + 1; mainBalls[1] <= HIGH_BALL - 5; mainBalls[1]++) {
      for (mainBalls[2] = mainBalls[1] + 1; mainBalls[2] <= HIGH_BALL - 4; mainBalls[2]++) {
        for (mainBalls[3] = mainBalls[2] + 1; mainBalls[3] <= HIGH_BALL - 3; mainBalls[3]++) {
          for (mainBalls[4] = mainBalls[3] + 1; mainBalls[4] <= HIGH_BALL - 2; mainBalls[4]++) {
            for (mainBalls[5] = mainBalls[4] + 1; mainBalls[5] <= HIGH_BALL - 1; mainBalls[5]++) {

              // create bitmask for match lookup
              long mainBallsBitmask = bitmask(mainBalls, SIZEOF(mainBalls));

              // iterate over bonus balls
              int bonusBall;
              for (bonusBall = mainBalls[5] + 1; bonusBall <= HIGH_BALL; bonusBall++) {

                // init match counts to zero
                int matchesCount[MATCHES_COUNT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};

                // iterate over entries
                int i;
                for (i = 0; i < ENTRIES_SIZE; i++) {
                  // count matches
                  int m = matches(entries[i], mainBallsBitmask, bonusBall);
                  // increment matches for number of balls matched
                  matchesCount[m]++;
                }

                // get total cost of draw
                long c = cost(matchesCount);

                // keep track of highest/lowest draw costs
                if (c < minCost) {
                  minCost = c;
                  memcpy(minCostMainBalls, mainBalls, MAIN_BALLS_SIZE * sizeof (int));
                  minCostBonusBall = bonusBall;
                } else if (c > maxCost) {
                  maxCost = c;
                  memcpy(maxCostMainBalls, mainBalls, MAIN_BALLS_SIZE * sizeof (int));
                  maxCostBonusBall = bonusBall;
                }
              }
            }
          }
        }
      }
    }
  }

  time_t end = time(NULL);

  print(start, end, minCost, minCostMainBalls, minCostBonusBall,
          maxCost, maxCostMainBalls, maxCostBonusBall);

  return (EXIT_SUCCESS);
}

void shuffle(int a[], int size) {
  srand(time(NULL));
  int i;
  for (i = 0; i < size; i++) {
    int index = rand() % (i + 1);
    int x = a[index];
    a[index] = a[i];
    a[i] = x;
  }
}

int bitSet(long bitMask, int i) {
  return (bitMask & (1L << i)) > 0;
}

long setBit(long bitmask, int i) {
  return bitmask | (1L << i);
}

long bitmask(int a[], int size) {

  long bitmask = 0;

  int i;
  for (i = 0; i < size; i++) {
    bitmask = setBit(bitmask, a[i]);
  }

  return bitmask;
}

int matches(int entry[ENTRY_BALLS_SIZE], long mainBallsBitmask, int bonusBall) {

  int matches = 0;
  int bonusMatch = FALSE;

  int i;
  for (i = 0; i < ENTRY_BALLS_SIZE; i++) {
    if (bitSet(mainBallsBitmask, entry[i])) {
      matches++;
    } else if (entry[i] == bonusBall) {
      bonusMatch = TRUE;
    }
  }

  if (matches == MATCH_5 && bonusMatch) {
    matches = MATCH_5_PLUS_BONUS;
  }

  return matches;
}

long cost(int matchesCount[MATCHES_COUNT_SIZE]) {

  long cost = 0;

  // add jackpot prize
  if (matchesCount[MATCH_6] > 0) {
    cost = MATCH_6_PRIZE;
  }

  // add lesser prizes, multiplied by number of winners
  cost += matchesCount[MATCH_5_PLUS_BONUS] * MATCH_5_PLUS_BONUS_PRIZE
          + matchesCount[MATCH_5] * MATCH_5_PRIZE
          + matchesCount[MATCH_4] * MATCH_4_PRIZE
          + matchesCount[MATCH_3] * MATCH_3_PRIZE;

  return cost;
}

void print(
        time_t start, time_t end,
        long minCost, int minCostMainBalls[MAIN_BALLS_SIZE], int minCostBonusBall,
        long maxCost, int maxCostMainBalls[MAIN_BALLS_SIZE], int maxCostBonusBall) {

  printf("\n--- time ---\n");
  printf("elapsed time = %lu seconds\n", (end - start));

  printf("\n--- min ---\n");
  printf("minimum cost = %lu\n", minCost);
  printf("minimum main balls = %d,%d,%d,%d,%d,%d\n",
          minCostMainBalls[0],
          minCostMainBalls[1],
          minCostMainBalls[2],
          minCostMainBalls[3],
          minCostMainBalls[4],
          minCostMainBalls[5]);
  printf("minimum bonus ball = %d\n", minCostBonusBall);

  printf("\n--- max ---\n");
  printf("maximum cost = %lu\n", maxCost);
  printf("maximum main balls = %d,%d,%d,%d,%d,%d\n",
          maxCostMainBalls[0],
          maxCostMainBalls[1],
          maxCostMainBalls[2],
          maxCostMainBalls[3],
          maxCostMainBalls[4],
          maxCostMainBalls[5]);
  printf("maximum bonus ball = %d\n", maxCostBonusBall);
}
