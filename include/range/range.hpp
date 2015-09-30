#ifndef JULES_RANGE_RANGE_H
#define JULES_RANGE_RANGE_H

#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/strided.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/uniqued.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/istream_range.hpp>

#include <range/tokenized.hpp>

namespace jules
{
namespace range
{

static struct tag_t {} tag;

using boost::copy;

using boost::make_iterator_range;

template <typename Range, typename = void, typename T = typename std::remove_reference_t<Range>::iterator>
struct range_iterator
{
    using type = T;
};

template <typename Range>
struct range_iterator<Range, std::enable_if_t<std::is_const<std::remove_reference_t<Range>>::value>, typename std::remove_reference_t<Range>::const_iterator>
{
    using type = typename std::remove_reference_t<Range>::const_iterator;
};

template <typename Range> using range_iterator_t = typename range_iterator<Range>::type;
template <typename Range> using range_value_t = typename std::iterator_traits<range_iterator_t<Range>>::value_type;

using boost::begin;
using boost::end;

using boost::size;

using boost::adaptors::index;
using boost::adaptors::stride;
using boost::adaptors::transform;
using boost::adaptors::unique;
} // namespace range

namespace adaptors
{
using boost::adaptors::indexed;
using boost::adaptors::strided;
using boost::adaptors::transformed;
using boost::adaptors::uniqued;
} // namespace adaptors
} // namespace jules

#endif // JULES_RANGE_RANGE_H
