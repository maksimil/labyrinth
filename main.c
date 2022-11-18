#include "labyrinth.h"
#include "malloc.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int main() {
  uint16_t width, height;
  printf("Width, height: ");
  scanf("%hu", &width);
  scanf("%hu", &height);

  Labyrinth lab = lab_create(width, height);

  lab_generate(&lab, 0, 0, width - 1, height - 1);

  for (uint16_t i = 0; i < 10; i++) {
    for (uint16_t j = 0; j < 10; j++) {
      lab.connect[i][j] &= 255 ^ VISITED;
    }
  }

  lab_print(&lab);

  lab_free(&lab);
}
