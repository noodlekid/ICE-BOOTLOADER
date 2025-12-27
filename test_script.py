import struct
from crccheck.crc import Crc16

# 1. Define the Custom STM32 Hardware Configuration
#    Poly: 0x755B (Koopman), Init: 0xFFFF, No Reflection
class Crc16Koopman(Crc16):
    _width = 16
    _poly = 0x755B
    _initvalue = 0xFFFF
    _reflect_input = False
    _reflect_output = False
    _xor_output = 0x0000

def create_packet(payload_bytes):
    # 2. Calculate CRC on the Payload ONLY
    #    (The FSM calculates CRC on the buffer content, which is the payload)
    crc_inst = Crc16Koopman()
    crc_inst.process(payload_bytes)
    crc_val = crc_inst.final()
    
    # 3. Build Frame: [SOF] [LEN] [PAYLOAD] [CRC_LSB] [CRC_MSB]
    #    < = Little Endian
    #    B = Uint8 (SOF)
    #    B = Uint8 (LEN)
    #    s = Byte String (Payload)
    #    H = Uint16 (CRC)
    sof = 0xAA
    length = len(payload_bytes)
    
    fmt = f'<BB{length}sH'
    packet = struct.pack(fmt, sof, length, payload_bytes, crc_val)
    
    return packet, crc_val

# --- TEST ---
payload = b'\xDD' # The command byte your C code expects
packet, crc = create_packet(payload)

print(f"Payload:  0x{payload.hex().upper()}")
print(f"CRC-16:   0x{crc:04X}")
print(f"Packet:   {packet.hex(' ').upper()}")