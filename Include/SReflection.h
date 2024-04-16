#include "VariableTraits.h"
#include "FunctionTraits.h"
#include <string_view>

namespace detail
{
template <typename Ret, typename... Params>
auto function_point_type(Ret (*)(Params...)) -> Ret (*)(Params...);

template <typename Ret, typename Class, typename... Params>
auto function_point_type(Ret (Class::*)(Params...)) -> Ret (Class::*)(Params...);

template <typename Ret, typename Class, typename... Params>
auto function_point_type(Ret (Class::*)(Params...) const) -> Ret (Class::*)(Params...) const;

template <auto F>
using function_point_type_t = decltype(function_point_type(F));

template <typename T, bool is_function>
struct basic_field_traits;

template <typename T>
struct basic_field_traits<T, true> : public function_traits<T>
{
    constexpr bool IsMember() const
    {
        return function_traits<T>::is_member;
    }

    constexpr bool IsConst() const
    {
        return function_traits<T>::is_const;
    }

    constexpr bool IsFunction() const
    {
        return true;
    }

    constexpr bool IsVariable() const
    {
        return false;
    }
};

template <typename T>
struct basic_field_traits<T, false> : public variable_traits<T>
{
    constexpr bool IsMember() const
    {
        return variable_traits<T>::is_member;
    }

    constexpr bool IsConst() const
    {
        return variable_traits<T>::is_const;
    }

    constexpr bool IsFunction() const
    {
        return false;
    }

    constexpr bool IsVariable() const
    {
        return true;
    }
};

template <typename T>
struct is_function {
    static constexpr bool value = std::is_function_v<std::remove_pointer_t<T>> || std::is_member_function_pointer_v<T>;
};

template<typename T>
constexpr bool is_function_v = is_function<T>::value;
} // namespace detail

namespace fr_reflection
{
template <typename T>
struct field_traits : public detail::basic_field_traits<T, detail::is_function_v<T>> {
    constexpr field_traits(T&& pointer, std::string_view name)
        : Pointer(pointer)
        , name(name.substr(name.find_last_of(":") + 1))
    {
    }
    T Pointer;
    std::string_view name;
};

template <typename>
struct TypeInfo;
} // namespace fr_reflection

#define BEGIN_CLASS(T) \
    template <>        \
    struct fr_reflection::TypeInfo<T> {

#define FUNCTIONS(...)\
    static constexpr auto functions = std::make_tuple(__VA_ARGS__);

#define FUNC(F) \
    fr_reflection::field_traits { F, #F }

#define END_CLASS() };
