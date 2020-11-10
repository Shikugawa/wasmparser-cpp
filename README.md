# wasmparser-cpp
Fast, Simple, Independent, C++ Header-only WebAssembly parser.
It supports higher than C++17.

## Example

```c++
#include "wasmparser/parser.h"

int main() {
    auto mod = wasmparser::Parser::doParse("../testdata/fibonacci.wasm");
    return 0;
}
```