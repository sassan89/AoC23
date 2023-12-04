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

size_t get_single_calibration_value_one_star(char* line, size_t len) {
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

uint16_t* get_digit_at_start(char* str) {
  // The digits must be in ascending order, so we can easily convert them
  const char* DIGITS_STR[] = {"zero", "one", "two",   "three", "four",
                              "five", "six", "seven", "eight", "nine"};
  const uint8_t DIGITS_SIZE = sizeof(DIGITS_STR) / sizeof(DIGITS_STR[0]);

  // We return a pointer, so we can later determine if we found something - or
  // not
  uint16_t* ptr_to_int = NULL;

  for (size_t idx = 0; idx < DIGITS_SIZE; ++idx) {
    // Is one number found at the first position?
    if (str == strstr(str, DIGITS_STR[idx])) {
      // If yes, allocate memory for the return value and convert the string to
      // an int
      ptr_to_int = malloc(sizeof(uint16_t));
      *ptr_to_int = idx;
      break;
    }
  }
  return ptr_to_int;
}

size_t get_single_calibration_value_two_star(char* line, size_t len) {
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
    } else if (isalpha(line[idx])) {
      uint16_t* digit = get_digit_at_start(&line[idx]);
      if (digit != NULL) {
        if (!first_digit_set) {
          first_digit = *digit;
          last_digit = *digit;
          first_digit_set = true;
        } else {
          last_digit = *digit;
        }
        free(digit);
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

  size_t calibration_value_one_star = 0;
  size_t calibration_value_two_stars = 0;

  // Extract from each line the calibration value
  while (fgets(buffer, buffer_size, fp)) {
    calibration_value_one_star +=
        get_single_calibration_value_one_star(buffer, strlen(buffer));
    calibration_value_two_stars +=
        get_single_calibration_value_two_star(buffer, strlen(buffer));
  }

  printf("Calibration value for one star = %ld\n", calibration_value_one_star);
  printf("Calibration value for two stars = %ld\n",
         calibration_value_two_stars);

  fclose(fp);

  exit(EXIT_SUCCESS);
}
