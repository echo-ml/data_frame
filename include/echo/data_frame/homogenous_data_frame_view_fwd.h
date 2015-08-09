#pragma once

#include <echo/memory.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// HomogenousDataFrameView
//------------------------------------------------------------------------------
template <class Pointer, class Shape, class ColumnTags,
          class MemoryBackendTag = echo::memory::memory_backend_tag>
class HomogenousDataFrameView;
}
}
