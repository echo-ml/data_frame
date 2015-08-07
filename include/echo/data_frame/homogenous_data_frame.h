#pragma once

#include <echo/data_frame/homogenous_data_frame_accessor.h>
#include <echo/k_array.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// HomogenousDataFrame
//------------------------------------------------------------------------------
template <class T, class ColumnTags, class Allocator>
class HomogenousDataFrame {
  static_assert(!std::is_same<T, T>::value, "invalid column tags");
};

template <class T, class... ColumnTags, class Allocator>
class HomogenousDataFrame<T, htl::Tuple<ColumnTags...>, Allocator> {
  using NumColumns = StaticIndex<sizeof...(ColumnTags)>;

 public:
  using column_tags = htl::Tuple<ColumnTags...>;
  HomogenousDataFrame(index_t num_rows,
                      const Allocator& allocator = Allocator())
      : _k_array(make_shape(num_rows, NumColumns()), allocator) {}
  const auto& shape() const { return _k_array.shape(); }
  T* data() { return _k_array.data(); }
  const T* data() const { return _k_array.data(); }
  const T* const_data() const { return _k_array.data(); }

  const auto& k_array() const { return _k_array; }
  auto& k_array() { return _k_array; }

 private:
  KArray<T, Shape<index_t, NumColumns>, Allocator> _k_array;
};
}
}
