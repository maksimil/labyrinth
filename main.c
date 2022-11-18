#include "labyrinth.h"
#include "malloc.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "unistd.h"

int main() {
  uint16_t width, height;
  printf("Width, height: ");
  scanf("%hu", &width);
  scanf("%hu", &height);

  Labyrinth lab = lab_create(width, height);

  lab_generate(&lab, 0, 0, width - 1, height - 1);

  lab_print(&lab);
  printf("\x1b[%dA", lab.height * 2 + 1);

  Path path = lab_solve(&lab, 0, 0);

  for (uint16_t x = 0; x < lab.width; x++) {
    for (uint16_t y = 0; y < lab.height; y++) {
      lab.connect[x][y] |= VISITED;
    }
  }

  for (uint16_t i = 0; i < path.ptr; i += 2) {
    lab_print(&lab);
    printf("\x1b[%dA", lab.height * 2 + 1);

    lab.connect[path.data[i]][path.data[i + 1]] &= 255 ^ VISITED;

    usleep(50 * 1000);
  }

  lab_print(&lab);

  lab_free(&lab);
  free(path.data);
}
