#include "util.h"

#include <ctype.h>
#include <string.h>

//两边去除空格(' ')、水平定位字符('\t')、归位键('\r')、换行('\n')、垂直定位字符('\v')或翻页('\f')
char* trim(char *str)
{
    char *left = str;
    char *right = str + (strlen(str) - 1);

    while (isspace(*left)) {
        left++;
    }
    while (isspace(*right)) {
        right--;
    }

    if (left == str) {
        *++right = '\0';
    }
    else {
        char *p = str;
        while (left <= right) {
            *p++ = *left++;
        }
        *p = '\0';
    }

    return str;
}

//返回分隔符后的字符串
char* split(char *str, char delimiter)
{
    char *p = strchr(str, delimiter);
    if (p != NULL) {
        *p = '\0';
        return p + 1;
    }
    return NULL;
}

//对指定字符转义
// this function will return a new alloced string
// with all escape chars prefixed with '\\'
char* escape(const char *str, char escape) {
    const char escape_prefix = '\\';
    const int len = strlen(str);
    char *escaped_str = (char *) malloc(sizeof(char) * len * 2);
    int i = 0, j = 0;
    // use i <= len so that the last '\0' is also copied
    for (; i <= len; i++) {
        if (str[i] == escape) {
            escaped_str[j++] = escape_prefix;
        }
        escaped_str[j++] = str[i];
    }
    return escaped_str;
}
