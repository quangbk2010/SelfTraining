#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void escape (char *s, char *t) {
    int i = 0, j = 0;
    while (s[i]) {
        switch (s[i])  {
            case '\a': 
                t[j++] = '\\';
                t[j] = 'a';
                break;
                
            case '\b':
                t[j++] = '\\';
                t[j] = 'b';
                break;
                
            case '\f':
                t[j++] = '\\';
                t[j] = 'f';
                break;

            case '\n':
                t[j++] = '\\';
                t[j] = 'n';
                break;

            case '\r':
                t[j++] = '\\';
                t[j] = 'r';
                break;

            case '\t':
                t[j++] = '\\';
                t[j] = 't';
                break;

            case '\v':
                t[j++] = '\\';
                t[j] = 'v';
                break;

            case '\\':
                t[j++] = '\\';
                t[j] = '\\';
                break;

            case '\'':
                t[j++] = '\\';
                t[j] = s[i];
                break;

            case '\"':
                t[j++] = '\\';
                t[j] = s[i];
                break;

            case '\?':
                t[j++] = '\\';
                t[j] = s[i];
                break;
            default:
                t[j] = s[i];
                break;
        }
        i ++;
        j ++;
    }
    t[j] = s[i];
}

int main () {
    char s[] = "a\nb";// "a\ab\bc\fd\ne\rf\tg\vh\\i\'j\"k\?l";
    char *t;
    t = (char *) malloc (strlen (s) + 1);
    escape (s, t);
    printf ("String: %s\nLength: %u\nConverted string: %s\nLength: %u\n", s, (int) strlen (s), t, (int) strlen (t));
    return 0;
}
