#include <SVMA.h>
#include <systemc.h>
#include <math.h>

#include <fstream>
#include<iostream>


sc_biguint<32> SVMA::Mult(sc_biguint<32> mult1, sc_biguint<32> mult2) {

  sc_bigint<32> arg1 = mult1.to_int();
  sc_bigint<32> arg2 = mult2.to_int();

  float f1 = (*(float*)&arg1);
  float f2 = (*(float*)&arg2);

  float rezzy = f1 * f2;
  cout << "rezzy\n";
  cout << rezzy;
  
  
  //sc_bigint<32> ires = arg1 * arg2;

  int ires = *(int*)&rezzy;
  
  //sc_bigint<32> res = ires;
  //sc_biguint<32> result = res;
  return ires;
}

sc_biguint<32> SVMA::Sub(sc_biguint<32> sub1, sc_biguint<32> sub2) {

  sc_bigint<32> sub1_s = sub1;
  sc_bigint<32> sub2_s = sub2;

  float f1 = (*(float*)&sub1_s);
  float f2 = (*(float*)&sub2_s);

  float rezzy = f1 - f2;

  int ires = *(int*)&rezzy;
  sc_bigint<32> res = ires;

  //sc_bigint<32> out = sub1_s - sub2_s;
  //sc_bigint<32> result_s = res.to_int();
  sc_biguint<32> result = res;

  return result;

}

sc_biguint<32> SVMA::Exponent(sc_biguint<32> exponent) {

  sc_bigint<32> arg1 = exponent.to_int();

  double arg0 = arg1.to_double();

  // need exponent to be double
  double res = exp(arg0);
  
  sc_bigint<32> result_s = res;
  sc_biguint<32> result = result_s;

  return result;
}

sc_biguint<32> SVMA::Shift(sc_biguint<32> base, sc_biguint<32> shiftamt) {
  
  sc_bigint<32> base_s = base;
  sc_bigint<32> shiftamt_s = shiftamt;  

  sc_bigint<32>  base_shift = base_s >> shiftamt_s;
  sc_biguint<32> result = base_shift;

  return result;
}

sc_biguint<32> SVMA::MultbyNegativeOne(sc_biguint<32> mult1) {

  sc_bigint<32> arg1 = mult1.to_int();
  sc_bigint<32> arg2 = -1;

  sc_bigint<32> out = arg1 * arg2;
  sc_biguint<32> result = out;

  return result;
}

sc_biguint<32> SVMA::Greatest(sc_biguint<32> comp1, sc_biguint<32> comp2) {
  
  sc_bigint<32> comp1_s = comp1;
  sc_bigint<32> comp2_s = comp2;
  sc_bigint<32> out = (comp1_s > comp2_s) ? 1 : 0;
  sc_biguint<32> result = out;
  return result;

}
