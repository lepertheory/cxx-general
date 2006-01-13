// STL includes.
#include <iostream>

// Testing include.
#include "GetOpt.h++"

// Namespace declarations.
using namespace std;
using namespace DAC;

// This is it.
int main (int argc, char** argv) {
  
  GetOpt test(argc, argv);
  test.add_option(GetOpt::Option('h', "help"  , GetOpt::ARG_NONE,     false, "Show this help screen.", ""));
  test.add_option(GetOpt::Option('o', "output", GetOpt::ARG_REQUIRED, false, "Some output option. This is a really long description, it just goes on and on forever, like it doesn't know how to shut up. The argument name is \"%s\". Here's a %%s.", "FARTY"));
  test.add_option(GetOpt::Option('b', "bling" , GetOpt::ARG_OPTIONAL, false, "Bling it out, yo. %s is optional, dog.", "HIZZLE"));
  test.add_option(GetOpt::Option('w', "width" , GetOpt::ARG_REQUIRED, false, "This one is for real. %s is the width of the help text to output. Defaults to 80.", "WIDTH"));
  
  cout << "set: " << test.isSet('o') << endl;
  
  test.ProgramName(argv[0])
      .Usage      ("[OPTION]...")
      .Description("This is the description. It has a really long line to test out my text wrapping function. Woo. Here tab\tlet's see if it gets this right. I be surprised if it does. Now let's try out some really long words. 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890 How about that? Rock on brother beavis, I kick ass. This isn't long enough yet, let's make it longer. The program name is %s, I'll bet you didn't know that.")
      .PostInfo   ("I guess this goes at the end.")
      .BugAddress ("cxxgeneral@lepertheory.net");
      .HelpWidth  (test.isSet('w') ? test.getArg;
  
  cout << "Help:\n" << test.getHelp();
  
  return 0;
  
}

