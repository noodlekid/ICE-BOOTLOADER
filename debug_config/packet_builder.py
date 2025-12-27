import struct
from crccheck.crc import Crc16

class Crc16Koopman(Crc16):
    _width = 16
    _poly = 0x755B
    _initvalue = 0xFFFF
    _reflect_input = False
    _reflect_output = False
    _xor_output = 0x0000

def create_packet(payload_bytes):
    crc_inst = Crc16Koopman()
    crc_inst.process(payload_bytes)
    crc_val = crc_inst.final()
    
    # Build Frame: [SOF] [LEN] [PAYLOAD] [CRC_LSB] [CRC_MSB]
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

payload = b'\xDD'
packet, crc = create_packet(payload)

print(f"Payload:  0x{payload.hex().upper()}")
print(f"CRC-16:   0x{crc:04X}")
print(f"Packet:   {packet.hex(' ').upper()}")