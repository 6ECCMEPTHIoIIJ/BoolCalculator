#ifndef BOOL_CALCULATOR_CONSTITUENT_HPP
#define BOOL_CALCULATOR_CONSTITUENT_HPP

#include <iostream>

namespace BoolCalc {

  class Constituent {
    uint32_t value_; // младшие биты - включение или исключение множества, старшие - игнорирование множества
    uint8_t size_; // кол-во значащих битов
    bool is_in_use_ = false; // участвовала ли конституанта в склеивании
  public:
    Constituent();

    // Конструктор для создания конституанты по таблице истинности
    // value - номер строки в таблице истинности (значения битов 0 и 1 соответствуют обратному и прямому включению
    // множества в конституанту)
    // size - общее кол-во множеств
    Constituent(uint32_t value,
                uint8_t size);

    // Конструктор для получения конституанты путем склеивания конституант c_1 и c_2
    Constituent(Constituent& c_1,
                Constituent& c_2);

    ~Constituent();

    [[nodiscard]] uint32_t value() const;

    [[nodiscard]] uint8_t size() const;

    [[nodiscard]] bool is_in_use() const;

    [[nodiscard]] bool canBeGluedWith(const Constituent& c) const;

    [[nodiscard]] bool isCoatingBy(const Constituent& c) const;

    [[nodiscard]] bool isEqualTo(const Constituent& c) const;

    friend std::ostream& operator<<(std::ostream& out,
                                    const Constituent& c);
  };
}

#endif // BOOL_CALCULATOR_CONSTITUENT_HPP
