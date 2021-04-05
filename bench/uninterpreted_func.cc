#include <SVMA.h>
#include <systemc.h>
#include <math.h>

#include <fstream>
#include<iostream>
#include<bitset>


sc_biguint<32> SVMA::Mult(sc_biguint<32> mult1, sc_biguint<32> mult2) {

  cout << "mult1and2\n";
  cout << mult1 << std::endl;
  cout << "\n";
  cout << mult2 << std::endl;
  cout << "\n";

  unsigned int i = mult1.to_uint();
  unsigned int ii = mult2.to_uint();

  float arg1 = (*(float*)&i);
  float arg2 = (*(float*)&ii);
  

  cout << "arg1and2\n";
  cout << arg1 << " " << std::bitset<32>(arg1) << std::endl;
  cout << "\n";
  cout << arg2 << " " << std::bitset<32>(arg2) << std::endl;
  cout << "\n";


  float rezzy = arg1 * arg2;

  cout << "rezzy\n";
  cout << rezzy;
  cout << "\n";
  
  unsigned int ires = *reinterpret_cast<unsigned int*>(&rezzy);


  cout << "ires\n";
  cout << ires;
  cout << "\n";

  return ires;
}

sc_biguint<32> SVMA::Sub(sc_biguint<32> sub1, sc_biguint<32> sub2) {

  unsigned int sub1_s = sub1.to_uint();
  unsigned int sub2_s = sub2.to_uint();

  float f1 = (*(float*)&sub1_s);
  float f2 = (*(float*)&sub2_s);

  float rezzy = f1 - f2;

  unsigned int ires = *reinterpret_cast<unsigned int*>(&rezzy);


  return ires;

}

sc_biguint<32> SVMA::Add(sc_biguint<32> sub1, sc_biguint<32> sub2) {
  cout << "add1and2\n";
  cout << sub1  << std::endl;
  cout << "\n";
  cout << sub2  << std::endl;
  cout << "\n";

  unsigned int sub1_s = sub1.to_uint();
  unsigned int sub2_s = sub2.to_uint();

  float f1 = (*(float*)&sub1_s);
  float f2 = (*(float*)&sub2_s);

  float rezzy = f1 + f2;

  cout << "rezzy add\n";
  cout << rezzy;
  cout << "\n";
  

  unsigned int ires = *reinterpret_cast<unsigned int*>(&rezzy);

  cout << "ires add\n";
  cout << ires;
  cout << "\n";
  
  return ires;

}

sc_biguint<32> SVMA::Exponent(sc_biguint<32> exponent) {

  unsigned int arg1 = exponent.to_uint();


  float f1 = (*(float*)&arg1);

  // need exponent to be double
  float res = exp(f1);
  unsigned int ires = *reinterpret_cast<unsigned int*>(&res);
  return ires;
}

sc_biguint<32> SVMA::Shift(sc_biguint<32> base, sc_biguint<32> shiftamt) {
  
  unsigned int base_s = base.to_uint();
  unsigned int shiftamt_s = shiftamt.to_uint();  

  unsigned int base_shift = base_s >> shiftamt_s;

  return base_shift;

}

sc_biguint<32> SVMA::MultbyNegativeOne(sc_biguint<32> mult1) {

  unsigned int arg1 = mult1.to_uint();

  float f2 = -1;

  float f1 = (*(float*)&arg1);

  float rezzy = f1 * f2;

  unsigned int result = *reinterpret_cast<unsigned int*>(&rezzy);

  return result;
}

sc_biguint<32> SVMA::Greatest(sc_biguint<32> comp1, sc_biguint<32> comp2) {
  

  unsigned int sub1_s = comp1.to_uint();
  unsigned int sub2_s = comp2.to_uint();

  float f1 = (*(float*)&sub1_s);
  float f2 = (*(float*)&sub2_s);

  sc_bigint<32> out = (f1 > f2) ? 1 : 0;
  sc_biguint<32> result = out;
  return result;

}
