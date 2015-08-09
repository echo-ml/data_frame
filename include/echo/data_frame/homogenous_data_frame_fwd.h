#pragma once

#include <memory>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// HomogenousDataFrame
//------------------------------------------------------------------------------
template <class T, class ColumnTags, class Allocator = std::allocator<T>>
class HomogenousDataFrame;
}
}
