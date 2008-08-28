/*****************************************************************************
 * AutoArray.c++
 *****************************************************************************
 * Unit tests for AutoArray.
 *****************************************************************************/

// Standard includes.
#include <iostream>
#include <cstdlib>
#include <memory>
#include <new>

// Internal includes.
#include "testcommon.h++"

// Testing includes.
#include "AutoArray.h++"

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************/
// Types.

// A memory allocation.
struct Allocation {
	Allocation () : p(0), size(0), deleted(false) {};
	void*  p      ;
	size_t size   ;
	bool   deleted;
};

/*****************************************************************************/
// Function declarations.

// Program entry.
int main ();

/*****************************************************************************/
// Global variables.

// Track allocations.
Allocation* g_allocations    = 0;
size_t      g_numalloc       = 0;
size_t      g_alloctracksize = 0;
size_t      g_numdeletions   = 0;

/*****************************************************************************/
// Function definitions.

/*
 * Override new so we can track allocations.
 */
void* operator new [] (size_t size) throw(std::bad_alloc) {
	
	// Do the allocation.
	void* retval = malloc(size);
	if (retval == 0) {
		throw std::bad_alloc();
	}
	
	// Track this allocation.
	g_allocations = reinterpret_cast<Allocation*>(realloc(g_allocations, g_alloctracksize + sizeof(Allocation)));
	if (g_allocations == 0) {
		cerr << "Could not allocate memory for allocation tracking of allocation number " << g_numalloc + 1 << ".\n";
		throw std::bad_alloc();
	}
	new (g_allocations + g_numalloc) Allocation;
	++g_numalloc;
	g_alloctracksize += sizeof(Allocation);
	g_allocations[g_numalloc].p       = retval;
	g_allocations[g_numalloc].size    = size  ;
	g_allocations[g_numalloc].deleted = false ;
	
	return retval;
	
}

/*
 * Override delete so we can track deallocations.
 */
void operator delete [] (void* p) throw() {
	
	Allocation* thisalloc;
	
	++g_numdeletions;
	
	// Make sure this is a correct deallocation.
	bool untracked  = true;
	bool doublefree = true;
	for (size_t i = 0; i != g_numalloc; ++i) {
		
		// Check for deleting an untracked pointer.
		if (g_allocations[i].p == p) {
			untracked = false;
			
			// If we find one that wasn't deleted, this isn't a double free.
			if (!g_allocations[i].deleted) {
				doublefree = false;
				thisalloc  = &g_allocations[i];
				break;
			}
			
		}
		
	}
	if (untracked) {
		cerr << "Deleting an untracked pointer.\n";
		exit(EXIT_FAILURE);
	}
	if (doublefree) {
		cerr << "Double free attempted.\n";
		exit(EXIT_FAILURE);
	}
	
	// Finally, free it.
	free(p);
	thisalloc->deleted = true;
	
}

/*
 * Program entry.
 */
int main () {
	
	cout << "Testing AutoArray<char>...\n";
	
	// Run the test battery on a null pointer.
	test(0);
	
	// Run the test battery on 1,000 random pointers. These must all be valid,
	// operation on invalid pointers is undefined.
	for (size_t count = 0; count != 1000; ++count) {
		test(
	
	// Test default constructor and zeroed operation.
	{
		
		// Check for proper construction.
		cout << "  Testing default constructor...\n";
		AutoArray<char> testarray;
		cout << "    Checking that _p is 0... ";
		if (testarray._p == 0) {
			cout << "OK!\n";
		} else {
			cout << "FAILED!\n";
			return EXIT_FAILURE;
		}
		cout << "  OK!\n";
		
		// Check that a reset to 0 doesn't try to free. Normally it would try to
		// free a 0 pointer, but we're resetting to the ame thing. reset() and
		// reset(0) should be the same. Our custom free will let us know if
		// there's a problem.
		{
			cout << "  Testing reset()... ";
			size_t deletions = g_numdeletions;
			testarray.reset();
			if (deletions == g_numdeletions) {
				cout << "OK!\n";
			} else {
				cout << "FAILED!\n";
				return EXIT_FAILURE;
			}
			cout << "  Testing reset(0)... ";
			testarray.reset(0);
			if (deletions == g_numdeletions) {
				cout << "OK!\n";
			} else {
				cout << "FAILED!\n";
				return EXIT_FAILURE;
			}
		}
	
	// Falling out of scope will cause destructor to run, custom delete will
	// catch any extraneous deletions.
	}
	
	
	
	// All tests were successful.
	cout << "OK!\n";
	return EXIT_SUCCESS;
	
}

