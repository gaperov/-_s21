#include "s21_math.h"

int s21_abs(int x) { return (x < 0) ? -x : x; }

long double s21_ceil(double x) {
  long double result = 0.0;
  char str[311];
  if (x == 0.0 || s21_fabs(x) == s21_inf)
    result = x;
  else if (x != x)
    result = s21_nan;
  else {
    sprintf(str, "%.lf", x);
    sscanf(str, "%Lf", &result);
    result = result < x ? result + 1 : result;
  }
  return result;
}

long double s21_exp(double x) {
  long double result = 0;
  if (s21_fabs(x) == DBL_MAX || s21_fabs(x) == s21_inf)
    result = (x > 0) ? s21_inf : 0;
  else if (x != x)
    result = s21_nan;
  else if (s21_fabs(x) == 0.0)
    result = 1;
  else {
    long double fact = 1;
    long double pow_x = x;
    long double iter = 1;
    result = 1 + x;
    int i = 2;
    int flag = 1;
    while (s21_fabs(iter / result) > 1e-300 && flag) {
      fact *= i;
      pow_x *= x;
      iter = pow_x / fact;
      result += iter;
      i++;
      if (result > DBL_MAX) {
        result = (x > 0) ? s21_inf : 0;
        flag = 0;
      }
    }
  }
  return result;
}

long double s21_fabs(double x) { return x < 0 ? -x : x; }

long double s21_floor(double x) {
  long double res = (int)x;
  if (x == +s21_inf)
    res = +s21_inf;
  else if (x == -s21_inf)
    res = -s21_inf;
  else if (x != x)
    res = s21_nan;
  else if (x == -0)
    res = -0;
  else if (x == +0)
    res = +0;
  else if (x < 0 && res != x) {
    res -= 1;
  }
  return res;
}

long double s21_fmod(double x, double y) {
  long double res = 0.0;
  int sign = 1;
  if (x < 0.0) {
    sign = -1;
    x = s21_fabs(x);
  }
  if (y < 0.0) y = s21_fabs(y);
  if (y == 0.0 || s21_fabs(x) == s21_inf || x != x || y != y)
    res = s21_nan;
  else if (x == y || (s21_fabs(x) == 0.0 && y != 0.0 && y != s21_nan))
    res = 0;
  else if (x < y || (s21_fabs(y) == s21_inf && s21_fabs(x) != s21_inf))
    res = x;
  else {
    long long int mod = x / y;
    res = (long double)x - (mod * (long double)y);
  }
  return res * sign;
}

long double s21_log(double x) {
  long double res = s21_nan;
  if (x == 0)
    res = -s21_inf;
  else if (x == 1)
    res = 0;
  else if (x == s21_inf)
    res = s21_inf;
  else if (x > 0) {
    double min = -s21_e, max = x;
    while (min < max) {
      res = (max - min) / 2 + min;
      long double exp_res = s21_exp(res);
      if (exp_res == x)
        min = x;
      else if (exp_res < x)
        min = res + 0.00000001;
      else
        max = res - 0.00000001;
    }
  }
  return res;
}

long double s21_pow(double x, double y) {
  long double result = 1;
  if ((x == -1 && s21_fabs(y) == s21_inf) || x == 1 || s21_fabs(y) == 0.0)
    result = 1;
  else if (y == 1)
    result = x;
  else {
    if (s21_fabs(x) >= DBL_MAX) x = sign(x) * s21_inf;
    if (s21_fabs(y) >= DBL_MAX) y = sign(y) * s21_inf;
    if ((x == -0.0 && y < 0.0 && is_odd_int(y)) ||
        (x == -s21_inf && y > 0.0 && is_odd_int(y))) {
      result = -s21_inf;
    } else if ((s21_fabs(x) < 1 && y == -s21_inf) ||
               (s21_fabs(x) > 1 && y == +s21_inf) ||
               (x == +0.0 && y < 0.0 && is_odd_int(y)) ||
               ((s21_fabs(x) == 0.0) && y < 0.0 && !is_odd_int(y)) ||
               (y > 0.0 && !is_odd_int(y) && x == -s21_inf) ||
               (y > 0.0 && x == +s21_inf)) {
      result = +s21_inf;
    } else if (((x == +0.0 && y > 0.0 && is_odd_int(y)) ||
                ((s21_fabs(x) == 0.0) && y > 0.0 && !is_odd_int(y)) ||
                (s21_fabs(x) > 1 && y == -s21_inf) ||
                (s21_fabs(x) < 1 && y == +s21_inf) ||
                (x == -s21_inf && y < 0.0 && !is_odd_int(y)) ||
                (x == +s21_inf && y < 0.0))) {
      result = +0.0;
    } else if (is_odd_int(y) &&
               ((x == -0.0 && y > 0.0) || (x == -s21_inf && y < 0.0))) {
      result = -0.0;
    } else if ((x < 0.0 && (s21_fabs(y) != s21_inf) && ((int)y != y)) ||
               (x != x && y != 0) || (y != y && x != 1)) {
      result = s21_nan;
    } else {
      if (x < 0.0f) {
        x = s21_fabs(x);
        if (is_odd_int(y)) result = -1;
      }
      result *= s21_exp(y * s21_log(x));
    }
  }
  return result;
}

long double s21_sqrt(double x) {
  long double res = -s21_nan;
  double min = (x >= 1) ? 0 : x, max = (x >= 1) ? x : 1;
  while (x >= 0 && min < max) {
    res = (max - min) / 2 + min;
    long double sqrt = res * res;
    if (sqrt == x)
      min = x;
    else if (sqrt < x)
      min = res + 0.00000001;
    else
      max = res - 0.00000001;
  }
  return res;
}

long double s21_sin(double x) {
  long double res = s21_nan;
  if (s21_fabs(x) != s21_inf) {
    while (s21_fabs(x) >= 2 * s21_pi) x -= sign(x) * 2 * s21_pi;
    res = x;
    for (int i = 1; i <= 12; i++) {
      long double term = s21_pow(x, 1 + 2 * i) / factorial(1 + 2 * i);
      res = (i % 2 == 1) ? res - term : res + term;
    }
  }
  return res;
}

long double s21_cos(double x) {
  long double res = s21_nan;
  double fact = 1;
  double double_i;
  if (s21_fabs(x) != s21_inf) {
    while (s21_fabs(x) >= 2 * s21_pi) x -= sign(x) * 2 * s21_pi;
    res = 1;
    if (x == s21_pi)
      res = -1;
    else if (x == s21_pi / 2 || x == (2 * s21_pi) / 2)
      res = 0;
    else if (x != 0 && x != 2 * s21_pi) {
      for (int i = 1; i <= 12; i++) {
        double_i = i * 2;
        fact = fact * double_i * (double_i - 1);
        long double term = s21_pow(x, 2 * i) / fact;
        res = (i % 2 == 1) ? res - term : res + term;
      }
    }
  }
  return res;
}

long double s21_tan(double x) { return s21_sin(x) / s21_cos(x); }

long double s21_asin(double x) {
  long double res = s21_acos(x);
  if (res || res == 0) res = s21_pi / 2 - res;
  return res;
}

long double s21_acos(double x) {
  long double res = s21_nan;
  if (x == -1)
    res = s21_pi;
  else if (x == 1)
    res = 0;
  else if (s21_fabs(x) < 1) {
    double min = 0, max = s21_pi;
    while (min < max) {
      res = (max - min) / 2 + min;
      long double cos = s21_cos(res);
      if (cos == x)
        min = s21_pi;
      else if (cos > x)
        min = res + 0.00000001;
      else
        max = res - 0.00000001;
    }
  }
  return res;
}

long double s21_atan(double x) {
  long double res = s21_nan;
  double min = -s21_pi / 2, max = (x != x) ? min : s21_pi / 2;
  while (min < max) {
    res = (max - min) / 2 + min;
    long double tan = s21_tan(res);
    if (tan == x)
      min = max;
    else if (tan < x)
      min = res + 0.00000001;
    else
      max = res - 0.00000001;
  }
  return res;
}

double factorial(int n) {
  if (n == 0)
    return 1;
  else
    return n * factorial(n - 1);
}

int sign(double x) {
  int res = 0;
  res = (x > 0) ? 1 : -1;
  return res;
}

int is_odd_int(double y) { return ((int)y == y && (int)y % 2 != 0) ? 1 : 0; }