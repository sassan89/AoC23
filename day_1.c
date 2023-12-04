// Copyright 2023 Arkadius Motyl 
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version. This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details. You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_single_calibration_value(char* line, size_t len) {
  uint16_t first_digit = 0;
  uint16_t last_digit = 0;

  bool first_digit_set = false;

  for (size_t idx = 0; idx < len; ++idx) {
    if (isdigit(line[idx])) {
      // atoi() converts until it finds a null terminator, but since we're
      // passing a whole string, we have to construct a temporary string with
      // just the char and the null terminator
      char tmp[2] = {line[idx], '\0'};
      uint16_t digit = atoi(tmp);

      // If only the first digit is set, the last digit equals the first
      if (!first_digit_set) {
        first_digit = digit;
        last_digit = digit;
        first_digit_set = true;
      } else {
        last_digit = digit;
      }
    }
  }

  return first_digit * 10 + last_digit;
}

int main() {
  // Each line will be read into this buffer
  size_t buffer_size = 512;
  char buffer[buffer_size];

  FILE* fp = NULL;

  // Open the file containing the calibration values
  fp = fopen("calibration.txt", "r");

  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }

  size_t calibration_value = 0;

  // Extract from each line the calibration value
  while (fgets(buffer, buffer_size, fp)) {
    calibration_value += get_single_calibration_value(buffer, strlen(buffer));
  }

  printf("Calibration value = %ld\n", calibration_value);

  fclose(fp);

  exit(EXIT_SUCCESS);
}
