#ifndef JLA_BOOST_GRAPH_PAIRTORANGEADAPTOR_H
#define	JLA_BOOST_GRAPH_PAIRTORANGEADAPTOR_H

#include <utility>

namespace jla_boost {

// adapted from http://stackoverflow.com/questions/6167598/why-was-pair-range-access-removed-from-c11
// (e.g., for used with Boost.Graph, which returns pairs of iterators)

template<typename Iter>
struct range : std::pair<Iter, Iter> {

	range(const std::pair<Iter, Iter> &x) : std::pair<Iter, Iter>(x) { }

	Iter begin() const {
		return this->first;
	}

	Iter end() const {
		return this->second;
	}
};

template<typename Iter>
inline range<Iter> asRange(const std::pair<Iter, Iter> &x) {
	return range<Iter>(x);
}

} // namespace jla_boost
namespace mod { // HAX!
using jla_boost::asRange;
} // namespace mod

#endif	/* JLA_BOOST_GRAPH_PAIRTORANGEADAPTOR_H */