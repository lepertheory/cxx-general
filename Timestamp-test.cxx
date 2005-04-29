/*****************************************************************************
 * Timestamp-test.cxx
 *****************************************************************************
 * Tests the class Timestamp.
 *****************************************************************************/

// STL includes.
#include <iostream>

// Internal includes.
#include "demangle.hxx"
#include "Exception.hxx"

// Testing include.
#include "Timestamp.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// This is where it all happens.
int main (int argc, char** argv, char** envp) {
  
  try {
    
    Timestamp test1;
    
    test1.getSystemTime();
    
    cout << "Current:" << endl
         << "  Julian: " << test1.Julian() << endl;
    
    Timestamp test2;
    
    typedef Timestamp::TimeVal TV;
    
    TV s, n, h, d, m, y;
    
    cout << "Year:   "; cin >> y;
    cout << "Month:  "; cin >> m;
    cout << "Day:    "; cin >> d;
    cout << "Hour:   "; cin >> h;
    cout << "Minute: "; cin >> n;
    cout << "Second: "; cin >> s;
    
    test2.set(Timestamp::Interval().Year(y).Month(m).Day(d).Hour(h).Minute(n).Second(s));
    
    cout << "Julian: " << test2.Julian() << endl;
    
    return 0;
    
  } catch (Exception& e) {
    
    cout << "Exception (" << e.type() << "): " << e << endl;
    exit(1);
    
  } catch (exception& e) {
    
    cout << "Exception (" << demangle(e) << "): " << e.what() << endl;
    exit(1);
    
  } catch (...) {
    
    cout << "Unexpected exception caught." << endl;
    exit(1);
    
  }
  
};
