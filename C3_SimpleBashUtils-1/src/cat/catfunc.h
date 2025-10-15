#ifndef CATFUNC_H
#define CATFUNC_H

#include <stdio.h>

FILE *open_n_check_file(const char *filepath);
void cat_e(char *line);
void cat_E(char *line);
void cat_T(char *line);
int cat_b(char *line);
int cat_s(char *line, char *prev_line);
int cat_v_for_sim(char line);
void paste_sim_on_str(char *str, int pos, char sim);
void cat_t(char *str);
void close_file(FILE *file);
void cat_v(char *line);
struct CommandLineArgs parseCommandLineArgs(int argc, char *argv[]);
struct CommandLineArgs {
  char b_flag;
  char n_flag;
  char s_flag;
  char e_flag;
  char v_flag;
  char t_flag;
  char E_flag;
  char T_flag;
};

#endif
