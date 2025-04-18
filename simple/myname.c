
const char* const_global_var = "I'm a constant";
char* global_var = "Hello";
char* uninit_global_var;

static char* static_var = "World";
static char* uninit_static_var;

static char* get_name_local_func() {
    return static_var;
}

char* get_name_global_func() {
    return get_name_local_func();
}

__attribute__((weak))
char* get_name_weak() {
    return get_name_local_func();
}

__attribute__((visibility("hidden")))
char* get_name_hidden() {
    return get_name_local_func();
}

__attribute__((visibility("protected")))
char* get_name_protected() {
    return get_name_local_func();
}

__attribute__((visibility("internal")))
char* get_name_internal() {
    return get_name_local_func();
}
