# Stypeid : A slim, header-only demangled typeid for GCC, Clang, ICC and MSVC

Have you ever tried to get the name of a type, and instead of a nice, readable name, you got mangled garbage?
``stypeid`` has you covered! To use, just add the header to your project, and get going!

## Example usage

```
using i = int;
float f = 3.0f;

std::cout << stypeid<i>() << "\n";
std::cout << rtti::name(f) << "\n";
```

## Notes

``stypeid`` will likely not work for versions of languages where the there is no official demangling method provided by ``cxxabi.h`` (such as TS's).
It relies on ``cxxabi.h`` to exist for languages where ``typeid`` does not provide demangled type names automatically, 
and will therefore not work on compilers which do not include this header, and do not demangle type names.
