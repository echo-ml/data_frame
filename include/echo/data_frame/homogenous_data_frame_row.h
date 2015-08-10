#pragma once

#include <echo/data_frame/homogenous_data_frame_row_fwd.h>
#include <echo/data_frame/tag_matcher.h>
#include <echo/data_frame/concept.h>
#include <echo/k_array.h>

namespace echo {
namespace data_frame {
//------------------------------------------------------------------------------
// HomogenousDataFrameRow
//------------------------------------------------------------------------------
template <class Pointer, class Shape, class ColumnTags, class MemoryBackendTag>
class HomogenousDataFrameRow {
 public:
  using column_tags = ColumnTags;
  HomogenousDataFrameRow(Pointer data, const Shape& shape)
      : _k_array(data, shape) {}

  template <class ColumnTag,
            CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
  decltype(auto) operator()(ColumnTag column_tag) const {
    auto column_index =
        htl::find_if(detail::TagMatcher<ColumnTag>(), ColumnTags());
    return _k_array(column_index);
  }

  template <class ColumnTag,
            CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
  decltype(auto) operator()(access_mode::readonly_t,
                            ColumnTag column_tag) const {
    auto column_index =
        htl::find_if(detail::TagMatcher<ColumnTag>(), ColumnTags());
    return _k_array(access_mode::readonly, column_index);
  }

  const auto& k_array() const { return _k_array; }

 private:
  KArrayView<Pointer, Shape, MemoryBackendTag> _k_array;
};

//------------------------------------------------------------------------------
// make_homogenous_data_frame_row
//------------------------------------------------------------------------------
template <class ColumnTags, class MemoryBackendTag, class Pointer, class Shape,
          CONCEPT_REQUIRES(
              concept::column_tags<ColumnTags>() &&
              memory::concept::memory_backend_tag<MemoryBackendTag>() &&
              echo::concept::contiguous_iterator<Pointer>() &&
              concept::homogenous_data_frame_row_shape<ColumnTags, Shape>())>
auto make_homogenous_data_frame_row(Pointer data, const Shape& shape) {
  return HomogenousDataFrameRow<Pointer, Shape, ColumnTags, MemoryBackendTag>(
      data, shape);
}
}
}
