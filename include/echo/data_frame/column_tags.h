#pragma once

#include <echo/htl.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// ColumnTags
//------------------------------------------------------------------------------
template <class... Tags>
using ColumnTags = htl::Tuple<Tags...>;
}
}
