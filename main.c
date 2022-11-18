#include "malloc.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

const uint8_t UP = 1 << 0;
const uint8_t RIGHT = 1 << 1;
const uint8_t DOWN = 1 << 2;
const uint8_t LEFT = 1 << 3;

const uint8_t ENABLE = 1 << 4;
const uint8_t START = 1 << 5;
const uint8_t FINISH = 1 << 6;
const uint8_t VISITED = 1 << 7;

typedef struct {
  uint16_t width;
  uint16_t height;

  // bitmask
  uint8_t **connect;
} Labyrinth;

Labyrinth lab_create(size_t width, size_t height) {
  Labyrinth lab;
  lab.width = width;
  lab.height = height;

  lab.connect = malloc(sizeof(*lab.connect) * width);
  for (uint16_t i = 0; i < width; i++) {
    lab.connect[i] = malloc(sizeof(*lab.connect[i] * height));
    for (uint16_t j = 0; j < height; j++) {
      lab.connect[i][j] = ENABLE;
    }
  }

  return lab;
}

void lab_free(Labyrinth *lab) {
  for (uint16_t i = 0; i < lab->width; i++) {
    free(lab->connect[i]);
  }

  free(lab->connect);
};

const char *PATH = "█";
const char *WALL = " ";

#define DISPLAY(x, y, dir)                                                     \
  if (lab->connect[x][y] & dir)                                                \
    printf("%s", PATH);                                                        \
  else                                                                         \
    printf("%s", WALL);

void lab_print(const Labyrinth *lab) {
  // first row
  printf("%s", WALL);

  for (uint16_t x = 0; x < lab->width; x++) {
    DISPLAY(x, 0, UP);
    printf("%s", WALL);
  }
  printf("\n");

  for (uint16_t y = 0; y < lab->height; y++) {
    DISPLAY(0, y, LEFT);
    for (uint16_t x = 0; x < lab->width; x++) {
      if (lab->connect[x][y] & START)
        printf("\x1b[32m");
      else if (lab->connect[x][y] & FINISH)
        printf("\x1b[31m");

      DISPLAY(x, y, ENABLE);

      printf("\x1b[0m");

      DISPLAY(x, y, RIGHT);
    }

    printf("\n");

    printf("%s", WALL);
    for (uint16_t x = 0; x < lab->width; x++) {
      DISPLAY(x, y, DOWN);
      printf("%s", WALL);
    }

    printf("\n");
  }
}

void lab_dig(Labyrinth *lab, int16_t p_x, int16_t p_y);

void lab_generate(Labyrinth *lab, int16_t s_x, int16_t s_y, int16_t f_x,
                  int16_t f_y) {
  srand(time(NULL));
  lab->connect[s_x][s_y] = START | ENABLE;
  lab->connect[f_x][f_y] = FINISH | ENABLE;

  lab_dig(lab, s_x, s_y);
}

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

int main() {
  uint16_t width, height;
  printf("Width, height: ");
  scanf("%hu", &width);
  scanf("%hu", &height);

  Labyrinth lab = lab_create(width, height);

  lab_generate(&lab, 0, 0, width - 1, height - 1);

  lab_print(&lab);

  lab_free(&lab);
}
