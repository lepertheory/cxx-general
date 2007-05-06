/*****************************************************************************
 * Exception.c++
 *****************************************************************************
 * Unit tests for Exception class.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <iostream>

// Testing include.
#include <Exception.h++>

// Bring in namespaces.
using namespace std;
using namespace DAC;

/*****************************************************************************
 * TestException
 *****************************************************************************
 * Test Exception class polymorphism, inheritance, etc.
 *****************************************************************************/
class TestException : public Exception {
  
  /*
   * Public members.
   */
  public:
    
    /*************************************************************************/
    // Function members.
    
    // Constructor.
    TestException () : Exception() {};
    
    // Destructor.
    ~TestException () throw() {};
    
    // Get the cause of this error.
    virtual char const* what () const throw() { return "Test exception."; };
    
    // Reset to just-constructed state.
    virtual TestException& clear () { Exception::clear(); return *this; };
  
  /*
   * Private members.
   */
  private:
  
};

/*
 * This is it.
 */
int main () {
  
  // Test the default constructor. As long as it constructs, no problem.
  cout << "Testing default constructor...";
  cout << "  Exception... ";
  {
    Exception test;
    cout << "OK!" << endl;
  }
  cout << "  TestException... ";
  {
    TestException test;
    cout << "OK!" << endl;
  }
  
  // Test the destructor. As long as it destructs, no problem.
  cout << "Testing destructor..." << endl;
  cout << "  Exception... ";
  {
    Exception test;
  }
  cout << "OK!" << endl;
  cout << "  TestException... ";
  {
    TestException test;
  }
  cout << "OK!" << endl;
  
  // Test what(). Should output exactly what we put in.
  cout << "Testing what()... " << endl;
  cout << "  Exception... ";
  {
    Exception test;
    cout << test.what() << " ";
    if (test.what() == "Undefined error.") {
      cout << "OK!" << endl;
    } else {
      cout << "FAILURE!" << endl;
      return 1;
    }
  }
  cout << "  TestException... ";
  {
    TestException test;
    cout << test.what() << " ";
    if (test.what() == "Test exception.") {
      cout << "OK!" << endl;
    } else {
      cout << "FAILURE!" << endl;
      return 1;
    }
  }
  
  // Test that type() returns the type correctly.
  cout << "Testing type(std::string&)..." << endl;
  cout << "  Testing Exception... ";
  {
    string buffer;
    {
      Exception test;
      cout << test.type(buffer) << " ";
      if (buffer == "DAC::Exception") {
        cout << "OK!" << endl;
      } else {
        cout << "FAILURE!" << endl;
        return 1;
      }
    }
    cout << "  Testing TestException... ";
    {
      TestException test;
      cout << test.type(buffer) << " ";
      if (test.type(buffer) == "TestException") {
        cout << "OK!" << endl;
      } else {
        cout << "FAILURE!" << endl;
        return 1;
      }
    }
  }
  cout << "Testing type()..." << endl;
  cout << "  Testing Exception... ";
  {
    Exception test;
    cout << test.type() << " ";
    if (test.type() == "DAC::Exception") {
      cout << "OK!" << endl;
    } else {
      cout << "FAILURE!" << endl;
      return 1;
    }
  }
  cout << "  Testing TestException... ";
  {
    TestException test;
    cout << test.type() << " ";
    if (test.type() == "TestException") {
      cout << "OK!" << endl;
    } else {
      cout << "FAILURE!" << endl;
      return 1;
    }
  }
  
  // Test buffer_message().
  cout << "Testing buffer_message()..." << endl;
  {
    Exception test;
    cout << "  Simple text constant... ";
    {
      string test_message("Simple text.");
      if (string(test.buffer_message(test_message)) == "Simple text.") {
        cout << "OK!" << endl;
      } else {
        cout << "FAILURE!" << endl;
        return 1;
      }
    }
    cout << "  Modified text... ";
    {
      string test_message("Unmodified!");
      char const* test_ptr = test.buffer_message(test_message);
      test_message = "Modified!";
      if (string(test_ptr) == "Unmodified!") {
        cout << "OK!" << endl;
      } else {
        cout << "FAILURE!" << endl;
        return 1;
      }
    }
    cout << "  Out of scope text... ";
    {
      char const* test_ptr;
      {
        string test_message("Scope defeated!");
        test_ptr = test.buffer_message(test_message);
        test_message = "I suck!";
      }
      if (string(test_ptr) == "Scope defeated!") {
        cout << "OK!" << endl;
      } else {
        cout << "FAILURE!" << endl;
        return 1;
      }
    }
  }
  
  // All tests passed.
  cout << "OK!" << endl;
  return 0;
  
}

