#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SV_IMPLEMENTATION
#include "./sv.h"
#define STB_DS_IMPLEMENTATION
#include "./stb_ds.h"

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

GraphText* gt_add_keyword(GraphText* gt, String_View keyword, String_View next_word) {
    // checks if word exists in hashTable
    String_View* keyword_vals = shget(gt, keyword.data);
    if(keyword_vals != NULL) {
        for(int i = 0; i < arrlen(keyword_vals); i++) {
            if(sv_eq_ignorecase(keyword_vals[i], next_word) == true) {
                return gt;
            }
        }
    }
    arrpush(keyword_vals, next_word);
    shput(gt, keyword.data, keyword_vals);
    return gt;
}

GraphText* gt_fill(GraphText* gt, String_View* text) {
    for(int i = 1; i < arrlen(text); i++) {
        gt = gt_add_keyword(gt, text[i-1], text[i]);
    }
    return gt;
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
            word[i++] = tolower(ch);
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

    gt = gt_fill(gt, file);

    gt_print(gt);
    return 0;
}