#pragma once

#include <echo/data_frame/concept.h>
#include <echo/k_array.h>

namespace echo { namespace data_frame {

template<class Pointer, class Shape, class ColumnTags, class MemoryBackendTag>
class HomogenousDataFrameRow 
{
 public:
  using column_tags = ColumnTags;
  HomogenousDataFrameRow(Pointer data, const Shape& shape)
    : _k_array(data, shape)
  {}

  template<class ColumnTag, CONCEPT_REQUIRES(
    concept::column_tag<ColumnTags, ColumnTag>()
  )>
  decltype(auto) operator()(ColumnTag column_tag) const {

  }
 private:
  KArrayView<Pointer, Shape, MemoryBackendTag> _k_array;
};

}}
