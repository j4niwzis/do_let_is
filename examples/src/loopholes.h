#ifndef LOOPHOLES_H
#define LOOPHOLES_H

#include <cstddef>
#include <tuple>
#include <utility>

namespace stdext::loopholes {

template <typename Key>
struct adl_tag {
  friend consteval auto get(adl_tag);
};

template <typename Key, typename Val>
struct injector {
  friend consteval auto get(adl_tag<Key>) {
    return [] -> Val { std::unreachable(); }();
  }
};

template <typename TagT, std::size_t Idx>
struct set_tag {};

template <typename TagT, typename Unique, std::size_t CurIdx = 0>
consteval std::size_t find_first_free() {
  if constexpr (requires {
                  get(adl_tag<set_tag<TagT, CurIdx>>{});
                  typename Unique /* fixes ifndr (i hope) */;
                }) {
    return find_first_free<TagT, Unique, CurIdx + 1>();
  } else {
    return CurIdx;
  }
}

template <typename TagT, typename Val>
consteval auto add_to_set() {
  constexpr std::size_t idx = find_first_free<TagT, Val>();
  std::ignore = injector<set_tag<TagT, idx>, Val>{};
  return idx;
}

template <typename... Ts>
struct type_list {};

template <typename Tag, typename Unique = Tag>
consteval auto read_set() {
  constexpr std::size_t have_cnt = find_first_free<Tag, Tag>();
  return []<std::size_t... Idxs>(std::index_sequence<Idxs...>) {
    return type_list<decltype(get(adl_tag<set_tag<Tag, Idxs>>{}))...>{};
  }(std::make_index_sequence<have_cnt>{});
}

}  // namespace stdext::loopholes

#endif
