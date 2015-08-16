#pragma once

#define DETAIL_NS detail_concept

#include <echo/data_frame/homogenous_data_frame_fwd.h>
#include <echo/data_frame/homogenous_data_frame_view_fwd.h>
#include <echo/data_frame/homogenous_data_frame_row_fwd.h>
#include <echo/k_array.h>
#include <echo/execution_context.h>

namespace echo {
namespace data_frame {
namespace concept {
//------------------------------------------------------------------------------
// column_tags
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Tag>
constexpr int count_matches() {
  return 0;
}

template <class Tag, class TagFirst, class... TagsRest>
constexpr int count_matches() {
  return std::is_same<Tag, TagFirst>::value + count_matches<Tag, TagsRest...>();
}

template <class... Tags>
auto column_tags_impl(htl::Tuple<Tags...> && ) -> htl::integral_constant<
    bool, and_c<(count_matches<Tags, Tags...>() == 1)...>()>;

template <class T>
auto column_tags_impl(T && ) -> std::false_type;
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
auto column_tag_impl(htl::Tuple<Tags...>&&, Tag && ) -> std::integral_constant<
    bool, concept::column_tags<htl::Tuple<Tags...>>() &&
              or_c<std::is_same<Tag, Tags>::value...>()>;

inline auto column_tag_impl(...) -> std::false_type;
}

template <class Tags, class Tag>
constexpr bool column_tag() {
  using Result = decltype(
      DETAIL_NS::column_tag_impl(std::declval<Tags>(), std::declval<Tag>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// homogenous_data_frame_shape
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class... Tags, class Shape>
auto match_column_extents(htl::Tuple<Tags...>&&, Shape&& shape)
    -> std::integral_constant<
        bool, std::is_same<StaticIndex<sizeof...(Tags)>,
                           decltype(get_extent<1>(shape))>::value>;

struct HomogenousDataFrameShape : Concept {
  template <class ColumnTags, class Shape>
  auto require(ColumnTags&&, Shape&& shape) -> list<
      concept::column_tags<ColumnTags>(), k_array::concept::shape_<2, Shape>(),
      static_cast<bool>(decltype(match_column_extents(
          std::declval<ColumnTags>(), std::declval<Shape>())){})>;
};
}

template <class ColumnTags, class Shape>
constexpr bool homogenous_data_frame_shape() {
  return models<DETAIL_NS::HomogenousDataFrameShape, ColumnTags, Shape>();
}

//------------------------------------------------------------------------------
// homogenous_data_frame_deep
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class T, class ColumnTags, class Allocator>
auto homogenous_data_frame_deep(
    HomogenousDataFrame<T, ColumnTags, Allocator> && ) -> std::true_type;

template <class T>
auto homogenous_data_frame_deep(T && ) -> std::false_type;
}

template <class T>
constexpr bool homogenous_data_frame_deep() {
  using Result =
      decltype(DETAIL_NS::homogenous_data_frame_deep(std::declval<T>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// homogenous_data_frame_view
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Pointer, class Shape, class ColumnTags, class MemoryBackendTag>
auto homogenous_data_frame_view_impl(
    HomogenousDataFrameView<Pointer, Shape, ColumnTags, MemoryBackendTag> && )
    -> std::true_type;

template <class T>
auto homogenous_data_frame_view_impl(T && ) -> std::false_type;
}

template <class T>
constexpr bool homogenous_data_frame_view() {
  using Result =
      decltype(DETAIL_NS::homogenous_data_frame_view_impl(std::declval<T>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// homogenous_data_frame
//------------------------------------------------------------------------------
template <class T>
constexpr bool homogenous_data_frame() {
  return homogenous_data_frame_deep<T>() || homogenous_data_frame_view<T>();
}

namespace DETAIL_NS {
template <int K>
struct KHomogenousDataFrame : Concept {
  template <class T>
  auto require(T&& x)
      -> list<homogenous_data_frame<T>(),
              same<decltype(get_num_columns(x)), StaticIndex<K>>()>;
};
}

template <int K, class T>
constexpr bool homogenous_data_frame_() {
  return models<DETAIL_NS::KHomogenousDataFrame<K>, T>();
}

//------------------------------------------------------------------------------
// modifiable_homogenous_data_frame_forward
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ModifiableHomogenousDataFrameForward : Concept {
  template <class T>
  auto require(T&& x) -> list<homogenous_data_frame<uncvref_t<T>>(),
                              echo::concept::writable<decltype(x.data())>()>;
};
}

template <class T>
constexpr bool modifiable_homogenous_data_frame_forward() {
  return models<DETAIL_NS::ModifiableHomogenousDataFrameForward, T>();
}

//------------------------------------------------------------------------------
// contiguous_data_frame
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ContiguousDataFrame : Concept {
  template <class T>
  auto require(T&& data_frame)
      -> list<homogenous_data_frame<T>(),
              k_array::concept::contiguous_shape<
                  uncvref_t<decltype(data_frame.shape())>>()>;
};
}

template <class T>
constexpr bool contiguous_data_frame() {
  return models<DETAIL_NS::ContiguousDataFrame, T>();
}

//------------------------------------------------------------------------------
// numeric_data_frame
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct NumericDataFrame : Concept {
  template <class T>
  auto require(T&& x) -> list<
      homogenous_data_frame<T>(),
      execution_context::concept::scalar<uncvref_t<decltype(*x.data())>>()>;
};
}

template <class T>
constexpr bool numeric_data_frame() {
  return models<DETAIL_NS::NumericDataFrame, T>();
}

//------------------------------------------------------------------------------
// homogenous_data_frame_convertible
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct HomogenousDataFrameConvertible : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b)
      -> list<homogenous_data_frame<A>(), homogenous_data_frame<B>(),
              same<typename A::column_tags, typename B::column_tags>(),
              convertible<uncvref_t<decltype(*a.data())>,
                          uncvref_t<decltype(*b.data())>>()>;
};
}

template <class A, class B>
constexpr bool homogenous_data_frame_convertible() {
  return models<DETAIL_NS::HomogenousDataFrameConvertible, A, B>();
}

//------------------------------------------------------------------------------
// homogenous_data_frame_row
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Pointer, class Shape, class ColumnTags, class MemoryBackendTag>
auto homogenous_data_frame_row_impl(
    HomogenousDataFrameRow<Pointer, Shape, ColumnTags, MemoryBackendTag> && )
    -> std::true_type;

template <class T>
auto homogenous_data_frame_row_impl(T && ) -> std::false_type;
}

template <class T>
constexpr bool homogenous_data_frame_row() {
  using Result =
      decltype(DETAIL_NS::homogenous_data_frame_row_impl(std::declval<T>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// homogenous_data_frame_row_shape
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class... Tags, class Shape>
auto match_row_extents(htl::Tuple<Tags...>&&, Shape&& shape)
    -> std::integral_constant<
        bool, std::is_same<StaticIndex<sizeof...(Tags)>,
                           decltype(get_extent<0>(shape))>::value>;

struct HomogenousDataFrameRowShape : Concept {
  template <class ColumnTags, class Shape>
  auto require(ColumnTags&& column_tags, Shape&& shape) -> list<
      concept::column_tags<ColumnTags>(), k_array::concept::shape_<1, Shape>(),
      static_cast<bool>(decltype(match_row_extents(std::declval<ColumnTags>(),
                                                   std::declval<Shape>())){})>;
};
}

template <class ColumnTags, class Shape>
constexpr bool homogenous_data_frame_row_shape() {
  return models<DETAIL_NS::HomogenousDataFrameRowShape, ColumnTags, Shape>();
}
}
}
}

#undef DETAIL_NS
