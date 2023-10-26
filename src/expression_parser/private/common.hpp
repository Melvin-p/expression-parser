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

template <typename Derived, typename Base>
bool dynamic_unique_ptr_cast_check(const std::unique_ptr<Base> &p) {
  if (Derived *result = dynamic_cast<Derived *>(p.get())) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief
 * attempt to dynamic cast unique_ptr if fail throw throw_obj
 * @tparam Derived
 * @tparam throw_obj
 * @tparam Base
 * @tparam TArgs
 * @param p input
 * @param args throwable object arguments
 * @return std::unique_ptr<Derived>
 */
template <typename Derived, typename throwObj, typename Base, typename... TArgs>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &&p, TArgs &&...args) {
  if (Derived *result = dynamic_cast<Derived *>(p.get())) {
    p.release();
    return std::unique_ptr<Derived>(result);
  } else {
    throw throwObj(std::forward<TArgs>(args)...);
  }
}

#endif