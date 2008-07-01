/*****************************************************************************
 * testcommon.h++
 *****************************************************************************
 * Commonly-used testing routines.
 *****************************************************************************/

// Include guard.
#if !defined(testcommon_9834huj9HU98hu089h3847uba7d)
	#define testcommon_9834huj9HU98hu089h3847uba7d

// STL includes.
//#include <vector>
#include <limits>

// Internal includes.
#include "NumInfo.h++"

// Namespace.
namespace DAC {
	
	/***************************************************************************/
	// Types.
	
	/*
	 * Create edge cases, single type.
	 */
	template <class T, bool is_signed> class Build_Edges;
	template <class T> class Build_Edges<T, false> { public: static void op (T& edges); };
	template <class T> class Build_Edges<T, true > { public: static void op (T& edges); };
	
	/*
	 * Create edge cases, two types.
	 */
	template <class T, class U, RelTypes::Type> class Build_Edges_Dual;
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::UE_UE> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::UL_US> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::US_UL> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::UE_SE> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::UL_SS> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::US_SL> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::SE_UE> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::SL_US> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::SS_UL> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::SE_SE> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::SL_SS> { public: static void op (T& edges_1, T& edges_2); };
	template <class T, class U> class Build_Edges_Dual<T, U, RelTypes::SS_SL> { public: static void op (T& edges_1, T& edges_2); };
	
	/***************************************************************************/
	// Functions.
	
	// Build edges, single type.
	template <class T> void build_edges (T& edges);
	
	// Build edges, dual type.
	template <class T, class U> void build_edges_dual (T& edges1, U& edges2);
	
	/***************************************************************************
	 * Inline and template definitions.
	 ***************************************************************************/
	
	/***************************************************************************/
	// Function members.
	
	/*
	 * Create edge cases, single type, unsigned.
	 */
	template <class T> void Build_Edges<T, false>::op (T& edges) {
		edges.push_back(0);
		edges.push_back(1);
		if (std::numeric_limits<typename T::value_type>::max() > 1) {
			if (std::numeric_limits<typename T::value_type>::max () > 2) {
				edges.push_back(std::numeric_limits<typename T::value_type>::max() - 1);
			}
			edges.push_back(std::numeric_limits<typename T::value_type>::max());
		}
	}
	
	/*
	 * Create edge cases, single type, signed.
	 */
	template <class T> void Build_Edges<T, true>::op (T& edges) {
		if (std::numeric_limits<typename T::value_type>::min() < -1) {
			edges.push_back(std::numeric_limits<typename T::value_type>::min());
			if (std::numeric_limits<typename T::value_type>::min() < -2) {
				edges.push_back(std::numeric_limits<typename T::value_type>::min() + 1);
			}
		}
		edges.push_back(-1);
		Build_Edges<T, false>::op(edges);
	}
	
	/***************************************************************************/
	// Functions.
	
	/*
	 * Create edge cases, single type.
	 */
	template <class T> inline void build_edges (T& edges) {
		Build_Edges<T, NumType<typename T::value_type>::type>::op(edges);
	}
	
}

// End include guard.
#endif
