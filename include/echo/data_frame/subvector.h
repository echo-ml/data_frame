#pragma once

#include <echo/data_frame/concept.h>
#include <echo/data_frame/homogenous_data_frame_row.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// get_num_rows
//------------------------------------------------------------------------------
template <class DataFrame,
          CONCEPT_REQUIRES(concept::homogenous_data_frame<DataFrame>())>
index_t get_num_rows(const DataFrame& data_frame) {
  return get_extent<0>(data_frame);
}

//------------------------------------------------------------------------------
// get_num_columns
//------------------------------------------------------------------------------
template <class DataFrame,
          CONCEPT_REQUIRES(concept::homogenous_data_frame<DataFrame>())>
auto get_num_columns(const DataFrame& data_frame) {
  return get_extent<1>(data_frame);
}

//------------------------------------------------------------------------------
// get_column
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::homogenous_data_frame<
                               uncvref_t<DataFrame>>())>
auto get_column(DataFrame&& data_frame, index_t j) {
  CONTRACT_EXPECT {
    CONTRACT_ASSERT(0 <= j && j < get_num_columns(data_frame));
  };
  auto shape = make_subshape(data_frame.shape(), slice::all, j);
  return k_array::make_k_array_view<
      typename uncvref_t<DataFrame>::memory_backend_tag>(
      &data_frame.k_array()(0, j), shape);
}

template <class DataFrame, class ColumnTag,
          CONCEPT_REQUIRES(
              concept::homogenous_data_frame<uncvref_t<DataFrame>>() &&
              concept::column_tag<typename uncvref_t<DataFrame>::column_tags,
                                  ColumnTag>())>
auto get_column(DataFrame&& data_frame, ColumnTag) {
  using ColumnTags = typename uncvref_t<DataFrame>::column_tags;
  auto column_index = get_column_index<ColumnTags, ColumnTag>();
  return get_column(std::forward<DataFrame>(data_frame), column_index);
}

//------------------------------------------------------------------------------
// get_row
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::homogenous_data_frame<
                               uncvref_t<DataFrame>>())>
auto get_row(DataFrame&& data_frame, index_t i) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(0 <= i && i < get_num_rows(data_frame)); };
  auto shape = make_subshape(data_frame.shape(), i, slice::all);
  return make_homogenous_data_frame_row<
      typename uncvref_t<DataFrame>::column_tags,
      typename uncvref_t<DataFrame>::memory_backend_tag>(
      &data_frame.k_array()(i, 0), shape);
}
}
}
