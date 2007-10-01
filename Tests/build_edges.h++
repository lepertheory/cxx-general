/*****************************************************************************
 * build_edges.h++
 *****************************************************************************
 * Provides commonly-used edge cases.
 *****************************************************************************/

// Standard includes.
#include <limits>

// Internal includes.
#include "NumInfo.h++"

/*****************************************************************************/
// Functions.

// Build standard single-type edges.
template <class T> void build_edges (T& edges              );

// Build two-type edge cases.
template <class T, class U> void build_edges (T& edges_t, U& edges_u);

// Merge one group of edges into another.
template <class T, class U> void merge_edges (T& dest, T const& edges_t, U const& edges_u);

/*****************************************************************************/
// Utilities.
namespace Build_EdgesUtil {
	
	// Build standard single-type edges.
	template <class T, bool is_signed> class Build_Edges;
	template <class T> class Build_Edges<T, false> { public: static void op (T& edges); };
	template <class T> class Build_Edges<T, true > { public: static void op (T& edges); };
	
	// Clean up after combining edges.
	template <class T> void clean_edges (T& edges);
	
	// Check if a number fits in a given type.
	template <class T, class U, DAC::RelTypes::Type> class Number_Fits;
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::SE_SE> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::SE_UE> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::SS_SL> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::SS_UL> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::SL_SS> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::SL_US> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::UE_SE> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::UE_UE> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::US_SL> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::US_UL> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::UL_SS> { public: static bool op (U const& number); };
	template <class T, class U> class Number_Fits<T, U, DAC::RelTypes::UL_US> { public: static bool op (U const& number); };
	
}

/*****************************************************************************
 * Inline and template definitions.
 *****************************************************************************/

/*****************************************************************************/
// Functions.

/*
 * Build standard single-type edges.
 */
template <class T> inline void build_edges (T& edges) {
	Build_EdgesUtil::Build_Edges<T, std::numeric_limits<typename T::value_type>::is_signed>::op(edges);
}
template <class T, class U> inline void build_edges (T& edges_t, U& edges_u) {
	T src_edges_t;
	U src_edges_u;
	build_edges(src_edges_t);
	build_edges(src_edges_u);
	merge_edges(edges_t, src_edges_t, src_edges_u);
	merge_edges(edges_u, src_edges_u, src_edges_t);
}

// Merge one group of edges into another.
template <class T, class U> void merge_edges (T& dest, T const& edges_t, U const& edges_u) {
	dest = edges_t;
	for (typename U::const_iterator i = edges_u.begin(); i != edges_u.end(); ++i) {
		if (Build_EdgesUtil::Number_Fits<typename T::value_type, typename U::value_type, DAC::IntRel<typename T::value_type, typename U::value_type>::type>::op(*i)) {
			dest.push_back(static_cast<typename T::value_type>(*i));
		}
	}
	Build_EdgesUtil::clean_edges(dest);
}

/*****************************************************************************/
// Utilities.

/*
 * Build standard single-type edges.
 */
template <class T> void Build_EdgesUtil::Build_Edges<T, false>::op (T& edges) {
	edges.push_back(static_cast<typename T::value_type>(0));
	if (std::numeric_limits<typename T::value_type>::digits >= 1) {
		edges.push_back(static_cast<typename T::value_type>(1));
		if (std::numeric_limits<typename T::value_type>::digits >= 2) {
			if (std::numeric_limits<typename T::value_type>::digits >= 4) {
				edges.push_back((static_cast<typename T::value_type>(1) << (std::numeric_limits<typename T::value_type>::digits >> 1)) - 2);
				edges.push_back((static_cast<typename T::value_type>(1) << (std::numeric_limits<typename T::value_type>::digits >> 1)) - 1);
				edges.push_back( static_cast<typename T::value_type>(1) << (std::numeric_limits<typename T::value_type>::digits >> 1)     );
			}
			edges.push_back(std::numeric_limits<typename T::value_type>::max() - 1);
			edges.push_back(std::numeric_limits<typename T::value_type>::max()    );
		}
	}
}
template <class T> void Build_EdgesUtil::Build_Edges<T, true>::op (T& edges) {
	if (std::numeric_limits<typename T::value_type>::digits >= 1) {
		if (std::numeric_limits<typename T::value_type>::digits >= 2) {
			edges.push_back(std::numeric_limits<typename T::value_type>::min()    );
			edges.push_back(std::numeric_limits<typename T::value_type>::min() + 1);
			if (std::numeric_limits<typename T::value_type>::digits >= 4) {
				edges.push_back((static_cast<typename T::value_type>(-1) << (std::numeric_limits<typename T::value_type>::digits >> 1)) - 1);
				edges.push_back( static_cast<typename T::value_type>(-1) << (std::numeric_limits<typename T::value_type>::digits >> 1)     );
				edges.push_back((static_cast<typename T::value_type>(-1) << (std::numeric_limits<typename T::value_type>::digits >> 1)) + 1);
			}
		}
		edges.push_back(static_cast<typename T::value_type>(-2));
		edges.push_back(static_cast<typename T::value_type>(-1));
	}
	Build_Edges<T, false>::op(edges);
}

/*
 * Build dual-type edges.
 */
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::SE_SE>::op (U const& number) {
	if (number) {}
	return true;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::SE_UE>::op (U const& number) {
	if (number) {}
	return true;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::SS_SL>::op (U const& number) {
	return
		number >= static_cast<U>(std::numeric_limits<T>::min()) &&
		number <= static_cast<U>(std::numeric_limits<T>::max())
	;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::SS_UL>::op (U const& number) {
	return number <= static_cast<U>(std::numeric_limits<T>::max());
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::SL_SS>::op (U const& number) {
	if (number) {}
	return true;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::SL_US>::op (U const& number) {
	if (number) {}
	return true;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::UE_SE>::op (U const& number) {
	if (number) {}
	return true;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::UE_UE>::op (U const& number) {
	if (number) {}
	return true;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::US_SL>::op (U const& number) {
	return
		number >= 0 &&
		number <= static_cast<U>(std::numeric_limits<T>::max())
	;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::US_UL>::op (U const& number) {
	return number <= static_cast<U>(std::numeric_limits<T>::max());
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::UL_SS>::op (U const& number) {
	return number >= 0;
}
template <class T, class U> inline bool Build_EdgesUtil::Number_Fits<T, U, DAC::RelTypes::UL_US>::op (U const& number) {
	if (number) {}
	return true;
}

/*
 * Clean up after combining edges.
 */
template <class T> void Build_EdgesUtil::clean_edges (T& edges) {
	std::sort(edges.begin(), edges.end());
	edges.resize(std::unique(edges.begin(), edges.end()) - edges.begin());
}

