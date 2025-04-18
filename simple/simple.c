#include <stdio.h>

extern const char* const_global_var;
extern char* global_var;

extern char* get_name_global_func();

char* simple_global_var = "HELLO";
static char* simple_name = "WORLD";

void print_message() {
    printf("%s\n", const_global_var);
    printf("%s %s\n", global_var, get_name_global_func());
    printf("%s %s\n", simple_global_var, simple_name);
}

extern char* get_name_weak() __attribute__((weak));

char* use_get_name_weak() {
    if (&get_name_weak != NULL) {
        printf("Use get_name_weak()\n");
    } else {
        printf("No get_name_weak() is defined\n");
    }
}

int main() {
    print_message();
    use_get_name_weak();
    return 0;
}

