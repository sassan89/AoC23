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

  if (fp == NULL){
    exit(EXIT_FAILURE);
  }

  size_t calibration_value = 0;

  size_t line_no = 1;

  // Extract from each line the calibration value
  while(fgets(buffer, buffer_size, fp)){
    calibration_value += get_single_calibration_value(buffer, strlen(buffer));
    printf("%ld: temp val = %ld\n", line_no, calibration_value);
    line_no++;
  }

  printf("Calibration value = %ld\n", calibration_value);
  
  fclose(fp);

  exit(EXIT_SUCCESS);
}
