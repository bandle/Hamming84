// Maximilian Bandle 2020
#pragma once

#include <stdint.h>

class Hamming84 {

   // Store 16 possible values for fast encoding
   static constexpr uint8_t encodeTable[16] = {0, 0b10000111, 0b10011001, 0b11110, 0b10101010, 0b101101, 0b110011,
                                            0b10110100, 0b1001011, 0b11001100, 0b11010010, 0b1010101, 0b11100001,
                                            0b1100110, 0b1111000, 0b11111111};

   // check if codeword has an error
   static uint8_t calcSyndrome(uint8_t byte) {
      const uint8_t syndromeCheck[3] = {
              0b1111000,
              0b1100110,
              0b1010101
      };

      // Check individual parity and find error
      uint8_t syndrome = 0;
      for (int y = 0; y < 3; ++y) {
         syndrome = (syndrome << 1) | __builtin_parity(syndromeCheck[y] & byte);
      }
      return syndrome;
   }

   // Helper to decodeHelper an encoded uint8_t into an nibble
   static uint8_t decodeHelper(uint8_t byte) {
      // Check parity first to find single bit flips
      if(__builtin_parity(byte)) {
         // A bit has to be flipped
         uint8_t e = calcSyndrome(byte);
         // Is it in the code word?
         if (e != 0) {
            // Toggle the byte
            byte ^= 1u << (e - 1);
         }
         // We don't correct a flipped parity
      }
      // Check if code word is correct
      if (calcSyndrome(byte) != 0) {
         return 0x80;
      }
      // Read the byte bits
      return ((byte >> 2) & 0x01) | (0xE & (byte >> 3));
   }

public:
   // Our internal buffer is twice the size
   template<typename T>
   using ParamStore = uint8_t[sizeof(T) << 1];

   // Encode with Hamming 7,4 code + parity
   template<typename T>
   static void encode(const T params, ParamStore<T> data) {
      const uint8_t *pRaw = reinterpret_cast<const uint8_t *>(&params);
      for (unsigned i = 0; i < sizeof(T); ++i) {
         // Use lookup table for encoding
         data[2 * i] = encodeTable[pRaw[i] & 15];
         data[2 * i + 1] = encodeTable[pRaw[i] >> 4];
      }
   }

   // Check Parity Decode with Hamming 7,4 code
   template<typename T>
   static bool decode(T &params, ParamStore<T> data) {
      uint8_t *pRaw = reinterpret_cast<uint8_t *>(&params);
      for (unsigned i = 0; i < sizeof(T); ++i) {
         uint8_t nibble1 = decodeHelper(data[2 * i]);
         uint8_t nibble2 = decodeHelper(data[2 * i + 1]);
         // Check for error
         if ((nibble1 | nibble2) & 0x80) {
            return false;
         }
         // Combine into uint8_t
         pRaw[i] = nibble1 | (nibble2 << 4);
      }
      return true;

   }
};