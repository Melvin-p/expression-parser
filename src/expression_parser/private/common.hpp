#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>

/**
 * @brief
 * attempt to dynamic cast unique_ptr if fail return unique_ptr to null
 * move
 * @tparam Derived
 * @tparam Base
 * @param input
 * @return std::unique_ptr<Derived>
 */
template <typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &&p) {
  if (Derived *result = dynamic_cast<Derived *>(p.get())) {
    p.release();
    return std::unique_ptr<Derived>(result);
  } else {
    return std::unique_ptr<Derived>(nullptr);
  }
}

#endif