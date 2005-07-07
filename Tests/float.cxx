#include <iostream>
#include <cmath>
#include "toString.hxx"

using namespace std;
using namespace DAC;

struct _Float_Bits {
  unsigned int mantissa : 23;
  unsigned int exponent :  8;
  unsigned int sign     :  1;
};

struct _Double_Bits {
  unsigned int mantissal;
  unsigned int mantissah : 20;
  unsigned int exponent  : 11;
  unsigned int sign      :  1;
};

struct _Long_Double_Bits {
  unsigned int mantissal;
  unsigned int mantissah : 31;
  unsigned int j         :  1;
  unsigned int exponent  : 15;
  unsigned int sign      :  1;
  unsigned int           : 16;
};

union _Float_Parts {
  float       number;
  _Float_Bits bits;
};

union _Double_Parts {
  double       number;
  _Double_Bits bits;
};

union _Long_Double_Parts {
  long double       number;
  _Long_Double_Bits bits;
};

int main () {
  
  float       thefloat      = 0;
  double      thedouble     = 0;
  long double thelongdouble = 0;
  
  cout << "Enter a float      : "; cin >> thefloat;
  cout << "Enter a double     : "; cin >> thedouble;
  cout << "Enter a long double: "; cin >> thelongdouble;
  
  int         expfloat       = 0;
  int         expdouble      = 0;
  int         explongdouble  = 0;
  float       normfloat      = frexp(thefloat,      &expfloat);
  double      normdouble     = frexp(thedouble,     &expdouble);
  long double normlongdouble = frexp(thelongdouble, &explongdouble);
  
  cout << "float:       " << toString(thefloat)      << " " << toString(normfloat)      << " " << expfloat      << endl;
  cout << "double:      " << toString(thedouble)     << " " << toString(normdouble)     << " " << expdouble     << endl;
  cout << "long double: " << toString(thelongdouble) << " " << toString(normlongdouble) << " " << explongdouble << endl;
  
  _Float_Parts       partfloat;
  _Double_Parts      partdouble;
  _Long_Double_Parts partlongdouble;
  
  partfloat.number      = thefloat;
  partdouble.number     = thedouble;
  partlongdouble.number = thelongdouble;
  cout << "partfloat     : " << partfloat     .bits.sign << " " << partfloat     .bits.exponent << " " << partfloat     .bits.mantissa  << endl;
  cout << "partdouble    : " << partdouble    .bits.sign << " " << partdouble    .bits.exponent << " " << partdouble    .bits.mantissah << "|" << partdouble    .bits.mantissal << endl;
  cout << "partlongdouble: " << partlongdouble.bits.sign << " " << partlongdouble.bits.exponent << " " << partlongdouble.bits.mantissah << "|" << partlongdouble.bits.mantissal << endl;
  
  return 0;
  
}
