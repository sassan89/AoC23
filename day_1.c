#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_single_calibration_value(char* line, size_t len) {
  uint16_t first_digit = 0;
  uint16_t last_digit = 0;

  bool first = true;

  for (size_t idx = 0; idx < len; ++idx) {
    if (isdigit(line[idx])) {
      if (first) {
        first_digit = atoi(&line[idx]);
        first = false;
      } else {
        last_digit = atoi(&line[idx]);
      }
    }
  }

  return first_digit * 10 + last_digit;
}

int main(int argc, char** argv) {
  char* line = "1ha2llo3";

  size_t value = get_single_calibration_value(line, strlen(line));

  printf("Value %ld\n", value);
}
