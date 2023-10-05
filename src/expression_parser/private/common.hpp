#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>

template <typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &&p) {
  if (Derived *result = dynamic_cast<Derived *>(p.get())) {
    p.release();
    return std::unique_ptr<Derived>(result);
  } else {
    return std::unique_ptr<Derived>(nullptr);
  }
}

template <typename Derived, typename throw_obj, typename Base, typename... TArgs>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &&p, TArgs &&...args) {
  if (Derived *result = dynamic_cast<Derived *>(p.get())) {
    p.release();
    return std::unique_ptr<Derived>(result);
  } else {
    throw throw_obj(std::forward<TArgs>(args)...);
  }
}

#endif