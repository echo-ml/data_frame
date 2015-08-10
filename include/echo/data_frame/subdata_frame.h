#pragma once

#include <echo/data_frame/subvector.h>
#include <echo/data_frame/homogenous_data_frame_view.h>

namespace echo {
namespace data_frame {

template <class DataFrame, CONCEPT_REQUIRES(concept::homogenous_data_frame<
                               uncvref_t<DataFrame>>())>
auto make_subdata_frame(DataFrame&& data_frame, index_t row_first,
                        index_t row_last) {
  CONTRACT_EXPECT {
    CONTRACT_ASSERT(0 <= row_first && row_first <= row_last);
    CONTRACT_ASSERT(row_last <= get_num_rows(data_frame));
  };
  auto shape = make_subshape(data_frame.shape(),
                             slice::range(row_first, row_last), slice::all);
  return make_homogenous_data_frame_view<
      typename uncvref_t<DataFrame>::column_tags,
      typename uncvref_t<DataFrame>::memory_backend_tag>(
      &data_frame.k_array()(row_first, 0), shape);
}
}
}
