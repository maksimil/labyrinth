#include "labyrinth.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int8_t lab_dig(Labyrinth *lab, int16_t p_x, int16_t p_y) {
  lab->connect[p_x][p_y] |= VISITED;

  if (lab->connect[p_x][p_y] & FINISH) {
    return 1;
  }

  int8_t finished = 0;

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

      finished |= lab_dig(lab, np_x, np_y);
    }
  }

  return finished;
}

int8_t lab_generate(Labyrinth *lab, int16_t s_x, int16_t s_y, int16_t f_x,
                    int16_t f_y) {
  lab->connect[s_x][s_y] = START | ENABLE;
  lab->connect[f_x][f_y] = FINISH | ENABLE;

  return lab_dig(lab, s_x, s_y);
}

const uint8_t NVISITED = VISITED;

int8_t lab_step(Labyrinth *lab, int16_t p_x, int16_t p_y, Path *path) {
  lab->connect[p_x][p_y] &= 255 ^ NVISITED;

  path->data[path->ptr] = p_x;
  path->data[path->ptr + 1] = p_y;
  path->ptr += 2;

  if (lab->connect[p_x][p_y] & FINISH) {
    return 1;
  }

  uint16_t this_ptr = path->ptr;

  int16_t dps[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
  int8_t dirs[4] = {RIGHT, UP, LEFT, DOWN};

  for (uint16_t i = 0; i < 4; i++) {
    int16_t *dp = dps[i];
    int16_t np_x = p_x + dp[0];
    int16_t np_y = p_y + dp[1];

    if (np_x >= 0 && np_y >= 0 && np_x < lab->width && np_y < lab->height &&
        (lab->connect[p_x][p_y] & dirs[i]) &&
        (lab->connect[np_x][np_y] & NVISITED)) {
      path->ptr = this_ptr;
      if (lab_step(lab, np_x, np_y, path))
        return 1;
    }
  }

  return 0;
}

Path lab_solve(Labyrinth *lab, int16_t s_x, int16_t s_y) {
  Path path;
  path.data = malloc(sizeof(*path.data) * 2 * lab->width * lab->height);
  path.ptr = 0;

  lab_step(lab, s_x, s_y, &path);

  return path;
}
