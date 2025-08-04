#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <openssl/sha.h>
#include <signal.h>

int check_time_trigger() {
    time_t now = time(NULL);
    struct tm* tm_now = localtime(&now);
    return (tm_now->tm_hour == 13 && tm_now->tm_min == 37);
}

int check_user_hash_trigger() {
    struct passwd* pw = getpwuid(getuid());
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)pw->pw_name, strlen(pw->pw_name), hash);
    return (hash[0] == 0x13 && hash[1] == 0x37);
}

void self_destruct() {
    printf("[!] Auto-eliminando proceso CPU...\n");
    kill(getpid(), SIGKILL);
}

int read_payload(const char* path, unsigned char** buffer, size_t* size) {
    FILE* f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    if (fsize <= 0 || fsize > 1024*1024) {
        fclose(f);
        return -2;
    }
    fseek(f, 0, SEEK_SET);
    *buffer = (unsigned char*)malloc(fsize);
    if (!*buffer) {
        fclose(f);
        return -3;
    }
    fread(*buffer, 1, fsize, f);
    fclose(f);
    *size = (size_t)fsize;
    return 0;
}
