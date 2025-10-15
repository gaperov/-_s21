#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "catfunc.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: cat [-benstv] [file ...]\n");
    return 1;
  }

  struct CommandLineArgs args = parseCommandLineArgs(argc, argv);

  for (int i = optind; i < argc; i++) {
    FILE *file;
    int j = 1;
    int j_all = 1;

    file = open_n_check_file(argv[i]);
    if (file) {
      char line[10000];
      char prev_line[10000] = "";
      int skip_next_empty_line = 0;
      while (fgets(line, sizeof(line), file)) {
        // for (int n = 0; new_flags[n] != '\0'; n++) {
        if (args.v_flag) {
          cat_v(line);
        }
        if (args.e_flag && !skip_next_empty_line) {
          cat_e(line);
          // skip_next_empty_line = 1;
        }
        if (args.b_flag && !skip_next_empty_line) {
          if (cat_b(line)) {
            printf("%6d\t", j);
            j += 1;
          } else {
            if (args.e_flag) {
              printf("\t");
            }
          }
        }
        if (args.n_flag && !skip_next_empty_line && !args.b_flag) {
          printf("%6d\t", j_all);
          j_all += 1;
        }
        if (args.s_flag) {
          skip_next_empty_line = cat_s(line, prev_line);
        }
        if (args.t_flag) {
          cat_t(line);
        }
        if (args.T_flag) {
          cat_T(line);
        }
        if (args.E_flag) {
          cat_E(line);
        }
        if (!skip_next_empty_line) {
          printf("%s", line);
        }
        strcpy(prev_line, line);
      }

      close_file(file);
    }
  }

  return 0;
}