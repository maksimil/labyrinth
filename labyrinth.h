#ifndef __LAB_DEFINE__
#define __LAB_DEFINE__

#include "stdint.h"

static const uint8_t UP = 1 << 0;
static const uint8_t RIGHT = 1 << 1;
static const uint8_t DOWN = 1 << 2;
static const uint8_t LEFT = 1 << 3;

static const uint8_t ENABLE = 1 << 4;
static const uint8_t START = 1 << 5;
static const uint8_t FINISH = 1 << 6;
static const uint8_t VISITED = 1 << 7;

typedef struct {
  uint16_t width;
  uint16_t height;

  // bitmask
  uint8_t **connect;
} Labyrinth;

Labyrinth lab_create(uint16_t width, uint16_t height);
void lab_free(Labyrinth *lab);

void lab_print(const Labyrinth *lab);
void lab_generate(Labyrinth *lab, int16_t s_x, int16_t s_y, int16_t f_x,
                  int16_t f_y);

#endif
