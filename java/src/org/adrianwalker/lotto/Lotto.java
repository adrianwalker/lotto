package org.adrianwalker.lotto;

import java.util.Random;

public final class Lotto {

  /*
   * Estimated lotto prizes:
   *   https://www.national-lottery.co.uk/player/p/help/aboutlotto/prizecalculation.ftl
   */
  private static final int MATCH_6_PRIZE = 5000000;
  private static final int MATCH_5_PLUS_BONUS_PRIZE = 50000;
  private static final int MATCH_5_PRIZE = 1000;
  private static final int MATCH_4_PRIZE = 100;
  private static final int MATCH_3_PRIZE = 25;
  // match types
  private static final int MATCH_5_PLUS_BONUS = 7;
  private static final int MATCH_6 = 6;
  private static final int MATCH_5 = 5;
  private static final int MATCH_4 = 4;
  private static final int MATCH_3 = 3;
  // define lowest and highest ball numbers
  private static final int LOW_BALL = 1;
  private static final int HIGH_BALL = 49;
  // array sizes
  private static final int MAIN_BALLS_SIZE = 6;
  private static final int ENTRY_BALLS_SIZE = 6;
  private static final int ENTRIES_SIZE = 5;

  public static void main(final String[] args) {

    // lotto entries, could be read from a database or file?
    int[][] entries = new int[ENTRIES_SIZE][ENTRY_BALLS_SIZE];
    
    // draw balls for random entries
    int[] balls = {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
      11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
      21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
      31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
      41, 42, 43, 44, 45, 46, 47, 48, 49
    };

    // create random entries for testing
    for (int i = 0; i < ENTRIES_SIZE; i++) {
      shuffle(balls);
      entries[i][0] = balls[0];
      entries[i][1] = balls[1];
      entries[i][2] = balls[2];
      entries[i][3] = balls[3];
      entries[i][4] = balls[4];
      entries[i][5] = balls[5];
    }

    long start = System.currentTimeMillis();

    // minimum values
    long minCost = Long.MAX_VALUE;
    int[] minCostMainBalls = new int[MAIN_BALLS_SIZE];
    int minCostBonusBall = -1;

    // maximum values
    long maxCost = Long.MIN_VALUE;
    int[] maxCostMainBalls = new int[MAIN_BALLS_SIZE];
    int maxCostBonusBall = -1;

    // iterate over main ball combinations
    int[] mainBalls = new int[MAIN_BALLS_SIZE];
    for (mainBalls[0] = LOW_BALL; mainBalls[0] <= HIGH_BALL - 6; mainBalls[0]++) {
      for (mainBalls[1] = mainBalls[0] + 1; mainBalls[1] <= HIGH_BALL - 5; mainBalls[1]++) {
        for (mainBalls[2] = mainBalls[1] + 1; mainBalls[2] <= HIGH_BALL - 4; mainBalls[2]++) {
          for (mainBalls[3] = mainBalls[2] + 1; mainBalls[3] <= HIGH_BALL - 3; mainBalls[3]++) {
            for (mainBalls[4] = mainBalls[3] + 1; mainBalls[4] <= HIGH_BALL - 2; mainBalls[4]++) {
              for (mainBalls[5] = mainBalls[4] + 1; mainBalls[5] <= HIGH_BALL - 1; mainBalls[5]++) {

                // create bitmask for match lookup
                long mainBallsBitmask = bitmask(mainBalls);

                // iterate over bonus balls
                for (int bonusBall = mainBalls[5] + 1; bonusBall <= HIGH_BALL; bonusBall++) {

                  // init match counts to zero
                  int[] matchesCount = {0, 0, 0, 0, 0, 0, 0, 0};

                  // iterate over entries
                  for (int[] entry : entries) {
                    // count matches
                    int matches = matches(entry, mainBallsBitmask, bonusBall);
                    // increment matches for number of balls matched
                    matchesCount[matches]++;
                  }

                  // get total cost of draw
                  long cost = cost(matchesCount);

                  // keep track of highest/lowest draw costs
                  if (cost < minCost) {
                    minCost = cost;
                    minCostMainBalls = mainBalls.clone();
                    minCostBonusBall = bonusBall;
                  } else if (cost > maxCost) {
                    maxCost = cost;
                    maxCostMainBalls = mainBalls.clone();
                    maxCostBonusBall = bonusBall;
                  }
                }
              }
            }
          }
        }
      }
    }

    long end = System.currentTimeMillis();

    print(start, end,
            minCost, minCostMainBalls, minCostBonusBall,
            maxCost, maxCostMainBalls, maxCostBonusBall);
  }

  private static void shuffle(final int[] a) {

    Random rand = new Random(System.currentTimeMillis());

    for (int i = 0; i < a.length; i++) {

      int index = rand.nextInt(i + 1);
      int x = a[index];
      a[index] = a[i];
      a[i] = x;
    }
  }

  private static boolean bitSet(final long bitMask, final int i) {
    return (bitMask & (1L << i)) > 0;
  }

  private static long setBit(final long bitmask, final int i) {
    return bitmask | (1L << i);
  }

  private static long bitmask(final int[] a) {

    long bitmask = 0;

    for (int i : a) {
      bitmask = setBit(bitmask, i);
    }

    return bitmask;
  }

  private static int matches(final int[] entry, final long mainBallsBitmask, final int bonusBall) {

    int matches = 0;
    boolean bonusMatch = false;

    for (int number : entry) {

      if (bitSet(mainBallsBitmask, number)) {
        matches++;
      } else if (number == bonusBall) {
        bonusMatch = true;
      }
    }

    if (matches == MATCH_5 && bonusMatch) {
      matches = MATCH_5_PLUS_BONUS;
    }

    return matches;
  }

  private static long cost(final int[] matchesCount) {

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

  private static void print(
          final long start, final long end,
          final long minCost, final int[] minCostMainBalls, final int minCostBonusBall,
          final long maxCost, final int[] maxCostMainBalls, final int maxCostBonusBall) {

    System.out.printf("\n--- time ---\n");
    System.out.printf("elapsed time = %s seconds\n", (end - start) / 1000);

    System.out.printf("\n--- min ---\n");
    System.out.printf("minimum cost = %s\n", minCost);
    System.out.printf("minimum main balls = %s,%s,%s,%s,%s,%s\n",
            minCostMainBalls[0],
            minCostMainBalls[1],
            minCostMainBalls[2],
            minCostMainBalls[3],
            minCostMainBalls[4],
            minCostMainBalls[5]);
    System.out.printf("minimum bonus ball = %s\n", minCostBonusBall);

    System.out.printf("\n--- max ---\n");
    System.out.printf("maximum cost = %s\n", maxCost);
    System.out.printf("maximum main balls = %s,%s,%s,%s,%s,%s\n",
            maxCostMainBalls[0],
            maxCostMainBalls[1],
            maxCostMainBalls[2],
            maxCostMainBalls[3],
            maxCostMainBalls[4],
            maxCostMainBalls[5]);
    System.out.printf("maximum bonus ball = %s\n", maxCostBonusBall);
  }
}
