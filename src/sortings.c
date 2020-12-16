#include "sortings.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#define CHAR_COUNT (UCHAR_MAX + 1)

#define swap(str1, str2) char* buffer = str1;str1 = str2;str2 = buffer


void bubble(strings_array_t strings, array_size_t amount_of_strings, comparator_func_t comparator){
    for (array_size_t i = 0; i<amount_of_strings; i++){
        for (array_size_t j = 1; j<amount_of_strings; j++){
            if (comparator(strings[j-1],strings[j]) > 0){
                swap(strings[j-1],strings[j]);
            }
        }
    }
}

void insertion(strings_array_t strings, array_size_t amount_of_strings, comparator_func_t comparator){
    for (array_size_t i = 1; i<amount_of_strings; i++){
        array_size_t j = i;
        while (j>0 && comparator(strings[j-1], strings[j])>0){
            swap(strings[j-1], strings[j]);
            j--;
        }
    }
}

void merge(strings_array_t strings, array_size_t amount_of_strings, comparator_func_t comparator){
    if (amount_of_strings == 2){
        if (comparator(strings[0], strings[1])>0){
            swap(strings[0], strings[1]);
        }
    }
    else if (amount_of_strings > 1){
        merge(&strings[0], amount_of_strings/2, comparator);
        merge(&strings[amount_of_strings/2], amount_of_strings-amount_of_strings/2, comparator);

        strings_array_t buffer = (strings_array_t)malloc((MAX_INPUT_STRING_SIZE+1) * amount_of_strings);
        if (buffer == NULL) {
            fprintf(stderr, "failed to find memory for merge sort\n");
            return;
        }

        array_size_t i = 0, j = amount_of_strings/2, pointer = 0;
        while (i<amount_of_strings/2 && j<amount_of_strings){
            if (comparator(strings[i],strings[j])>0){
                buffer[pointer] = strings[j];
                j++;
            }
            else{
                buffer[pointer] = strings[i];
                i++;
            }
            pointer++;
        }
        //in case i is still <amount_of_strings/2 or j<amount_of_strings, which means we didn't add all the strings to the buffer
        while (i<amount_of_strings/2){
            buffer[pointer] = strings[i];
            i++;
            pointer++;
        }

        while (j<amount_of_strings){
            buffer[pointer] = strings[j];
            j++;
            pointer++;
        }

        for (array_size_t iter = 0; iter<amount_of_strings; iter++){
            strings[iter] = buffer[iter];
        }

        free(buffer);
    }
}

static void quick_split(strings_array_t strs, unsigned int beg, const unsigned int end, comparator_func_t cmp) {
    while (beg < end) {
        if ((strs[beg] <= strs[(beg + end - 1) / 2] && strs[(beg + end - 1) / 2] <= strs[end - 1]) || (strs[end - 1] <= strs[(beg + end - 1) / 2] && strs[(beg + end - 1) / 2] <= strs[beg])) {
            swap(strs[beg], strs[(beg + end - 1) / 2]);
        } else if ((strs[beg] <= strs[end - 1] && strs[end - 1] <= strs[(beg + end - 1) / 2]) || (strs[(beg + end - 1) / 2] <= strs[end - 1] && strs[end - 1] <= strs[beg])) {
            swap(strs[beg], strs[end - 1]);
        }
        // Partition order: [beg, left - 1] -- less than pivot, [left, mid - 1] -- equal to pivot, [right, end - 1] -- greater than pivot
        unsigned int left = beg, mid = beg + 1, right = end;
        for (unsigned int i = 0; i < end - beg - 1; i++) {
            const int cmp_result = cmp(strs[mid], strs[beg]);
            if (cmp_result < 0) {
                left++;
                swap(strs[mid], strs[left]);
                mid++;
            } else if (cmp_result > 0) {
                right--;
                swap(strs[mid], strs[right]);
            } else {
                mid++;
            }
        }
        swap(strs[beg], strs[left]);
        quick_split(strs, beg, left, cmp);
        beg = right;
    }
}

void quick(strings_array_t strings, array_size_t amount_of_strings, comparator_func_t comparator){
    quick_split(strings, 0, amount_of_strings, comparator);
}

static void radix_core_part(strings_array_t lines, array_size_t count, comparator_func_t comp, array_size_t l, array_size_t r, array_size_t d) {
    if (d >= MAX_INPUT_STRING_SIZE || l + 1 >= r) return;

    size_t cmp_proxy[CHAR_COUNT];
    if (comp("a", "b") < 0) for (size_t i = 0; i < CHAR_COUNT; ++i) cmp_proxy[i] = i;
    else for (size_t i = 0; i < CHAR_COUNT; ++i) cmp_proxy[i] = CHAR_COUNT - i - 1;

    int cnt[CHAR_COUNT] = {0};
    for (array_size_t i = l; i < r; ++i) cnt[cmp_proxy[(unsigned char) lines[i][d]]]++;
    for (int i = 1; i < CHAR_COUNT; ++i) cnt[i] += cnt[i - 1];
    int cnt_copy[CHAR_COUNT];
    memcpy(cnt_copy, cnt, (CHAR_COUNT) * sizeof *cnt);

    strings_array_t c = malloc((r - l) * sizeof *c);
    if (c == NULL) {
        fprintf(stderr, "failed to find memory for radix sort\n");
        return;
    }
    for (array_size_t i = l; i < r; ++i) {
        c[--cnt[cmp_proxy[(unsigned char) lines[i][d]]]] = lines[i];
    }

    memcpy(lines + l, c, (r - l) * sizeof *c);

    radix_core_part(lines, count, comp, l, l + cnt_copy[0], d + 1);
    for (int i = 1; i < CHAR_COUNT; ++i) {
        radix_core_part(lines, count, comp, l + cnt_copy[i - 1], l + cnt_copy[i], d + 1);
    }

    free(c);
}

void radix(strings_array_t strings, array_size_t amount_of_strings, comparator_func_t comparator){
    radix_core_part(strings, amount_of_strings, comparator, 0, amount_of_strings, 0);
}