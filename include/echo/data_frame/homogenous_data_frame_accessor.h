#pragma once

#include <echo/data_frame/concept.h>
#include <echo/data_frame/tag_matcher.h>
#include <echo/k_array.h>

namespace echo {
namespace data_frame {
//------------------------------------------------------------------------------
// HomogenousDataFrameConstAccessor
//------------------------------------------------------------------------------
template <class Derived, class ColumnTags>
struct HomogenousDataFrameConstAccessor {
  template <class ColumnTag,
            CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
  decltype(auto) operator()(access_mode::readonly_t, index_t row_index,
                            ColumnTag column_tag) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(0 <= row_index && row_index < get_extent<0>(derived));
    };
    auto column_index =
        htl::find_if(detail::TagMatcher<ColumnTag>(), ColumnTags());
    return derived.k_array()(access_mode::readonly, row_index, column_index);
  }
  template <class ColumnTag,
            CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
  decltype(auto) operator()(index_t row_index, ColumnTag column_tag) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(0 <= row_index && row_index < get_extent<0>(derived));
    };
    auto column_index =
        htl::find_if(detail::TagMatcher<ColumnTag>(), ColumnTags());
    return derived.k_array()(row_index, column_index);
  }
  CONCEPT_MEMBER_REQUIRES(htl::tuple_traits::num_elements<ColumnTags>() == 1)
  decltype(auto) operator()(index_t row_index) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(0 <= row_index && row_index < get_extent<0>(derived));
    };
    return derived.k_array()(row_index, 0);
  }
};

//------------------------------------------------------------------------------
// HomogenousDataFrameAccessor
//------------------------------------------------------------------------------
template <class Derived, class ColumnTags>
struct HomogenousDataFrameAccessor
    : HomogenousDataFrameConstAccessor<Derived, ColumnTags> {
  using HomogenousDataFrameConstAccessor<Derived, ColumnTags>::operator();
  template <class ColumnTag,
            CONCEPT_REQUIRES(concept::column_tag<ColumnTags, ColumnTag>())>
  decltype(auto) operator()(index_t row_index, ColumnTag column_tag) {
    Derived& derived = static_cast<Derived&>(*this);
    const auto& const_base = static_cast<
        const HomogenousDataFrameConstAccessor<Derived, ColumnTags>&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(0 <= row_index && row_index < get_extent<0>(derived));
    };
    return const_cast<decltype(*derived.data())>(
        const_base.operator()(row_index, column_tag));
  }
  CONCEPT_MEMBER_REQUIRES(htl::tuple_traits::num_elements<ColumnTags>() == 1)
  decltype(auto) operator()(index_t row_index) {
    Derived& derived = static_cast<Derived&>(*this);
    const auto& const_base = static_cast<
        const HomogenousDataFrameConstAccessor<Derived, ColumnTags>&>(*this);
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(0 <= row_index && row_index < get_extent<0>(derived));
    };
    return const_cast<decltype(*derived.data())>(
        const_base.operator()(row_index));
  }
};
}
}

#undef detail
