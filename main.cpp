#include <concepts>
#include <string>
#include <iostream>

template <typename T>
concept ComplexConcept = requires(const T v) {
    {
        v.hash()
    } -> std::convertible_to<long>;
    {
        v.toString()
    } -> std::same_as<std::string>;
} && !std::has_virtual_destructor<T>::value;

template <ComplexConcept T>
void ConceptVerification(T val)
{
    std::cout << "Method hash(): " << val.hash() << std::endl;
    std::cout << "Method toString(): " << val.toString() << std::endl;
    std::cout << "Presence of virtual destructor: "
              << std::has_virtual_destructor<T>::value << std::endl;
}

// Соответствует концепту.
struct Type1
{
    double val_ = 0;
    Type1(auto const val) : val_(val){};
    ~Type1() = default;
    auto hash() const
    {
        return static_cast<long>(val_);
    }
    auto toString() const
    {
        return std::to_string(val_);
    }
};

// Не соответствует концепту. Отсутствует метод hash().
struct Type2
{
    double val_ = 0;
    Type2(auto const val) : val_(val){};
    ~Type2() = default;
    auto toString() const
    {
        return std::to_string(val_);
    }
};

// Не соответствует концепту. Метод toString() возвращает double.
struct Type3
{
    double val_ = 0;
    Type3(auto const val) : val_(val){};
    ~Type3() = default;
    auto hash() const
    {
        return static_cast<long>(val_);
    }
    auto toString() const
    {
        return val_;
    }
};

// Не соответствует концепту. Наличие виртуального деструктора.
struct Type4
{
    double val_ = 0;
    Type4(auto const val) : val_(val){};
    virtual ~Type4() = default;
    auto hash() const
    {
        return static_cast<long>(val_);
    }
    auto toString() const
    {
        return std::to_string(val_);
    }
};

int main()
{
    Type1 t1(111.111);
    ConceptVerification<Type1>(t1); // соответствует концепту

    Type2 t2(222.222);
    // ConceptVerification<Type2>(t2);  // Отсутствует метод hash().

    Type3 t3("333.333");
    // ConceptVerification<Type3>(t3);  // toString() возвращает double

    Type4 t4(444.444);
    // ConceptVerification<Type4>(t4);  // Наличие виртуального деструктора
    return 0;
}