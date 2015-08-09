#pragma once

#include <echo/data_frame/homogenous_data_frame_fwd.h>
#include <echo/data_frame/homogenous_data_frame_accessor.h>
#include <echo/k_array.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// HomogenousDataFrame
//------------------------------------------------------------------------------
template <class T, class... ColumnTags, class Allocator>
class HomogenousDataFrame<T, htl::Tuple<ColumnTags...>, Allocator>
    : public HomogenousDataFrameAccessor<
          HomogenousDataFrame<T, htl::Tuple<ColumnTags...>, Allocator>,
          htl::Tuple<ColumnTags...>> {
  using NumColumns = StaticIndex<sizeof...(ColumnTags)>;

 public:
  using column_tags = htl::Tuple<ColumnTags...>;
  using memory_backend_tag =
      memory_backend_traits::memory_backend_tag<Allocator>;
  explicit HomogenousDataFrame(index_t num_rows,
                               const Allocator& allocator = Allocator())
      : _k_array(make_shape(num_rows, NumColumns()), allocator) {}
  const auto& shape() const { return _k_array.shape(); }
  auto data() { return _k_array.data(); }
  auto data() const { return _k_array.data(); }
  auto const_data() const { return _k_array.data(); }

  const auto& k_array() const { return _k_array; }
  auto& k_array() { return _k_array; }

 private:
  KArray<T, Shape<index_t, NumColumns>, Allocator> _k_array;
};

//------------------------------------------------------------------------------
// NumericDataFrame
//------------------------------------------------------------------------------
template <class T, class ColumnTags>
using NumericDataFrame =
    HomogenousDataFrame<T, ColumnTags, memory::SimdAllocator<T>>;
}
}
