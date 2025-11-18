#include "s21_decimal.h"

void full_decimal(s21_decimal* num) {
  for (int i = 0; i < 96; i++) {
    set_bit(num, i, 1);
  }
}
