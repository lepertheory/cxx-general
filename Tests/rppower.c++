/*****************************************************************************
 * rppower.c++
 *****************************************************************************
 * Unit tests for rppower().
 *****************************************************************************/

// Standard includes.
#include <iostream>
#include <vector>
#include <cmath>

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
template <class T, class U, bool test_is_sufficient, bool is_signed_u> class Signed_Sensitive_Test;
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
		cout << "Testing " << demangle<T>(temp) << "..." << endl;
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
		cout << "  With " << demangle<U>(temp) << "..." << endl;
	}
	
	vector<T> edges_t;
	vector<U> edges_u;
	
	build_edges(edges_t, edges_u);
	
	for (typename vector<T>::const_iterator base = edges_t.begin(); base != edges_t.end(); ++base) {
		for (typename vector<U>::const_iterator exp = edges_u.begin(); exp != edges_u.end(); ++exp) {
			
			cout << "    " << to_string(*base) << " ** " << to_string(*exp);
			
			if (Signed_Sensitive_Test<T, U,
				numeric_limits<long double>::digits >= numeric_limits<T>::digits &&
				numeric_limits<long double>::digits >= numeric_limits<U>::digits,
				numeric_limits<U>::is_signed>::op(*base, *exp)
			) {
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
	if (base) {}
	if (exp ) {}
	cout << "        Insufficient digits in test type.";
	return 1;
}
template <class T, class U> int Signed_Sensitive_Test<T, U, false, true >::op (T const base, U const exp) {
	if (base) {}
	if (exp ) {}
	cout << "        Insufficient digits in test type.";
	return 1;
}
template <class T, class U> int Signed_Sensitive_Test<T, U, true, false>::op (T const base, U const exp) {
	
	long double controlval;
	
	if (base == 0) {
		controlval = 0.0L;
	} else if (exp == 0) {
		controlval = 1.0L;
	} else {
		controlval = powl(static_cast<long double>(base), static_cast<long double>(exp));
	}
	
	T testval;
	try {
		testval = rppower(base, exp);
	} catch (RPPower::Errors::ZeroZeroth&) {
		if (base == 0 && exp == 0) {
			cout << "       Zero to the zeroth power." << endl;
			return 0;
		} else {
			cout << "       Unexpected zero to the zeroth power exception." << endl;
			return 1;
		}
	} catch (RPPower::Errors::Overflow&) {
		if (
			controlval > static_cast<long double>(numeric_limits<T>::max()) ||
			controlval < static_cast<long double>(numeric_limits<T>::min())
		) {
			cout << "       OVERFLOW" << endl;
			return 0;
		} else {
			cout << " == " << to_string(static_cast<T>(controlval)) << " Unexpected overflow exception." << endl;
			return 1;
		}
	} catch (Exception& e) {
		{
			string temp;
			cout << "       Unexpected exception of type " << demangle<Exception>(temp) << ": " << e.what() << endl;
		}
		return 1;
	} catch (...) {
		cout << "       Unexpected exception of unknown type caught." << endl;
		throw;
	}
	
	if (base == 0 && exp == 0) {
		cout << "       Missed zero to the zeroth power!" << endl;
		return 1;
	}
	if (
		controlval > static_cast<long double>(numeric_limits<T>::max()) ||
		controlval < static_cast<long double>(numeric_limits<T>::min())
	) {
		cout << " == " << controlval << " Missed overflow!" << endl;
		return 1;
	}
	
	if (testval != static_cast<T>(controlval)) {
		cout << " != " << to_string(testval) << "  FAILED!" << endl;
		return 1;
	}
	
	cout << " == " << to_string(testval) << "  OK!" << endl;
	return 0;
	
}
template <class T, class U> int Signed_Sensitive_Test<T, U, true , true >::op (T const base, U const exp) {
	return Signed_Sensitive_Test<T, U, true, false>::op(base, exp);
}

