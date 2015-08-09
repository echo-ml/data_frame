#pragma once

#include <echo/data_frame/concept.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// all_begin
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::contiguous_data_frame<
                               uncvref_t<DataFrame>>())>
auto all_begin(DataFrame&& data_frame) {
  return all_begin(data_frame.k_array());
}

//------------------------------------------------------------------------------
// all_end
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::contiguous_data_frame<
                               uncvref_t<DataFrame>>())>
auto all_end(DataFrame&& data_frame) {
  return all_end(data_frame.k_array());
}

//------------------------------------------------------------------------------
// all_cbegin
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::contiguous_data_frame<
                               uncvref_t<DataFrame>>())>
auto all_cbegin(DataFrame&& data_frame) {
  return all_cbegin(data_frame.k_array());
}

//------------------------------------------------------------------------------
// all_cend
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::contiguous_data_frame<
                               uncvref_t<DataFrame>>())>
auto all_cend(DataFrame&& data_frame) {
  return all_cend(data_frame.k_array());
}

//------------------------------------------------------------------------------
// all_range
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::contiguous_data_frame<
                               uncvref_t<DataFrame>>())>
auto all_range(DataFrame&& data_frame) {
  return all_range(data_frame.k_array());
}

//------------------------------------------------------------------------------
// all_crange
//------------------------------------------------------------------------------
template <class DataFrame, CONCEPT_REQUIRES(concept::contiguous_data_frame<
                               uncvref_t<DataFrame>>())>
auto all_crange(DataFrame&& data_frame) {
  return all_crange(data_frame.k_array());
}
}
}
