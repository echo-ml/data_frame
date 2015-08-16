#pragma once

#include <echo/data_frame/homogenous_data_frame_view.h>
#include <echo/linear_algebra.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// make_matrix_view
//------------------------------------------------------------------------------
template <class DataFrame,
          CONCEPT_REQUIRES(concept::numeric_data_frame<uncvref_t<DataFrame>>())>
auto make_matrix_view(DataFrame&& data_frame) {
  return numeric_array::make_numeric_array_view<
      linear_algebra::structure::matrix_general,
      typename uncvref_t<DataFrame>::memory_backend_tag>(data_frame.data(),
                                                         data_frame.shape());
}

//------------------------------------------------------------------------------
// make_matrix_cview
//------------------------------------------------------------------------------
template <class DataFrame>
auto make_matrix_cview(DataFrame&& data_frame)
    -> decltype(make_cview(make_matrix_view(data_frame))) {
  return make_cview(make_matrix_view(data_frame));
}
}
}
