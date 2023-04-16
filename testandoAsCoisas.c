#include <stdio.h>
#include <string.h>

int main(){
    char teste[3] = "abc";
    char *aux = &teste[1];
    *aux = 'e';
    printf("%s\n\n", teste);
}
