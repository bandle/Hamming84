#include <iostream>
#include "Hamming84.h"
using namespace std;

// Return false if errors where not detected and wrong result returned
bool test(int errors = 0) {
   Hamming84::ParamStore<uint8_t> buffer;
   bool correctOrDetected = true;
   for(int i = 0;i <= 255; ++i) {
      // Encode
      uint8_t in = i;
      Hamming84::encode(in, buffer);
      uint8_t out = 0;
      if (errors) {
         int pos = i;
         int e = errors;
         do {
            int error = 0x01 << (pos % 8);
            buffer[pos % 2] ^= error;
            pos += 3;
         } while(e--);
      }
      // Introduce a bit error
      if(!Hamming84::decode(out, buffer)) {
         cout << i << ": decodeHelper failed" << endl;
         continue;
      }
      if(out != in) {
         cout << i << ": wrong result" << endl;
         correctOrDetected = false;
      }
   }
   return correctOrDetected;
}
int main() {
   cout << "Testing w/o bit error" << endl;
   if(!test()) {
      return 1;
   }
   cout << "Testing w single bit error" << endl;
   if(!test(1)) {
      return 1;
   }
   cout << "Testing w double bit error" << endl;
   if(!test(2)) {
      return 1;
   }
   cout << "Double Bit Errors Detected but not corrected" << endl;
   cout << "Testing w triple bit error" << endl;
   if(!test(3)) {
      return 1;
   }
   cout << "Triple Bit Errors Detected but not corrected" << endl;
   cout << "Everything works as expected" << endl;
   return 0;
}
