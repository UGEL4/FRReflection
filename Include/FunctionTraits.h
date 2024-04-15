#include <tuple>

namespace detail
{
template <typename T>
struct basic_function_traits;

template <typename Ret, typename... Params>
struct basic_function_traits<Ret(Params...)> {
    using RetType   = Ret;
    using ParamList = std::tuple<Params...>;
};
} // namespace detail

template <typename T>
struct function_traits;

template <typename Ret, typename... Params>
struct function_traits<Ret(Params...)> : public detail::basic_function_traits<Ret(Params...)> {
    using Type                      = Ret(Params...);
    using ParamListWithClass        = std::tuple<Params...>;
    using Pointer                   = Ret (*)(Params...);
    static constexpr bool is_member = false;
    static constexpr bool is_const  = false;
};

template <typename Ret, typename Class, typename... Params>
struct function_traits<Ret (Class::*)(Params...)> : public detail::basic_function_traits<Ret(Params...)> {
    using Type                      = Ret (Class::*)(Params...);
    using ParamListWithClass        = std::tuple<Class*, Params...>;
    using Pointer                   = Ret (*)(Params...);
    static constexpr bool is_member = true;
    static constexpr bool is_const  = false;
};

template <typename Ret, typename Class, typename... Params>
struct function_traits<Ret (Class::*)(Params...) const> : public detail::basic_function_traits<Ret(Params...)> {
    using Type                      = Ret (Class::*)(Params...) const;
    using ParamListWithClass        = std::tuple<Class*, Params...>;
    using Pointer                   = Ret (Class::*)(Params...) const;
    static constexpr bool is_member = true;
    static constexpr bool is_const  = true;
};
