#pragma once

#include <echo/data_frame/concept.h>
#include <echo/k_array.h>

namespace echo {
namespace data_frame {

//------------------------------------------------------------------------------
// DataFrameConstAccessor
//------------------------------------------------------------------------------
template <class Derived, class ColumnTags>
struct DataFrameConstAccessor {
  template <class ColumnTag,
            CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
  decltype(auto) operator()(access_mode::readonly_t, index_t row_index,
                            ColumnTag column_tag) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(0 <= row_index < get_extent<0>(derived));
    };
    auto column_index = htl::find_if(
        [](auto column_tag2) -> htl::integral_constant<
            bool, std::is_same<decltype(column_tag2), ColumnTag>::value> {
          return {};
        },
        ColumnTags());
    return derived.k_array()(access_mode::readonly, row_index, column_index);
  }
  template <class ColumnTag,
            CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
  decltype(auto) operator()(index_t row_index, ColumnTag column_tag) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(0 <= row_index < get_extent<0>(derived));
    };
    auto column_index = htl::find_if(
        [](auto column_tag2) -> htl::integral_constant<
            bool, std::is_same<decltype(column_tag2), ColumnTag>::value> {
          return {};
        },
        ColumnTags());
    return derived.k_array()(row_index, column_index);
  }
};

//------------------------------------------------------------------------------
// DataFrameAccessor
//------------------------------------------------------------------------------
}
}
