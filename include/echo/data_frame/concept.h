#pragma once

#define DETAIL_NS detail_concept

#include <echo/concept.h>
#include <echo/htl.h>

namespace echo {
namespace data_frame {
namespace concept {
//------------------------------------------------------------------------------
// column_tags
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class... Tags>
auto column_tags_impl(htl::Tuple<Tags...> && ) -> std::true_type;

inline auto column_tags_impl(...) -> std::false_type;
}

template <class T>
constexpr bool column_tags() {
  using Result = decltype(DETAIL_NS::column_tags_impl(std::declval<T>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// column_tag
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class... Tags, class Tag>
auto column_tag_impl(htl::Tuple<Tags...>&&, Tag && )
    -> std::integral_constant<bool, or_c<std::is_same<Tag, Tags>::value...>()>;

inline auto column_tag_impl(...) -> std::false_type;
}

template <class Tags, class Tag>
auto column_tag() {
  using Result = decltype(
      DETAIL_NS::column_tag_impl(std::declval<Tags>(), std::declval<Tag>()));
  return Result::value;
}
}
}
}

#undef DETAIL_NS
