#include <stdio.h>
#include <string.h>
#include <ctype.h>

void flag_e(char *temp, char *file, int flag, int s_flag);
void flag_i(char *temp, char *file, int flag);
void flag_v(char *temp, char *file, int flag);
void flag_c(char *temp, char *file, int flag);
void flag_n(char *temp, char *file, int flag);
void flag_l(char *temp, char *file);
void flag_f(char *temp, char *file, int flag);

int main(int argc, char **argv) {
    int flag;
    int s_flag = 0;
    if (argc < 4) {
        printf("Usage: %s [-e | -i | -v | -c | -l | -n | -h | -s | -f file] template <filename>", argv[0]);
        return 1;
    } else if (argc >= 4){
        if (argc==4){
            flag = 0;
        } else {
            flag = 1;
        }
        char *opt = argv[1];
        char *temp = argv[2];
        if (strcmp(opt, "-e") == 0) {
            for (int i = 3; i<argc;i++){
                flag_e(temp,argv[i], flag, s_flag);
            }
        } else if (strcmp(opt, "-i") == 0) {
            for (int i = 3; i<argc;i++){
                flag_i(temp,argv[i], flag);
            }
        } else if (strcmp(opt, "-v") == 0) {
            for (int i = 3; i<argc;i++){
                flag_v(temp,argv[i], flag);
            }
        } else if (strcmp(opt, "-c") == 0) {
            for (int i = 3; i<argc;i++){
                flag_c(temp,argv[i], flag);
            }
        } else if (strcmp(opt, "-n") == 0) {
            for (int i = 3; i<argc;i++){
                flag_n(temp,argv[i], flag);
            }
        } else if (strcmp(opt, "-l") == 0) {
            for (int i = 3; i<argc;i++){
                flag_l(temp,argv[i]);
            }
        } else if (strcmp(opt, "-h") == 0) {
            for (int i = 3; i<argc;i++){
                flag_e(temp,argv[i], 0, s_flag);
            }
        } else if (strcmp(opt, "-s") == 0) {
            s_flag = 1;
            for (int i = 3; i<argc;i++){
                flag_e(temp,argv[i], flag, s_flag);
            }
        } else if (strcmp(opt, "-f") == 0) {
            for (int i = 3; i<argc;i++){
                flag_f(temp, argv[i], flag);
            }
        }
    }
    return 0;
}

void flag_e(char *temp, char *file, int flag, int s_flag) {
    FILE *f = fopen(file, "rt");
    if (f!=NULL) {
        char str[256];
        while (fgets(str, 256, f) != NULL){
            if (strstr(str, temp) != NULL){
                if (flag == 0){
                    printf("%s",str);
                } else {
                    printf("%s:%s",file, str);
                }
                if (strchr(str,'\n') == NULL){
                    printf("\n");
                }
            }
        }
    } else {
        if (s_flag == 0){
            printf("grep: %s: No such file or directory\n", file);
        }
    }
    fclose(f);
}

void flag_i(char *temp, char *file, int flag) {
    FILE *f = fopen(file, "rt");
    if (f!=NULL) {
        int temp_len = strlen(temp);
        char str[256];
        while(fgets(str, 256, f) != NULL) {
            int str_len = strlen(str);
            int match = 0;
            for (int i = 0; i < str_len; i++){
                if (tolower(str[i]) == tolower(temp[match])){
                    match = match + 1;
                    if (match == temp_len){
                        if (flag == 0){
                            printf("%s", str);
                        } else {
                            printf("%s:%s",file, str);
                        }
                        i = str_len;
                        if(strchr(str,'\n') == NULL){
                            printf("\n");
                        }
                    }
                } else {
                    match = 0;
                }
            }
        }
    } else {
        printf("grep: %s: No such file or directory\n", file);
    }
    fclose(f);
}

void flag_v(char *temp, char *file, int flag) {
    FILE *f = fopen(file, "rt");
    if (f!=NULL) {
        char str[256];
        while (fgets(str, 256, f) != NULL){
            if (strstr(str, temp) == NULL){
                if (flag==0){
                    printf("%s",str);
                } else {
                    printf("%s:%s",file,str);
                }
                if(strchr(str,'\n') == NULL){
                    printf("\n");
                }
            }
        }
    } else {
        printf("grep: %s: No such file or directory\n", file);
    }
    fclose(f);
}

void flag_c(char *temp, char *file, int flag) {
    FILE *f = fopen(file, "rt");
    if (f!=NULL) {
        char str[256];
        int match = 0;
        while (fgets(str, 256, f) != NULL){
            if (strstr(str, temp) != NULL){
                match++;
            }
        }
        if (flag == 0){
            printf("%d\n", match);
        } else {
            printf("%s:%d\n",file, match);
        }
    } else {
        printf("grep: %s: No such file or directory\n", file);
    }
    fclose(f);
}

void flag_n(char *temp, char *file, int flag) {
    FILE *f = fopen(file, "rt");
    if (f!=NULL) {
        char str[256];
        int i = 0;
        while (fgets(str, 256, f) != NULL){
            i++;
            if (strstr(str, temp) != NULL){
                if (flag == 0){
                    printf("%d:%s",i,str);
                } else {
                    printf("%s:%d:%s",file,i,str);
                }
                if(strchr(str,'\n') == NULL){
                    printf("\n");
                }
            }
        }
    } else {
        printf("grep: %s: No such file or directory\n", file);
    }
    fclose(f);
}

void flag_l(char *temp, char *file) {
    FILE *f = fopen(file, "rt");
    if (f!=NULL) {
        char str[256];
        int count = 0;
        while (fgets(str, 256, f) != NULL){
            if (strstr(str, temp) != NULL){
                count++;
            }
        }
        if (count > 0){
            printf("%s\n",file);
        }
    } else {
        printf("grep: %s: No such file or directory\n", file);
    }
    fclose(f);
}

void flag_f(char *temp, char *file, int flag) {
    FILE *pat = fopen(temp, "rt");
    if (pat!=NULL){
        FILE *f = fopen(file, "rt");
        if (f!=NULL) {
            char str[256];
            char p[256];
            while (fgets(p, 256, pat) != NULL){
                while(fgets(str, 256, f) != NULL){
                    if (strstr(str, p) != NULL){
                        if (flag == 0){
                            printf("%s",str);
                        } else {
                            printf("%s:%s",file, str);
                        }
                        if (strchr(str,'\n') == NULL){
                            printf("\n");
                        }
                    }
                }
            }
        } else {
            printf("grep: %s: No such file or directory\n", file);
        }
        fclose(f);
    } else {
        printf("grep: %s: No such file or directory\n", temp);
    }
    fclose(pat);
}