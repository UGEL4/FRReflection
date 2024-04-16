#include <iostream>
#include "SReflection.h"

bool Fool(int a)
{
    return false;
}

class Person
{
public:
    void SetAge(int age) {
        std::cout << "call SetAge:" << std::endl;
        this->age = age;
    }
    int GetAge() { 
        std::cout << "call GetAge:" << std::endl;
        return age; 
    }
    int GetAgeConst() const {
        std::cout << "call GetAgeConst:" << std::endl;
        return age; 
    }

    int age = 0;
    const int sex = 1;
};

BEGIN_CLASS(Person)
FUNCTIONS(
    FUNC(&Person::SetAge),
    FUNC(&Person::GetAge),
    FUNC(&Person::GetAgeConst)
)
END_CLASS()

int main()
{
    std::cout << "main" << std::endl;

    constexpr auto type_info = fr_reflection::TypeInfo<Person>{};
    std::get<0>(type_info.functions).is_member;
    std::get<2>(type_info.functions).is_const;
    std::cout << std::get<2>(type_info.functions).name << std::endl;

    Person p;
    (p.*(std::get<2>(type_info.functions).Pointer))();
    (p.*(std::get<0>(type_info.functions).Pointer))(10);
    std::cout << (p.*(std::get<1>(type_info.functions).Pointer))() << std::endl;
    std::cout << (p.*(std::get<2>(type_info.functions).Pointer))() << std::endl;
    return 0;
}