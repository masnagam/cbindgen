#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct dep_struct;

typedef struct dep_struct {
  uint32_t x;
  double y;
} dep_struct;

uint32_t get_x(const struct dep_struct *dep_struct);
