#include "labyrinth.h"
#include "malloc.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "unistd.h"

int main() {
  srand(time(NULL));

  uint16_t width, height;
  double wall_proportion;
  printf("Width, height: ");
  scanf("%hu", &width);
  scanf("%hu", &height);

  printf("Wall proportion (.5-.6 recommended): ");
  scanf("%lf", &wall_proportion);

  Labyrinth lab = lab_create(width, height);

  // generating labyrinth
  {
    int8_t gen_res = 0;
    size_t failures = 0;

    uint16_t disabled =
        (int)(width * height -
              ((2 * width - 1) * (2 * height - 1) * (1 - wall_proportion) + 1) /
                  2.);

    printf("Disabled cells: %hu\n", disabled);

    while (!gen_res) {
      // clear from previous attempt
      for (uint16_t i = 0; i < width; i++) {
        lab.connect[i] = malloc(sizeof(*lab.connect[i]) * height);
        for (uint16_t j = 0; j < height; j++) {
          lab.connect[i][j] = ENABLE;
        }
      }

      lab.connect[0][0] |= START;
      lab.connect[lab.width - 1][lab.height - 1] |= FINISH;

      // disable cells
      uint16_t dcount = 0;
      while (dcount < disabled) {
        uint16_t x = random() % lab.width;
        uint16_t y = random() % lab.height;

        if ((lab.connect[x][y] & ENABLE) &&
            ((lab.connect[x][y] & (START | FINISH)) == 0)) {
          lab.connect[x][y] = 0;
          dcount += 1;
        }
      }

      // generate maze
      gen_res = lab_generate(&lab, 0, 0, width - 1, height - 1);

      if (!gen_res) {
        failures += 1;
        if (failures % 100 == 0) {
          printf("Failed to generate maze %lu\n", failures);
        }
      }
    }

    printf("Failed to generate maze %lu times\n", failures);
  }

  // labyrinth solve and clearing
  Path path = lab_solve(&lab, 0, 0);

  for (uint16_t x = 0; x < lab.width; x++) {
    for (uint16_t y = 0; y < lab.height; y++) {
      lab.connect[x][y] |= VISITED;
    }
  }

  // animation loop
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
