#pragma once

#include <echo/data_frame/homogenous_data_frame_fwd.h>
#include <echo/data_frame/homogenous_data_frame_accessor.h>
#include <echo/data_frame/homogenous_data_frame_traits.h>
#include <echo/data_frame/matrix_conversion.h>
#include <echo/data_frame/concept.h>
#include <echo/data_frame/subvector.h>
#include <echo/numeric_array.h>
#include <echo/execution_context.h>

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

  auto& operator=(InitializerMultilist<T, 2> values) {
    _k_array = values;
    return *this;
  }

 private:
  KArray<T, Shape<index_t, NumColumns>, Allocator> _k_array;
};

//------------------------------------------------------------------------------
// make_homogenous_data_frame
//------------------------------------------------------------------------------
template <class T, class ColumnTags, class Allocator = std::allocator<T>,
          CONCEPT_REQUIRES(concept::column_tags<ColumnTags>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_homogenous_data_frame(index_t num_rows,
                                const Allocator& allocator = Allocator()) {
  return HomogenousDataFrame<T, ColumnTags, Allocator>(num_rows, allocator);
}

template <class T, class ColumnTags, class Allocator = std::allocator<T>,
          CONCEPT_REQUIRES(concept::column_tags<ColumnTags>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_homogenous_data_frame(ColumnTags, index_t num_rows,
                                const Allocator& allocator = Allocator()) {
  return HomogenousDataFrame<T, ColumnTags, Allocator>(num_rows, allocator);
}

//------------------------------------------------------------------------------
// NumericDataFrame
//------------------------------------------------------------------------------
template <class T, class ColumnTags, class Allocator = memory::SimdAllocator<T>>
using NumericDataFrame = HomogenousDataFrame<T, ColumnTags, Allocator>;

//------------------------------------------------------------------------------
// make_numeric_data_frame
//------------------------------------------------------------------------------
template <class Scalar, class ColumnTags,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           concept::column_tags<ColumnTags>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_numeric_data_frame(index_t num_rows,
                             const Allocator& allocator = Allocator()) {
  return make_homogenous_data_frame<Scalar, ColumnTags>(num_rows, allocator);
}

template <class Scalar, class ColumnTags,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           concept::column_tags<ColumnTags>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_numeric_data_frame(ColumnTags, index_t num_rows,
                             const Allocator& allocator = Allocator()) {
  return make_homogenous_data_frame<Scalar, ColumnTags>(num_rows, allocator);
}

template <
    class ExecutionContext, class DataFrame,
    CONCEPT_REQUIRES(
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        execution_context::concept::expression_executer<ExecutionContext>() &&
        concept::numeric_data_frame<uncvref_t<DataFrame>>())>
auto make_numeric_data_frame(const ExecutionContext& execution_context,
                             DataFrame&& data_frame) {
  using Scalar = homogenous_data_frame_traits::value_type<uncvref_t<DataFrame>>;
  auto result = make_numeric_data_frame<
      Scalar, homogenous_data_frame_traits::column_tags<uncvref_t<DataFrame>>>(
      get_num_rows(data_frame),
      make_aligned_allocator<Scalar>(execution_context));
  copy(execution_context, make_matrix_cview(data_frame),
       make_matrix_view(result));
  return result;
}
}
}
