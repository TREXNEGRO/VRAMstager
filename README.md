# VRAMstager-CUDA

PoC de malware híbrido que usa CUDA para cargar y desencriptar payloads en VRAM con triggers locales para ejecución condicional.

---

## Requisitos

- NVIDIA GPU con soporte CUDA
- Drivers NVIDIA actualizados
- CUDA Toolkit instalado para compilación
- Payload `.bin` XOR-encriptado (clave 0x55)

---

## Uso

1. Prepara el payload XOR-encriptado (`payload_xored.bin`).  
2. Compila con `./build.sh`.  
3. Ejecuta:  
   ```bash
   ./gpustic_cuda payload_xored.bin
   ```
4. El payload se ejecutará solo si se cumple algún trigger (variable de entorno `GPUSTIC_TRIGGER`, hora 13:37 o hash de usuario).

---

## Advertencias

- No es persistencia permanente (VRAM se pierde en reinicios).  
- El payload solo se guarda temporalmente en `/tmp/` antes de ejecutar.  

---
