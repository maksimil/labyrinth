#include "labyrinth.h"
#include "stdlib.h"
#include "time.h"

void lab_dig(Labyrinth *lab, int16_t p_x, int16_t p_y) {
  lab->connect[p_x][p_y] |= VISITED;

  if (lab->connect[p_x][p_y] & FINISH) {
    return;
  }

  int16_t dps[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
  int8_t dirs[4] = {RIGHT, UP, LEFT, DOWN};
  int8_t visited = 0;

  for (uint16_t _ = 0; _ < 4; _++) {
    uint8_t idx = random() % 4;

    while ((visited & (1 << idx)) != 0) {
      idx = random() % 4;
    }

    visited |= (1 << idx);

    int16_t *dp = dps[idx];
    int16_t np_x = p_x + dp[0];
    int16_t np_y = p_y + dp[1];

    if (np_x >= 0 && np_y >= 0 && np_x < lab->width && np_y < lab->height &&
        ((lab->connect[np_x][np_y] & VISITED) == 0) &&
        (lab->connect[np_x][np_y] & ENABLE)) {
      lab->connect[p_x][p_y] |= dirs[idx];
      lab->connect[np_x][np_y] |= dirs[(idx + 2) % 4];

      lab_dig(lab, np_x, np_y);
    }
  }
}

void lab_generate(Labyrinth *lab, int16_t s_x, int16_t s_y, int16_t f_x,
                  int16_t f_y) {
  srand(time(NULL));
  lab->connect[s_x][s_y] = START | ENABLE;
  lab->connect[f_x][f_y] = FINISH | ENABLE;

  lab_dig(lab, s_x, s_y);
}
