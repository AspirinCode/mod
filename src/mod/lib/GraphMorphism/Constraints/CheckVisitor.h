#ifndef MOD_LIB_GRAPHMORPHISM_MATCHESVISITOR_H
#define MOD_LIB_GRAPHMORPHISM_MATCHESVISITOR_H

#include <mod/lib/GraphMorphism/Constraints/AllVisitor.h>

namespace mod {
namespace lib {
namespace GraphMorphism {
namespace Constraints {

template<typename GraphDom, typename LabelledGraphCodom, typename Morphism>
struct CheckVisitor : AllVisitor<GraphDom> {

	CheckVisitor(const GraphDom &gDom, const LabelledGraphCodom &lgCodom, Morphism &m, const LabelSettings ls)
	: gDom(gDom), lgCodom(lgCodom), m(m), ls(ls) { }

	virtual void operator()(const VertexAdjacency<GraphDom> &c) override {
		result = c.matches(*this, gDom, lgCodom, m, ls);
	}

	virtual void operator()(const ShortestPath<GraphDom> &c) override {
		result = c.matches(*this, gDom, lgCodom, m, ls);
	}
public:
	const GraphDom &gDom;
	const LabelledGraphCodom &lgCodom;
	Morphism &m;
	const LabelSettings ls;
	bool result;
};

template<typename ConstraintRange, typename LabelledGraphCodom, typename Next>
struct Checker {

	Checker(ConstraintRange constraints, const LabelledGraphCodom &lgCodom, LabelSettings ls, Next next)
	: constraints(constraints), lgCodom(lgCodom), ls(ls), next(next) { }

	template<typename Morphism, typename GraphDom, typename GraphCodom>
	bool operator()(Morphism &&m, const GraphDom &gDom, const GraphCodom &gCodom) const {
		assert(&gCodom == &get_graph(lgCodom));
		CheckVisitor<GraphDom, LabelledGraphCodom, Morphism> visitor(gDom, lgCodom, m, ls);
		for(const auto &c : constraints) {
			c->accept(visitor);
			if(!visitor.result) return true;
		}
		return next(std::forward<Morphism>(m), gDom, gCodom);
	}
private:
	ConstraintRange constraints;
	const LabelledGraphCodom &lgCodom;
	LabelSettings ls;
	Next next;
};

template<typename ConstraintRange, typename LabelledGraphCodom, typename Next = jla_boost::AlwaysTrue>
Checker<ConstraintRange, LabelledGraphCodom, Next>
makeChecker(ConstraintRange constraints, const LabelledGraphCodom &lgCodom, LabelSettings ls, Next &&next = jla_boost::AlwaysTrue()) {
	return Checker<ConstraintRange, LabelledGraphCodom, Next>(constraints, lgCodom, ls, std::forward<Next>(next));
}

} // namespace Constraints
} // namespace GraphMorphism
} // namespace lib
} // namespace mod

#endif /* MOD_LIB_GRAPHMORPHISM_MATCHESVISITOR_H */
