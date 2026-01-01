import struct
from crccheck.crc import Crc16

def cobs_encode(data: bytes) -> bytes:
    """
    Encodes bytes using Consistent Overhead Byte Stuffing (COBS).
    Matches the logic in ice_cobs.c
    """
    output = bytearray()
    output.append(0) # Placeholder for the first overhead byte
    code_ptr = 0
    code = 0x01

    for byte in data:
        if byte != 0:
            output.append(byte)
            code += 1
            
            if code == 0xFF:
                output[code_ptr] = code
                code = 0x01
                code_ptr = len(output)
                output.append(0)
        else:
            output[code_ptr] = code
            code = 0x01
            code_ptr = len(output)
            output.append(0)

    output[code_ptr] = code
    return bytes(output)

class Crc16Koopman(Crc16):
    _width = 16
    _poly = 0x755B
    _initvalue = 0xFFFF
    _reflect_input = False
    _reflect_output = False
    _xor_output = 0x0000

def create_full_packet(payload_bytes):
    op = 0x00
    seq = 0x42 
    flags = 0x00
    padding = 0x00
    arg = 0x08000000
    length = len(payload_bytes)

    header_fmt = '<BBBBII' 
    header = struct.pack(header_fmt, op, seq, flags, padding, arg, length)
    
    raw_pdu = header + payload_bytes

    crc_inst = Crc16Koopman()
    crc_inst.process(raw_pdu)
    crc_val = crc_inst.final()

    # raw_pdu_with_crc = raw_pdu i+ struct.pack('<H', crc_val)

    cobs_frame = cobs_encode(raw_pdu)

    final_wire_data = cobs_frame + b'\x00'

    return final_wire_data, crc_val

if __name__ == "__main__":
    payload = b'Hello'
    
    packet, crc = create_full_packet(payload)

    print(f"Payload:  {payload}")
    print(f"CRC-16:   0x{crc:04X}")
    print(f"Wire Hex: {packet.hex(' ').upper()}")
    
    if b'\x00' in packet[:-1]:
        print("ERROR: Zero byte found inside COBS frame!")
    else:
        print("SUCCESS: Valid COBS frame.")