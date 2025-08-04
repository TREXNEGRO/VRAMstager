#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

int check_time_trigger();
int check_user_hash_trigger();
void self_destruct();

int read_payload(const char* path, unsigned char** buffer, size_t* size);

#endif
