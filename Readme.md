# Hamming84
## Hamming 7,4 (+Parity) Decoder & Encoder

Hamming84 is a header only C++ Library for Hamming Error correction.

## Usage
### Encoding
```c++
#include "Hamming84.h"
// Payload data
struct myData = {...};
myData in = {};

// Reserve Buffer Space
Hamming84::ParamStore<myData> buffer;
// Encode to buffer
Hamming84::encode(in, buffer);
// Do sth with buffer
```
### Decoding
```c++
#include "Hamming84.h"
// Payload data
struct myData = {...};

// Input data
Hamming84::ParamStore<myData> buffer;
buffer = // put data here
        
// Prepare struct for data
myData out;
// Encode to buffer
Hamming84::decode(out, buffer);
// Do sth with out
```

## Test
You find an end-to-end test with random introduced errors in test.cpp