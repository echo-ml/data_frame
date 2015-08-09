#pragma once

#include <echo/data_frame/homogenous_data_frame.h>
#include <echo/data_frame/homogenous_data_frame_view_fwd.h>
#include <echo/data_frame/homogenous_data_frame_accessor.h>
#include <echo/k_array.h>

namespace echo {
namespace data_frame {
//------------------------------------------------------------------------------
// HomogenousDataFrameView
//------------------------------------------------------------------------------
template <class Pointer, class Shape, class... ColumnTags,
          class MemoryBackendTag>
class HomogenousDataFrameView<Pointer, Shape, htl::Tuple<ColumnTags...>,
                              MemoryBackendTag>
    : public HomogenousDataFrameConstAccessor<
          HomogenousDataFrameView<Pointer, Shape, htl::Tuple<ColumnTags...>,
                                  MemoryBackendTag>,
          htl::Tuple<ColumnTags...> > {
  using NumColumns = StaticIndex<sizeof...(ColumnTags)>;

 public:
  using column_tags = htl::Tuple<ColumnTags...>;
  using memory_backend_tag = MemoryBackendTag;
  HomogenousDataFrameView(Pointer data, const Shape& shape)
      : _k_array(data, shape) {}
  auto data() const { return _k_array.data(); }
  auto const_data() const { return _k_array.const_data(); }
  const auto& k_array() const { return _k_array; }
  const auto& shape() const { return _k_array.shape(); }

 private:
  KArrayView<Pointer, Shape, MemoryBackendTag> _k_array;
};

//------------------------------------------------------------------------------
// make_homogenous_data_frame_view
//------------------------------------------------------------------------------
template <
    class ColumnTags, class Pointer, class Shape,
    CONCEPT_REQUIRES(concept::column_tags<ColumnTags>() &&
                     echo::concept::contiguous_iterator<Pointer>() &&
                     concept::homogenous_data_frame_shape<ColumnTags, Shape>())>
auto make_homogenous_data_frame_view(Pointer data, const Shape& shape) {
  return HomogenousDataFrameView<Pointer, Shape, ColumnTags>(data, shape);
}

template <
    class ColumnTags, class MemoryBackendTag, class Pointer, class Shape,
    CONCEPT_REQUIRES(concept::column_tags<ColumnTags>() &&
                     memory::concept::memory_backend_tag<MemoryBackendTag>() &&
                     echo::concept::contiguous_iterator<Pointer>() &&
                     concept::homogenous_data_frame_shape<ColumnTags, Shape>())>
auto make_homogenous_data_frame_view(Pointer data, const Shape& shape) {
  return HomogenousDataFrameView<Pointer, Shape, ColumnTags, MemoryBackendTag>(
      data, shape);
}

//------------------------------------------------------------------------------
// make_view
//------------------------------------------------------------------------------
template <class T, class ColumnTags, class Allocator>
auto make_view(HomogenousDataFrame<T, ColumnTags, Allocator>& data_frame) {
  return make_homogenous_data_frame_view<
      ColumnTags, memory_backend_traits::memory_backend_tag<Allocator> >(
      data_frame.data(), data_frame.shape());
}

template <class T, class ColumnTags, class Allocator>
auto make_view(
    const HomogenousDataFrame<T, ColumnTags, Allocator>& data_frame) {
  return make_homogenous_data_frame_view<
      ColumnTags, memory_backend_traits::memory_backend_tag<Allocator> >(
      data_frame.data(), data_frame.shape());
}

template <class Pointer, class Shape, class ColumnTags, class MemoryBackendTag>
auto make_view(const HomogenousDataFrameView<
    Pointer, Shape, ColumnTags, MemoryBackendTag>& data_frame_view) {
  return data_frame_view;
}

//------------------------------------------------------------------------------
// make_cview
//------------------------------------------------------------------------------
template <class T, class ColumnTags, class Allocator>
auto make_cview(
    const HomogenousDataFrame<T, ColumnTags, Allocator>& data_frame) {
  return make_homogenous_data_frame_view<
      ColumnTags, memory_backend_traits::memory_backend_tag<Allocator> >(
      data_frame.const_data(), data_frame.shape());
}

template <class Pointer, class Shape, class ColumnTags, class MemoryBackendTag>
auto make_cview(const HomogenousDataFrameView<
    Pointer, Shape, ColumnTags, MemoryBackendTag>& data_frame_view) {
  return make_homogenous_data_frame_view<ColumnTags, MemoryBackendTag>(
      data_frame_view.const_data(), data_frame_view.shape());
}
}
}
