#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define SV_IMPLEMENTATION
#include "./include/sv.h"
#define STB_DS_IMPLEMENTATION
#include "./include/stb_ds.h"

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
    String_View* keyword_vals = shget(gt, keyword.data);
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


void gt_generate_text(GraphText* gt, int num_words, int num_sentences, FILE* file) {
    int gt_len = shlen(gt); 
    for(int i = 0; i < num_sentences; i++) {
        int key_idx = rand() % gt_len;
        const char* curr_key = gt[key_idx].key;
        for(int j = 0; j < num_words; j++) {
            String_View* curr_value = shget(gt, curr_key);
            int val_idx = rand() % arrlen(curr_value);
            const char* word = curr_value[val_idx].data;
            fputs(word, file);
            fputs(" ", file);
            curr_key = word;
        }
        fputs("\n\n", file);
    }
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
            strcpy(temp, word);
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
    srand(time(NULL));
    GraphText* gt = NULL;
    sh_new_arena(gt);
    
    if(argc < 4) {
        fputs("Input error: Must be 3 or more arguments\n", stdout);
        return EXIT_FAILURE;
    }
    char* filename = argv[1];
    int num_words = atoi(argv[2]);
    int num_sentences = atoi(argv[3]);
    String_View* file = read_file(filename);
    if(file == NULL) {
        perror("ERROR: POINTER NOT CREATED\n");
        return EXIT_FAILURE;
    }
    if(num_words <= 0 || num_sentences <= 0) {
        fputs("Input error: Arguments 2 and 3 must be positive and non-zero\n", stdout);
        return EXIT_FAILURE;
    }
    gt = gt_fill(gt, file);

    FILE* write_file = stdout;
    if(argv[4] != NULL) {
        write_file = fopen(argv[4], "w");
    }
    gt_generate_text(gt, num_words, num_sentences, write_file);

    // Cleanup
    for(int i = 0; i < arrlen(file); i++) {
        free((void *) file[i].data);
        file[i].data = NULL;
    }
    arrfree(file);
    shfree(gt);
    fclose(write_file);
    return 0;
}