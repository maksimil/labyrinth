#include "labyrinth.h"
#include "malloc.h"
#include "stdlib.h"

Labyrinth lab_create(uint16_t width, uint16_t height) {
  Labyrinth lab;
  lab.width = width;
  lab.height = height;

  lab.connect = malloc(sizeof(*lab.connect) * width);
  for (uint16_t i = 0; i < width; i++) {
    lab.connect[i] = malloc(sizeof(*lab.connect[i]) * height);
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

uint8_t lab_get(const Labyrinth *lab, int16_t x, int16_t y, uint8_t d) {
  if (x >= 0 && x < lab->width && y >= 0 && y < lab->height) {
    return lab->connect[x][y];
  } else {
    return d;
  }
}

void lab_print(const Labyrinth *lab) {
  // first row
  printf("%s", WALL);

  for (uint16_t x = 0; x < lab->width; x++) {
    printf("%s%s", WALL, WALL);
  }
  printf("\n");

  for (uint16_t y = 0; y < lab->height; y++) {
    printf("%s", WALL);
    for (uint16_t x = 0; x < lab->width; x++) {
      if (lab->connect[x][y] & START)
        printf("\x1b[32m");
      else if (lab->connect[x][y] & FINISH)
        printf("\x1b[31m");
      else if ((lab->connect[x][y] & VISITED) == 0)
        printf("\x1b[33m");

      DISPLAY(x, y, ENABLE);

      printf("\x1b[0m");

      if ((x < lab->width - 1) && (lab->connect[x][y] & VISITED) == 0 &&
          (lab->connect[x + 1][y] & VISITED) == 0)
        printf("\x1b[33m");

      DISPLAY(x, y, RIGHT);

      printf("\x1b[0m");
    }

    printf("\n");

    printf("%s", WALL);
    for (uint16_t x = 0; x < lab->width; x++) {
      if ((y < lab->height - 1) && ((lab->connect[x][y] & VISITED) == 0) &&
          ((lab->connect[x][y + 1] & VISITED) == 0))
        printf("\x1b[33m");
      DISPLAY(x, y, DOWN);

      printf("\x1b[0m");

      printf("%s", WALL);
    }

    printf("\n");
  }
}
