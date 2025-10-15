#include "catfunc.h"

#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

void close_file(FILE *file) { fclose(file); }

FILE *open_n_check_file(const char *filepath) {
  DIR *dir = opendir(filepath);
  if (dir != NULL) {
    closedir(dir);
    printf("cat: %s: Is a directory\n", filepath);
    return NULL;
  } else {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
      printf("cat: %s: No such file or directory\n", filepath);
      return NULL;
    }
    return file;
  }
}

struct CommandLineArgs parseCommandLineArgs(int argc, char *argv[]) {
  struct CommandLineArgs args = {0};

  static struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"show-nonprinting", no_argument, NULL, 'v'},
      {"E", no_argument, NULL, 'E'},
      {"T", no_argument, NULL, 'T'},
      {0, 0, 0, 0}};

  int opt;
  while ((opt = getopt_long(argc, argv, "ebnsvtET", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'e':
        args.e_flag += 1;
        break;
      case 'b':
        args.b_flag += 1;
        break;
      case 'n':
        args.n_flag = 1;
        break;
      case 's':
        args.s_flag = 1;
        break;
      case 'v':
        args.v_flag = 1;
        break;
      case 't':
        args.t_flag = 1;
        break;
      case 'E':
        args.E_flag = 1;
        break;
      case 'T':
        args.T_flag = 1;
        break;

      case '?':
        printf("cat: illegal option -- %c\n", opt);
        printf("usage: cat [-benstv] [file ...]\n");
        break;
      default:
        break;
    }
  }

  return args;
}

void paste_sim_on_str(char *str, int pos, char sim) {
  int len = strlen(str);
  for (int i = len; i >= pos; i--) {
    str[i + 1] = str[i];
  }
  str[pos] = '^';
  str[pos + 1] = sim;
}

void temp(char *chr1, char *chr2) {
  char temp = *chr1;
  *chr1 = *chr2;
  *chr2 = temp;
}

void cat_e(char *line) {
  cat_v(line);
  int line_length = strlen(line);
  if (line[line_length - 1] == '\n') {
    line[line_length - 1] = '$';
    line[line_length] = '\n';
    line[line_length + 1] = '\0';
  }
}

void cat_v(char *line) {
  for (int i = 0; line[i] != '\0'; i++) {
    if (line[i] != cat_v_for_sim(line[i])) {
      paste_sim_on_str(line, i, cat_v_for_sim(line[i]));
    }
  }
}

int cat_b(char *line) { return (line[0] != '\n') ? 1 : 0; }

int cat_v_for_sim(char line) {
  // int next = 0;
  if (line < -96) {
    line = line + 192;
    // next = 1;
  } else if (line < 0) {
    line = line + 128;
    // next = 1;
  } else if (line == 10 || line == 9) {
    line = line + 0;
  }
  // s = s;
  else if (line < 32) {
    line = line + 64;
    // next = 1;
    //} else if (line[pos] < 32)
    // s = s;
    // else {
    // printf("^?");
    // next = 1;
  }
  return line;
}

int cat_s(char *line, char *prev_line) {
  return (line[0] == '\n' && prev_line[0] == '\n') ? 1 : 0;
}

void cat_t(char *str) {
  cat_v(str);
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == '\t') {
      paste_sim_on_str(str, i, 'I');
    }
  }
}

void cat_T(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == '\t') {
      paste_sim_on_str(str, i, 'I');
    }
  }
}

void cat_E(char *line) {
  int line_length = strlen(line);
  if (line[line_length - 1] == '\n') {
    line[line_length - 1] = '$';
    line[line_length] = '\n';
    line[line_length + 1] = '\0';
  }
}
