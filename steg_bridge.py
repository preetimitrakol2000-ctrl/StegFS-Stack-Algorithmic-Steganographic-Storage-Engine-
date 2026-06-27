import ctypes
import os
import sys

class StegFsBridge:
    def __init__(self):
        if not os.path.exists("./libsteg.so") and not os.path.exists("./libsteg.dll"):
            print("[*] Compiling algorithmic steganographic variance engines...")
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libsteg.dll steg_heap.c bit_shifter.c")
                lib_path = "./libsteg.dll"
            else:
                os.system("gcc -shared -fPIC -o libsteg.so steg_heap.c bit_shifter.c")
                lib_path = "./libsteg.so"
        else:
            lib_path = "./libsteg.dll" if sys.platform.startswith("win") else "./libsteg.so"

        self.lib = ctypes.CDLL(lib_path)

        # Mapping native function arguments
        self.lib.encode_payload.argtypes = [
            ctypes.POINTER(ctypes.c_ubyte), 
            ctypes.POINTER(ctypes.c_int), 
            ctypes.c_int, 
            ctypes.c_char_p, 
            ctypes.c_int
        ]
        
        self.lib.decode_payload.argtypes = [
            ctypes.POINTER(ctypes.c_ubyte), 
            ctypes.POINTER(ctypes.c_int), 
            ctypes.c_int, 
            ctypes.POINTER(ctypes.c_ubyte), 
            ctypes.c_int
        ]

    def hide_bytes(self, carrier: bytes, target_indices: list, payload: str) -> bytes:
        carrier_array = (ctypes.c_ubyte * len(carrier))(*carrier)
        indices_array = (ctypes.c_int * len(target_indices))(*target_indices)
        
        self.lib.encode_payload(
            carrier_array, 
            indices_array, 
            len(target_indices), 
            payload.encode('utf-8'), 
            len(payload)
        )
        return bytes(carrier_array)

    def reveal_bytes(self, modified_carrier: bytes, target_indices: list, payload_len: int) -> str:
        carrier_array = (ctypes.c_ubyte * len(modified_carrier))(*modified_carrier)
        indices_array = (ctypes.c_int * len(target_indices))(*target_indices)
        output_buffer = (ctypes.c_ubyte * (payload_len + 1))()

        self.lib.decode_payload(
            carrier_array, 
            indices_array, 
            len(target_indices), 
            output_buffer, 
            payload_len
        )
        return ctypes.string_at(output_buffer).decode('utf-8')
