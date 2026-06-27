#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "steg_fs.h"

#ifdef _WIN32
    __declspec(dllexport) void encode_payload(unsigned char* carrier, int* pixel_indices, int indices_count, unsigned char* payload, int payload_len);
    __declspec(dllexport) void decode_payload(unsigned char* carrier, int* pixel_indices, int indices_count, unsigned char* output, int payload_len);
#endif

// Embeds bits directly into the carrier buffer arrays using bit masking
void encode_payload(unsigned char* carrier, int* pixel_indices, int indices_count, unsigned char* payload, int payload_len) {
    int bit_idx = 0;
    
    for (int i = 0; i < payload_len; i++) {
        unsigned char current_byte = payload[i];
        
        for (int bit = 7; bit >= 0; bit--) {
            if (bit_idx >= indices_count) return;

            int target_pixel = pixel_indices[bit_idx];
            // Extract single bit out of data payload stream
            unsigned char bit_to_hide = (current_byte >> bit) & 1;

            // Mask out and replace the least significant bit of the carrier channel
            carrier[target_pixel] = (carrier[target_pixel] & 0xFE) | bit_to_hide;
            bit_idx++;
        }
    }
}

// Reassembles scattered bit segments back into structured text bytes
void decode_payload(unsigned char* carrier, int* pixel_indices, int indices_count, unsigned char* output, int payload_len) {
    int bit_idx = 0;
    
    for (int i = 0; i < payload_len; i++) {
        unsigned char reconstructed_byte = 0;
        
        for (int bit = 7; bit >= 0; bit--) {
            if (bit_idx >= indices_count) return;

            int target_pixel = pixel_indices[bit_idx];
            // Extract the hidden bit from the carrier channel
            unsigned char hidden_bit = carrier[target_pixel] & 1;

            // Shift and collect bits sequentially into a single byte frame
            reconstructed_byte = (reconstructed_byte << 1) | hidden_bit;
            bit_idx++;
        }
        output[i] = reconstructed_byte;
    }
    output[payload_len] = '\0';
}
