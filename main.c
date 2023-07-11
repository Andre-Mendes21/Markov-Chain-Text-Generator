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

void gt_add_keyword(GraphText* gt, char* keyword, String_View next_word) {
    // checks if word exists in hashTable
    String_View* keyword_vals = shget(gt, keyword);
    if(keyword_vals != NULL) {
        for(int i = 0; i < arrlen(keyword_vals); i++) {
            if(sv_eq_ignorecase(keyword_vals[i], next_word) == true) {
                return;
            }
        }
    }
    arrpush(keyword_vals, next_word);
    shput(gt, keyword, keyword_vals);
}

int main(int argc, char** argv) {
    GraphText* gt = NULL;
    sh_new_arena(gt);
    
    char* test_key = "hello";
    char* test_value = "world";
    char* test_value2 = "hello";
    
    gt_add_keyword(gt, test_key, SV(test_value));
    gt_add_keyword(gt, test_key, SV(test_value2));

    gt_print(gt);
    
    return 0;
}