#include <mod/Py/Common.h>

#include <mod/graph/Printer.h>
#include <mod/rule/GraphInterface.h>
#include <mod/rule/Rule.h>

namespace mod {
namespace rule {
namespace Py {
namespace {

py::object getLabelType(std::shared_ptr<Rule> r) {
	if(r->getLabelType()) return py::object(r->getLabelType().get());
	else return py::object();
}

} // namespace

void Rule_doExport() {
	std::pair<std::string, std::string> (Rule::*printWithoutOptions)() const = &Rule::print;
	std::pair<std::string, std::string> (Rule::*printWithOptions)(const graph::Printer&, const graph::Printer&) const = &Rule::print;

	// rst: .. py:class:: Rule
	// rst:
	// rst:		Model of a transformation rule in the Double Pushout formalism.
	// rst:
	py::class_<Rule, std::shared_ptr<Rule>, boost::noncopyable>("Rule", py::no_init)
			// rst:		.. py:attribute:: id
			// rst:
			// rst:			(Read-only) The unique instance id among all :class:`Rule` instances.
			// rst:
			// rst:			:type: unsigned int
			.add_property("id", &Rule::getId)
			.def(str(py::self))
			//------------------------------------------------------------------
			// rst:		.. py:attribute:: numVertices
			// rst:
			// rst:			(Read-only) The number of vertices in the rule.
			// rst:
			// rst:			:type: int
			.add_property("numVertices", &Rule::numVertices)
			// rst:		.. py:attribute:: vertices
			// rst:
			// rst:			(Read-only) An iterable of all vertices in the rule.
			// rst:
			// rst:			:type: :py:class:`RuleVertexRange`
			.add_property("vertices", &Rule::vertices)
			// rst:		.. py:attribute:: numEdges
			// rst:
			// rst:			(Read-only) The number of edges in the rule.
			// rst:
			// rst:			:type: int
			.add_property("numEdges", &Rule::numEdges)
			// rst:		.. py:attribute:: edges
			// rst:
			// rst:			(Read-only) An iterable of all edges in the rule.
			// rst:
			// rst:			:type: :py:class:`RuleEdgeRange`
			.add_property("edges", &Rule::edges)
			// rst:		.. py:attribute:: left
			// rst:
			// rst:			(Read-only) A proxy object representing the left graph of the rule.
			// rst:
			// rst:			:type: :py:class:`RuleLeftGraph`
			.add_property("left", &Rule::getLeft)
			// rst:		.. py:attribute:: context
			// rst:
			// rst:			(Read-only) A proxy object representing the context graph of the rule.
			// rst:
			// rst:			:type: :py:class:`RuleContextGraph`
			.add_property("context", &Rule::getContext)
			// rst:		.. py:attribute:: right
			// rst:
			// rst:			(Read-only) A proxy object representing the right graph of the rule.
			// rst:
			// rst:			:type: :py:class:`RuleRightGraph`
			.add_property("right", &Rule::getRight)
			//------------------------------------------------------------------
			// rst:		.. py:method:: makeInverse()
			// rst:
			// rst:			:returns: a rule representing the inversion of this rule.
			// rst:			:rtype: :class:`Rule`
			// rst:			:raises: :class:`LogicError` if inversion is not possible (due to matching constraints).
			.def("makeInverse", &Rule::makeInverse)
			// rst:		.. py:method:: print()
			// rst:		               print(first, second=None)
			// rst:
			// rst:			Print the rule, using either the default options or the options in ``first`` and ``second``.
			// rst:			If ``first`` and ``second`` are the same, only one depiction will be made.
			// rst:
			// rst:			:param first: the printing options used for the first depiction.
			// rst:			:type first: :class:`GraphPrinter`
			// rst:			:param second: the printing options used for the second depiction.
			// rst:				If it is ``None`` then it is set to ``first``.
			// rst:			:type second: :class:`GraphPrinter`
			// rst:
			// rst:			:returns: a pair of filename prefixes for the PDF-files that will be compiled in post-processing.
			// rst:				The actual names can be obtained by appending ``_L.pdf``, ``_K.pdf``, and ``_R.pdf`` for
			// rst:				respectively the left side, context, and right side graphs.
			// rst:				If ``first`` and ``second`` are the same, the two file prefixes are equal.
			// rst:			:rtype: (string, string)
			.def("print", printWithoutOptions)
			.def("print", printWithOptions)
			// rst:		.. py:method:: printTermState
			// rst:
			// rst:			Print the term state for the rule.
			.def("printTermState", &Rule::printTermState)
			// rst:		.. py:method:: getGMLString(withCoords=False)
			// rst:
			// rst:			:returns: the :ref:`GML <rule-gml>` representation of the rule,
			// rst:			          optionally with generated 2D coordinates.
			// rst:			:rtype: string
			// rst:			:raises: :py:class:`LogicError` when coordinates are requested, but
			// rst:			         none can be generated.
			.def("getGMLString", &Rule::getGMLString)
			// rst:		.. py:method:: printGML(withCoords=False)
			// rst:	
			// rst:			Print the rule in :ref:`GML <rule-gml>` format,
			// rst:			optionally with generated 2D coordinates.
			// rst:
			// rst:			:returns: the filename of the printed GML file.
			// rst:			:rtype: string
			// rst:			:raises: :py:class:`LogicError` when coordinates are requested, but
			// rst:			         none can be generated.
			.def("printGML", &Rule::printGML)
			// rst:		.. py:attribute:: name
			// rst:
			// rst:			The name of the rule. The default name includes the unique instance id.
			// rst:
			// rst:			:type: string
			.add_property("name", py::make_function(&Rule::getName,
			py::return_value_policy<py::copy_const_reference>()), &Rule::setName)
			// rst:		.. py:attribute:: labelType
			// rst:
			// rst:			(Read-only) The intended label type for this rule, or None if no specific label type is intended.
			// rst:
			// rst:			:type: :py:class:`LabelType`
			.add_property("labelType", &getLabelType)
			// rst:		.. py:attribute:: numLeftComponents
			// rst:
			// rst:			(Read-only) The number of connected components of the left graph.
			// rst:
			// rst:			:type: :cpp:type:`std::size_t`
			.add_property("numLeftComponents", &Rule::getNumLeftComponents)
			// rst:		.. py:attribute:: numRightComponents
			// rst:
			// rst:			(Read-only) The number of connected components of the right graph.
			// rst:
			// rst:			:type: :cpp:type:`std::size_t`
			.add_property("numRightComponents", &Rule::getNumRightComponents)
			// rst:		.. py:method:: isomorphism(other, maxNumMatches=1, labelSettings=LabelSettings(LabelType.String, LabelRelation.Isomorphism))
			// rst:		
			// rst:			:param other: the other :class:`Rule` for comparison.
			// rst:			:type other: :class:`Rule`
			// rst:			:param maxNumMatches: the maximum number of isomorphisms to search for.
			// rst:			:type maxNumMatches: :cpp:any:`std::size_t`
			// rst:			:param labelSettings: the label settings to use during the search.
			// rst:			:type labelSettings: :py:class:`LabelSettings`
			// rst:			:returns: the number of isomorphisms found between ``other`` and this rule, but at most ``maxNumMatches``.
			// rst:			:rtype: :cpp:any:`std::size_t`
			.def("isomorphism", &Rule::isomorphism)
			// rst:		.. py:method:: monomorphism(host, maxNumMatches=1, labelSettings=LabelSettings(LabelType.String, LabelRelation.Isomorphism))
			// rst:		
			// rst:			:param host: the host :class:`Rule` to check for subgraphs.
			// rst:			:type host: :class:`Rule`
			// rst:			:param maxNumMatches: the maximum number of morphisms to search for.
			// rst:			:type maxNumMatches: :cpp:any:`std::size_t`
			// rst:			:param labelSettings: the label settings to use during the search.
			// rst:			:type labelSettings: :py:class:`LabelSettings`
			// rst:			:returns: the number of monomorphisms from this rule to subgraphs of ``host``, though at most ``maxNumMatches``.
			// rst:			:rtype: :cpp:any:`std::size_t`
			.def("monomorphism", &Rule::monomorphism)
			// rst:		.. py:method:: getVertexFromExternalId(id)
			// rst:
			// rst:			If the rule was not loaded from an external data format, then this function
			// rst:			always return a null descriptor.
			// rst:
			// rst:			.. note:: In general there is no correlation between external and internal ids.
			// rst:
			// rst:			:param id: the external id to find the vertex descriptor for.
			// rst:			:type id: int
			// rst:			:returns: the vertex descriptor for the given external id.
			// rst:		            The descriptor is null if the external id was not used.
			// rst:			:rtype: :py:class:`RuleVertex`
			.def("getVertexFromExternalId", &Rule::getVertexFromExternalId)
			// rst:		.. py::attribute:: minExternalId
			// rst:		                   maxExternalId
			// rst:
			// rst:			If the rule was not loaded from an external data format, then these attributes
			// rst:			are always return 0. Otherwise, they are the minimum/maximum external id from which
			// rst:			non-null vertices can be obtained from :py:meth:`getVertexFromExternalId`.
			// rst:			If no such minimum and maximum exists, then they are 0.
			// rst:
			// rst:			:type: int
			.add_property("minExternalId", &Rule::getMinExternalId)
			.add_property("maxExternalId", &Rule::getMaxExternalId)
			;

	// rst: .. py:method:: ruleGMLString(s, invert=False)
	// rst:
	// rst:		Load a rule from a :ref:`GML <rule-gml>` string, and store either that rule or its inverse.
	// rst:		The name of the rule is the one specified in the GML string, though when ``invert=True``
	// rst:		the string ", inverse" is appended to the name.
	// rst:
	// rst:		.. note::
	// rst:
	// rst:			If the GML string specifies matching constraints it is not possible to invert the rule.
	// rst:			There is however a configuration option to ignore matching constraints when inverting rules.
	// rst:
	// rst:		:param string s: the GML string to load a rule from.
	// rst:		:param bool invert: whether or not to invert the loaded rule.
	// rst:		:returns: the rule in the GML string, possibly inverted.
	// rst:		:rtype: :class:`Rule`
	py::def("ruleGMLString", &Rule::ruleGMLString);
	// rst: .. py:method:: ruleGML(s, invert=False)
	// rst:		
	// rst:		Read ``file`` and pass the contents to :py:func:`ruleGMLString`.
	// rst:
	// rst:		:param string s: name of the GML file to be loaded.
	// rst:		:param bool invert: whether or not to invert the loaded rule.
	// rst:		:returns: the rule in the GML file, possibly inverted.
	// rst:		:rtype: :class:`Rule`
	py::def("ruleGML", &Rule::ruleGML);
}

} // namespace Py
} // namespace rule
} // namespace mod