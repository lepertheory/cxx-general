/*****************************************************************************
 * build_edges.h++
 *****************************************************************************
 * Provides commonly-used edge cases.
 *****************************************************************************/

// Standard includes.
#include <limits>

/*****************************************************************************/
// Functions.

// Build standard single-type edges.
template <class T> void build_edges (T& edges);

/*****************************************************************************/
// Utilities.
namespace Build_EdgesUtil {
	
	// Build standard single-type edges.
	template <class T, bool is_signed> class Build_Edges;
	template <class T> class Build_Edges<T, false> { public: static void op (T& edges); };
	template <class T> class Build_Edges<T, true > { public: static void op (T& edges); };
	
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

