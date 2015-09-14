#pragma once

#define DETAIL_NS detail_get_column_index

#include <echo/data_frame/concept.h>
#include <echo/data_frame/column_tags.h>
#include <echo/htl.h>

namespace echo {
namespace data_frame {
//------------------------------------------------------------------------------
// TagMatcher
//------------------------------------------------------------------------------
namespace DETAIL_NS {
// Functor class to work around bug with lambda expressions in intel compiler
template <class Tag1>
struct TagMatcher {
  template <class Tag2>
  auto operator()(Tag2) const
      -> htl::integral_constant<bool, std::is_same<Tag1, Tag2>::value> {
    return {};
  }
};

template <class Tag, std::size_t I>
struct TagMatcher<IndexedTag<Tag, I>> {
  template <class Tag2>
  auto operator()(Tag2) const -> htl::integral_constant<bool, false> {
    return {};
  }

  template <std::size_t N>
  auto operator()(TagGroup<Tag, N>) const
      -> htl::integral_constant<bool, true> {
    return {};
  }
};
}
//------------------------------------------------------------------------------
// get_tag_group_size
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Tag>
constexpr index_t get_tag_group_size(Tag) {
  return 1;
}

template <class Tag, std::size_t N>
constexpr index_t get_tag_group_size(TagGroup<Tag, N>) {
  return N;
}
}

//------------------------------------------------------------------------------
// get_tag_index
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Tag>
constexpr index_t get_tag_index(Tag) {
  return 0;
}

template <class Tag, std::size_t I>
constexpr index_t get_tag_index(IndexedTag<Tag, I>) {
  return I;
}
}

//------------------------------------------------------------------------------
// get_column_index
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class TagGroupFirst, class... TagGroupsRest, class Tag,
          CONCEPT_REQUIRES(concept::subtag<TagGroupFirst, Tag>())>
constexpr index_t get_column_index_impl(
    htl::Tuple<TagGroupFirst, TagGroupsRest...>, Tag tag) {
  return get_tag_index(tag);
}

template <class TagGroupFirst, class... TagGroupsRest, class Tag,
          CONCEPT_REQUIRES(!concept::subtag<TagGroupFirst, Tag>())>
constexpr index_t get_column_index_impl(
    htl::Tuple<TagGroupFirst, TagGroupsRest...>, Tag tag) {
  return get_tag_group_size(TagGroupFirst()) +
         get_column_index_impl(htl::Tuple<TagGroupsRest...>(), tag);
}
}

template <class ColumnTags, class ColumnTag,
          CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
constexpr index_t get_column_index() {
  return DETAIL_NS::get_column_index_impl(ColumnTags(), ColumnTag());
}
}
}

#undef DETAIL_NS
