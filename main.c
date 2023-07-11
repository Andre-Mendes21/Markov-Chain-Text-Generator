#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SV_IMPLEMENTATION
#include "./sv.h"
#define STB_DS_IMPLEMENTATION
#include "./stb_ds.h"

#define ARRLEN(arr) \
    (sizeof((arr)) / sizeof((arr[0])))

typedef struct {
    char* key;
    String_View* value;
} GraphText;

void gt_print(GraphText* gt) {
    for(int i = 0; i < shlen(gt); ++i) {
        String_View* val = shget(gt, gt[i].key);
        printf("key: %s\n", gt[i].key);

        for(int j = 0; j < arrlen(val); j++) {
            printf("\tvalue[%d]: %s\n", j, val[j].data);
        }
    }
}

void gt_add_keyword(GraphText* gt, const char* keyword, String_View next_word) {
    // checks if word exists in hashTable
    String_View* keyword_vals = shget(gt, keyword);
    if(keyword_vals != NULL) {
        for(int i = 0; i < arrlen(keyword_vals); i++) {
            if(sv_eq_ignorecase(keyword_vals[i], next_word) == true) {
                return;
            }
        }
    }
    arrput(keyword_vals, next_word);
    shput(gt, keyword, keyword_vals);
}

String_View* read_file(char* filename) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        return NULL;
    }

    int ch;
    char word[256];
    String_View* text = NULL;
    int i = 0;
    while((ch = fgetc(file)) != EOF) {
        if(isspace(ch)) {
            word[i] = '\0';
            char* temp = malloc(strlen(word) + 1);
            memcpy(temp, word, strlen(word) + 1);
            arrput(text, SV(temp));
            i = 0;
        }
        if(isalpha(ch)) {
            word[i++] = ch;
        }
    }
    fclose(file);
    return text;
}

int main(int argc, char** argv) {
    GraphText* gt = NULL;
    sh_new_arena(gt);
    
    if(argc < 4) {
        fputs("Input error: Must be 3 or more arguments\n", stdout);
        return EXIT_FAILURE;
    }
    String_View* file = read_file(argv[1]);
    if(file == NULL) {
        perror("ERROR: POINTER NOT CREATED\n");
        return EXIT_FAILURE;
    }

    for(int i = 1; i < arrlen(file); i++) {
        gt_add_keyword(gt, file[i-1].data, file[i]);
    }

    gt_print(gt);
    
    return 0;
}