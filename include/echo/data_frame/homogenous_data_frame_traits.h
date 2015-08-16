#pragma once

#include <echo/k_array.h>

namespace echo { namespace homogenous_data_frame_traits {

//------------------------------------------------------------------------------
// value_type
//------------------------------------------------------------------------------
template<class T>
using value_type = uncvref_t<decltype(*std::declval<T>().data())>;

//------------------------------------------------------------------------------
// column_tags
//------------------------------------------------------------------------------
template<class T>
using column_tags = typename T::column_tags;

//------------------------------------------------------------------------------
// column_extent_type
//------------------------------------------------------------------------------
template<class T>
using column_extent_type = shaped_traits::extent_type<1, T>;

}}
