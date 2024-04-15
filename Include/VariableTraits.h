#include <type_traits>
namespace detail
{
template <typename T>
struct variable_type {
    using Type = T;
};

template <typename T, typename Class>
struct variable_type<T Class::*> {
    using Type = T;
};
} // namespace detail

template<typename T>
using variable_type_t = typename detail::variable_type<T>::Type;

namespace internal
{
template <typename T>
struct basic_variable_traits {
    using Type                      = variable_type_t<T>;
    static constexpr bool is_member = std::is_member_pointer_v<T>;
};
} // namespace internal

template <typename>
struct variable_traits;

template <typename T>
struct variable_traits<T*> : public internal::basic_variable_traits<T>
{
    using PointerType = T*;  
};

template <typename T, typename Class>
struct variable_traits<T Class::*> : public internal::basic_variable_traits<T Class::*>
{
    using PointerType = T Class::*;
    using ClassType   = Class;
};
