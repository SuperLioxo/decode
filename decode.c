/*
*解答思路：
*通过栈管理嵌套层级
*遇到[时保存当前字符串和重复次数
*遇到]时弹出并将括号内字符串重复指定次数后拼接
*数字累计重复次数、字母直接追加，完成解码。
*/
#include <corecrt.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//自定义栈结构体，保存字符串s和重复次数k
typedef struct {
    char *str;
    int num;
} StackElement;

//重复函数，将字符串重复k遍后输出新字符串
char *repeat_str(char *str, int k) {
    if (k <= 0 || str == NULL) {
        char *res = malloc(1);
        if (res) res[0] = '\0';
        return res;
    }
    int len = strlen(str);
    int total_len = len * k;
    char *res = (char*)malloc(total_len + 1);
    if (!res) {
        fprintf(stderr, "Malloc failed(repeat_str)\n");
        exit(EXIT_FAILURE);
    }
    
    res[0] = '\0';
    for (int i = 0; i < k; ++i) {
        strcat(res, str);
    }
    return res;
}

char *decodeString(const char *str) {
    if (!str) {
        return NULL;
    }

    char *curr_str = (char*)malloc(1);
    curr_str[0] = '\0';
    int num = 0;

    StackElement stack[1024];
    int top = -1;

    for (int i = 0; str[i] != '\0'; ++i) {
        char c = str[i];

        if (isdigit((unsigned char)c)) {
            num = num *10 + (c - '0');
        } else if (c == '[') {
            if (top >= 1023) {
                fprintf(stderr, "Stack overflow\n");
                exit(EXIT_FAILURE);
            }
            stack[++top].str = curr_str;
            stack[top].num = num;

            curr_str = (char*)malloc(1);
            curr_str[0] = '\0';
            num = 0;
        } else if (c == ']') {
            if (top < 0) {
                fprintf(stderr, "Mismatched brackets\n");
                exit(EXIT_FAILURE);
            }
            StackElement prev = stack[top--];

            char *repeated = repeat_str(curr_str, prev.num);

            size_t new_len = strlen(prev.str) + strlen(repeated) + 1;
            char *new_str = (char*)malloc(new_len);
            strcpy(new_str, prev.str);
            strcat(new_str, repeated);

            free(curr_str);
            free(repeated);
            free(prev.str);

            curr_str = new_str;
        } else {
            size_t len = strlen(curr_str);
            curr_str = realloc(curr_str, len + 2);
            curr_str[len] = c;
            curr_str[len + 1] = '\0';
        }
    }

    return curr_str;
}

int main(void) {
    const char *s = "3[a]2[bdc]";
    char *decoded = decodeString(s);
    printf("输入: %-15s 输出: %s\n", s, decoded);
    free(decoded);

    return 0;
}