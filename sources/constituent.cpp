#include "constituent.hpp"

namespace BoolCalc {

  Constituent::Constituent() = default;

  Constituent::Constituent(const uint32_t value,
                           const uint8_t size) {
    value_ = value;
    size_ = size;
  }

  Constituent::Constituent(Constituent& c_1,
                           Constituent& c_2) {
    c_1.is_in_use_ = true;
    c_2.is_in_use_ = true;
    size_ = c_1.size_;
    value_ = c_1.value_ | (c_1.value_ ^ c_2.value_) << size_;
  }

  Constituent::~Constituent() = default;

  uint32_t Constituent::value() const {
    return value_;
  }

  uint8_t Constituent::size() const {
    return size_;
  }

  bool Constituent::is_in_use() const {
    return is_in_use_;
  }

  bool Constituent::canBeGluedWith(const Constituent& c) const {
    const uint16_t mask = (1 << size_) - 1;
    const uint32_t t = value_ ^ c.value_;
    const uint16_t significant_bits_diff = t & mask;
    const uint16_t glued_bits_diff = t >> size_ & mask;

    return !glued_bits_diff &&
           significant_bits_diff && !(significant_bits_diff & (significant_bits_diff - 1));
  }

  bool Constituent::isCoatingBy(const Constituent& c) const {
    const uint16_t mask = (1 << size_) - 1;
    return !((c.value_ ^ value_) & ~(c.value_ >> c.size_) & mask);
  }

  bool Constituent::isEqualTo(const Constituent& c) const {
    return value_ == c.value_;
  }
}