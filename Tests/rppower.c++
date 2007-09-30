/*****************************************************************************
 * rppower.c++
 *****************************************************************************
 * Unit tests for rppower().
 *****************************************************************************/

// Standard includes.
#include <iostream>
#include <vector>

// Internal includes.
#include "demangle.h++"
#include "build_edges.h++"
#include "to_string.h++"

// Test include.
#include "rppower.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************/
// Declarations.

/*****************************************************************************/
// Functions.

// Program entry.
int main ();

// Run the test suite on a given type.
template <class T> static int test ();

// Run the test suite on a combination of two types.
template <class T, class U> static int test2 ();

// Run a signed-sensitive test.
template <class T, class U, bool is_signed_t, bool is_signed_u> class Signed_Sensitive_Test;
template <class T, class U> class Signed_Sensitive_Test<T, U, false, false> { public: static int op (T const base, U const exp); };
template <class T, class U> class Signed_Sensitive_Test<T, U, false, true > { public: static int op (T const base, U const exp); };
template <class T, class U> class Signed_Sensitive_Test<T, U, true , false> { public: static int op (T const base, U const exp); };
template <class T, class U> class Signed_Sensitive_Test<T, U, true , true > { public: static int op (T const base, U const exp); };

/*****************************************************************************/
// Defininions.

/*****************************************************************************/
// Functions.

int main () {
	
	if (test<bool          >()) { return 1; }
	if (test<char          >()) { return 1; }
	if (test<signed char   >()) { return 1; }
	if (test<unsigned char >()) { return 1; }
	if (test<wchar_t       >()) { return 1; }
	if (test<short         >()) { return 1; }
	if (test<unsigned short>()) { return 1; }
	if (test<int           >()) { return 1; }
	if (test<unsigned int  >()) { return 1; }
	if (test<long          >()) { return 1; }
	if (test<unsigned long >()) { return 1; }
	
	return 0;
	
}

/*
 * Run the test suite on a given type.
 */
template <class T> int test () {
	
	{
		string temp;
		cout << "Testing " << demangle<T>(temp) << "...\n";
	}
	
	if (test2<T, bool          >()) { return 1; }
	if (test2<T, char          >()) { return 1; }
	if (test2<T, signed char   >()) { return 1; }
	if (test2<T, unsigned char >()) { return 1; }
	if (test2<T, wchar_t       >()) { return 1; }
	if (test2<T, short         >()) { return 1; }
	if (test2<T, unsigned short>()) { return 1; }
	if (test2<T, int           >()) { return 1; }
	if (test2<T, unsigned int  >()) { return 1; }
	if (test2<T, long          >()) { return 1; }
	if (test2<T, unsigned long >()) { return 1; }
	
	return 0;
	
}

/*
 * Run the test suite on two types.
 */
template <class T, class U> int test2 () {
	
	{
		string temp;
		cout << "  With " << demangle<U>(temp) << "...\n";
	}
	
	vector<T> edges_t;
	vector<U> edges_u;
	
	build_edges(edges_t, edges_u);
	
	for (typename vector<T>::const_iterator base = edges_t.begin(); base != edges_t.end(); ++base) {
		for (typename vector<U>::const_iterator exp = edges_u.begin(); exp != edges_u.end(); ++exp) {
			
			cout << "    " << to_string(*base) << " ** " << to_string(*exp);
			
			if (Signed_Sensitive_Test<T, U, numeric_limits<T>::is_signed, numeric_limits<U>::is_signed>::op(*base, *exp)) {
				return 1;
			}
			
		}
	}
	
	return 0;
	
}

/*
 * Run a signed-sensitive test.
 */
template <class T, class U> int Signed_Sensitive_Test<T, U, false, false>::op (T const base, U const exp) {
	
	T testval       ;
	T controlval = 1;
	
	if (base == 0 && exp == 0) {
		try {
			testval = rppower(base, exp);
		} catch (RPPower::Errors::ZeroZeroth&) {
			cout << "    Zero to the zeroth power.\n";
			return 0;
		} catch (Exception& e) {
			{
				string temp;
				cout << "    Unexpected exception of type " << demangle<Exception>(temp) << ": " << e.what() << "\n";
			}
			return 1;
		} catch (...) {
			cout << "    Unexpected exception of unknown type caught.\n";
			throw;
		}
		{
			string temp;
			cout << "    No " << demangle<RPPower::Errors::ZeroZeroth>(temp) << " error thrown!\n";
		}
		return 1;
	}
	
	for (U count = 0; count != exp; ++count) {
		if (numeric_limits<T>::max() / controlval < base) {
			cout << "    OVERFLOW\n";
			return 0;
		}
		controlval *= base;
	}
	
	cout.flush();
	testval = rppower(base, exp);
	if (testval != controlval) {
		cout << " != " << to_string(testval) << "  FAILED!\n";
		return 1;
	}
	cout << " == " << to_string(testval) << "  OK!\n";
	return 0;
	
}
template <class T, class U> int Signed_Sensitive_Test<T, U, false, true >::op (T const base, U const exp) {
	if (base) {}
	if (exp ) {}
	cout << "\n"; return 0;
}
template <class T, class U> int Signed_Sensitive_Test<T, U, true , false>::op (T const base, U const exp) {
	if (base) {}
	if (exp ) {}
	cout << "\n"; return 0;
}
template <class T, class U> int Signed_Sensitive_Test<T, U, true , true >::op (T const base, U const exp) {
	if (base) {}
	if (exp ) {}
	cout << "\n"; return 0;
}

