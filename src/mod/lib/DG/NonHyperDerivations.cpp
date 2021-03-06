#include "NonHyperDerivations.h"

#include <mod/Derivation.h>
#include <mod/rule/Rule.h>
#include <mod/lib/Graph/Single.h>
#include <mod/lib/Rules/Real.h>

namespace mod {
namespace lib {
namespace DG {

NonHyperDerivations::NonHyperDerivations(std::vector<Derivation> derivations)
: NonHyper({},
{
LabelType::String, LabelRelation::Isomorphism, false, LabelRelation::Isomorphism
}), derivations(new std::vector<Derivation>(std::move(derivations))) {
	calculate();
}

std::string NonHyperDerivations::getType() const {
	return "DGDerivations";
}

void NonHyperDerivations::calculateImpl() {
	assert(derivations);
	// add all graphs
	for(const auto &der : *derivations) {
		for(auto g : der.left) this->addGraphAsVertex(g);
		for(auto g : der.right) this->addGraphAsVertex(g);
	}
	// add derivations
	for(const auto &der : *derivations) {
		const auto makeSide = [this](const mod::Derivation::GraphList & graphs) -> GraphMultiset {
			std::vector<const lib::Graph::Single*> gPtrs;
			gPtrs.reserve(graphs.size());
			for(const auto &g : graphs) gPtrs.push_back(&g->getGraph());
			return GraphMultiset(std::move(gPtrs));
		};
		auto gmsLeft = makeSide(der.left);
		auto gmsRight = makeSide(der.right);
		const lib::Rules::Real *rule = nullptr;
		if(der.r) rule = &der.r->getRule();
		this->suggestDerivation(std::move(gmsLeft), std::move(gmsRight), rule);
	}
	derivations.release();
}

void NonHyperDerivations::listImpl(std::ostream &s) const {
	s << "(nothing to list for " << getType() << "(" << getId() << ")" << std::endl;
}

} // namespace DG
} // namespace lib
} // namespace mod