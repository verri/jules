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
using boost::copy;

using boost::make_iterator_range;

// TODO Use for range checking
// first need to make iterators inherit from std::iterator
template <typename Range> using range_value_t = typename boost::range_value<Range>::type;

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
