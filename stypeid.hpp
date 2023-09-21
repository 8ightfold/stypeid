#ifndef STYPEID_RTTI_HPP
#define STYPEID_RTTI_HPP

#include <string>
#include <string_view>
#include <typeinfo>

#if defined(__has_include)
#  if __has_include(<cxxabi.h>)
#    include <cstddef>
#    include <cstdlib>
#    include <cxxabi.h>
#    include <fmt/core.h>
#    define RTTI_REQUIRES_DEMANGLING 1
#  endif // __has_include(<cxxabi.h>)
#endif // defined(__has_include)

#if !defined(RTTI_REQUIRES_DEMANGLING)
#  define RTTI_REQUIRES_DEMANGLING 0
#endif // !defined(RTTI_REQUIRES_DEMANGLING)


//=== Implementation details ===//
namespace rtti::detail_ {
#  if RTTI_REQUIRES_DEMANGLING
    namespace {
        std::string handle__cxa_demangle_result(
          std::string_view mangled, char* data, std::size_t len, int status) {
            std::string result;
            switch (status) {
                case 0: {
                    result = std::string(data, len);
                    break;
                }
                case -2: {
                    result = fmt::format(
                        "`invalid mangled name {}`", mangled);
                    break;
                }
                default: {
                    result = "`__cxa_demangle failure`";
                }
            };

            std::free(data);
            return result;
        }

        std::string demangle(std::string_view mangled) {
            std::size_t len;
            int status;
            char* raw_str = abi::__cxa_demangle(
                mangled.data(), nullptr, &len, &status);
            return handle__cxa_demangle_result(
                mangled, raw_str, len, status);
        }
    } // namespace `anonymous`
#  else
    namespace {
        std::string demangle(std::string_view mangled) {
            return { mangled };
        }
    } // namespace `anonymous`
#  endif // RTTI_REQUIRES_DEMANGLING
} // namespace rtti::detail_


//=== Exposed interface ===//
namespace rtti {
    /**
     * Takes an implementation defined mangled `std::string_view`,
     * returns a (potentially) demangled `std::string`.
     */
    std::string demangle(std::string_view mangled) noexcept {
        return detail_::demangle(mangled);
    }

    /**
     * Takes a value and returns a demangled type name.
     */
    std::string name(auto& v) noexcept {
        std::string_view mangled { typeid(v).name() };
        return demangle(mangled);
    }

    /**
     * Takes a type and returns a demangled type name.
     */
    template <typename T>
    std::string name() noexcept {
        std::string_view mangled { typeid(T).name() };
        return demangle(mangled);
    }
} // namespace rtti

#endif // STYPEID_RTTI_HPP
