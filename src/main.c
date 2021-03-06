#include <stdio.h>
#include "sortings.h"
#include <stdlib.h>
#include <string.h>


int asc(const char* str1, const char* str2){
    return strcmp(str1, str2);
}

int des(const char* str1, const char* str2){
    return -strcmp(str1, str2);
}

int is_not_digit(char c){
    if (c=='0') return 0;
    if (c=='1') return 0;
    if (c=='2') return 0;
    if (c=='3') return 0;
    if (c=='4') return 0;
    if (c=='5') return 0;
    if (c=='6') return 0;
    if (c=='7') return 0;
    if (c=='8') return 0;
    if (c=='9') return 0;
    return 1;
}

int check_input(char** argv, strings_array_t possible_sorts, strings_array_t possible_comps){
    int sortcheck = 1;
    for (int i=0; i<5; i++){ //5 is amount of strings in possible_sorts
        if (strncmp(argv[4], possible_sorts[i], strlen(possible_sorts[i]))==0) sortcheck = 0;
    }
    if (sortcheck != 0){
        fprintf(stderr, "sorting name %s is invalid\n", argv[4]);
    }

    int compcheck = 1;
    for (int i=0; i<2; i++){ //2 is amount of strings in possible_comps
        if (strncmp(argv[5], possible_comps[i], strlen(possible_comps[i]))==0) compcheck = 0;
    }
    if (compcheck != 0){
        fprintf(stderr, "comparator name %s is invalid\n", argv[5]);
    }

    if (sortcheck || compcheck) return -1;

    for(size_t i = 0; i<strlen(argv[1]); i++){
        if (is_not_digit(argv[1][i])){
            fprintf(stderr, "the amount of strings %s is not a number\n", argv[1]);
            return -1;
        }
    }
    return 0;
}

int read_strings(char* input_file_name, strings_array_t strings, array_size_t amount_of_strings){
    FILE* input_file = fopen(input_file_name, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Failed to open given input file\n");
        return -1;
    }

    for (array_size_t i = 0; i<amount_of_strings; i++){
        if(fgets(strings[i], MAX_INPUT_STRING_SIZE+1, input_file)==NULL){
            fprintf(stderr, "Failed to read given input file\n");
            return -1;
        }
    }

    fclose(input_file);

    const char* enter = "\n";

    if (strings[amount_of_strings-1][strlen(strings[amount_of_strings-1])-1] != '\n'){
        strncat(strings[amount_of_strings-1], enter, 1);
    }
    return 0;
}

void sort_strings(strings_array_t strings, array_size_t amount_of_strings, char* sorting, char* comparator){
    if (strncmp(comparator, "asc", 3) == 0){ // the length of the string "asc"
        if (strncmp(sorting, "bubble", 6) == 0) bubble(strings, amount_of_strings, asc);
        else if (strncmp(sorting, "insertion", 9) == 0) insertion(strings, amount_of_strings, asc);
        else if (strncmp(sorting, "merge", 5) == 0) merge(strings, amount_of_strings, asc);
        else if (strncmp(sorting, "quick", 5) == 0) quick(strings, amount_of_strings, asc);
        else if (strncmp(sorting, "radix", 5) == 0) radix(strings, amount_of_strings, asc);
    }
    else{
        if (strncmp(sorting, "bubble", 6) == 0) bubble(strings, amount_of_strings, des);
        else if (strncmp(sorting, "insertion", 9) == 0) insertion(strings, amount_of_strings, des);
        else if (strncmp(sorting, "merge", 5) == 0) merge(strings, amount_of_strings, des);
        else if (strncmp(sorting, "quick", 5) == 0) quick(strings, amount_of_strings, des);
        else if (strncmp(sorting, "radix", 5) == 0) radix(strings, amount_of_strings, des);
    }
}

int write_strings(char* output_file_name, strings_array_t strings, array_size_t amount_of_strings){
    FILE* output_file = fopen(output_file_name, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Failed to open given output file\n");
        return -1;
    }

    for (array_size_t i = 0; i<amount_of_strings; i++){
        if(fputs(strings[i], output_file)==EOF){
            fprintf(stderr, "Failed to fill given output file\n");
            return -1;
        }
    }

    fclose(output_file);

    return 0;
}

int main(int argc, char** argv) {
    if (argc != 6){
        fprintf(stderr, "too little or too much params: it is supposed to be 5 params\n");
        return -1;
    }

    strings_array_t possible_sorts = (strings_array_t)malloc(10 * 5);
    possible_sorts[0] = "bubble";
    possible_sorts[1] = "insertion";
    possible_sorts[2] = "merge";
    possible_sorts[3] = "quick";
    possible_sorts[4] = "radix";

    strings_array_t possible_comps = (strings_array_t)malloc(3 * 2);
    possible_comps[0] = "asc";
    possible_comps[1] = "des";

    int err = check_input(argv, possible_sorts, possible_comps);
    if (err != 0) return err;

    array_size_t amount_of_strings = atoll(argv[1]);

    if (amount_of_strings == 0) {
        FILE* output_file = fopen(argv[3], "wb");
        if (output_file == NULL) {
            fprintf(stderr, "Failed to open given output file\n");
            return -1;
        }

        if(fputs("\n", output_file)==EOF){
            fprintf(stderr, "Failed to fill given output file\n");
            return -1;
        }

        fclose(output_file);
    }
    else{

    strings_array_t strings = (strings_array_t)malloc((MAX_INPUT_STRING_SIZE+1) * amount_of_strings);
    if(strings == NULL){
        fprintf(stderr, "Failed to find enough memory in main\n");
        return -1;
    }

    for (array_size_t i = 0; i<amount_of_strings; i++){
        strings[i] = calloc(MAX_INPUT_STRING_SIZE+1, sizeof(char));
        if (strings[i] == NULL){
            fprintf(stderr, "Failed to give memory to one of the strings\n");
        }
    }

    err = read_strings(argv[2], strings, amount_of_strings);
    if (err != 0) return err;

    sort_strings(strings, amount_of_strings, argv[4], argv[5]);

    err = write_strings(argv[3], strings, amount_of_strings);
    if (err != 0) return err;

    free(strings);

    return 0;}
}