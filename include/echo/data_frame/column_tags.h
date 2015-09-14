#pragma once

#define DETAIL_NS detail_column_tags

#include <echo/htl.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// TagGroup
//------------------------------------------------------------------------------
template <class T, std::size_t N>
struct TagGroup {};

//------------------------------------------------------------------------------
// IndexedTag
//------------------------------------------------------------------------------
template <class T, std::size_t I>
struct IndexedTag {};

//------------------------------------------------------------------------------
// indexed_tag
//------------------------------------------------------------------------------
template <std::size_t I, class T>
IndexedTag<T, I> indexed_tag(T) {
  return IndexedTag<T, I>();
}

//------------------------------------------------------------------------------
// ColumnTags
//------------------------------------------------------------------------------
template <class... Tags>
using ColumnTags = htl::Tuple<Tags...>;

//------------------------------------------------------------------------------
// get_column_group_size
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class T>
struct column_group_size {
  static constexpr std::size_t value = 1;
};

template <class T, std::size_t N>
struct column_group_size<TagGroup<T, N>> {
  static constexpr std::size_t value = N;
};
}

//------------------------------------------------------------------------------
// get_num_columns
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class T>
struct num_columns {};

template <>
struct num_columns<htl::Tuple<>> {
  static constexpr std::size_t value = 0;
};

template <class TFirst, class... TRest>
struct num_columns<htl::Tuple<TFirst, TRest...>> {
  static constexpr std::size_t value = column_group_size<TFirst>::value +
                                       num_columns<htl::Tuple<TRest...>>::value;
};
}

template <class T>
constexpr auto get_num_columns() -> decltype(DETAIL_NS::num_columns<T>::value) {
  return DETAIL_NS::num_columns<T>::value;
}
}
}

#undef DETAIL_NS
