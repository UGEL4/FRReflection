#include <iostream>
#include "FunctionTraits.h"
#include "VariableTraits.h"

bool Fool(int a)
{
    return false;
}

class Person
{
public:
    void SetAge(int age) {}
    int GetAge() { return 0; }
    int GetAgeConst() const { return 0; }

    int age;
    const int sex = 1;
};

template<typename>
struct TypeInfo;

#define BEGIN_CLASS(T) \
    template <>        \
    struct TypeInfo<T> {  \
        using type = T;

#define FUNCTIONS(...)\
    using functions = std::tuple<__VA_ARGS__>;

#define END_CLASS() };

/* BEGIN_CLASS(Person)
FUNCTIONS(function_traits<decltype(&Person::SetAge)>,
          function_traits<decltype(&Person::GetAge)>,
          function_traits<decltype(&Person::GetAgeConst)>)
END_CLASS() */

template <typename Ret, typename... Params>
auto function_point_type(Ret (*)(Params...)) -> Ret(*)(Params...);

template <typename Ret, typename Class, typename... Params>
auto function_point_type(Ret (Class::*)(Params...)) -> Ret(Class::*)(Params...);

template <typename Ret, typename Class, typename... Params>
auto function_point_type(Ret (Class::*)(Params...) const) -> Ret(Class::*)(Params...) const;

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

template <typename T>
struct field_traits : public basic_field_traits<T, is_function_v<T>> {
    constexpr field_traits(T&& pointer)
        : Pointer(pointer)
    {
    }
    T Pointer;
};

template <>
struct TypeInfo<Person>
{
    using functions = std::tuple<
        function_traits<function_point_type_t<&Person::SetAge>>,
        function_traits<function_point_type_t<&Person::GetAge>>,
        function_traits<function_point_type_t<&Person::GetAgeConst>>
    >;
};

int main()
{
    std::cout << "main" << std::endl;

   /*  using type1 = function_point_type_t<&Person::SetAge>;
    using type2 = function_point_type_t<&Person::GetAge>;
    using type3 = function_point_type_t<&Person::GetAgeConst>;
    static_assert(std::is_same_v<type1, void (Person::*)(int)>);
    static_assert(std::is_same_v<type2, int (Person::*)(void)>);
    static_assert(std::is_same_v<type3, int (Person::*)(void) const>); */

    auto field = field_traits(&Person::SetAge);
    std::cout << field.is_member << std::endl;
    std::cout << field.is_const << std::endl;
    std::cout << (&Person::SetAge == field.Pointer) << std::endl;

    auto p = &Person::sex;

    return 0;
}