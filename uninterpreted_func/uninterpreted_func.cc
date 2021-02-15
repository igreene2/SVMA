#include <SVMA.h>
#include <systemc.h>
#include <math.h>

#include <fstream>


sc_biguint<16> SVMA::Mult(sc_biguint<16> mult1, sc_biguint<16> mult2) {

  sc_bigint<16> arg1 = mult1.to_int();
  sc_bigint<16> arg2 = mult2.to_int();

  float f1 = (*(float*))&arg1);
  float f2 = (*(float*))&arg2);

  float res = f1 * f2;
  
  sc_bigint<16> result_s = res.to_int();
  sc_biguint<16> result = result_s;
}

sc_biguint<16> SVMA::Sub(sc_biguint<16> sub1, sc_biguint<16> sub2) {

  sc_bigint<16> sub1_s = sub1;
  sc_bigint<16> sub2_s = sub2;

  sc_bigint<16> out = sub1_s - sub2_s;
  sc_bigint<32> result_s = out.to_int();
  sc_biguint<32> result = result_s;

  return result;

}

sc_biguint<16> SVMA::Exponent(sc_biguint<16> exponent) {

  sc_bigint<16> arg1 = exponent.to_int();

  float f1 = (*(float*))&arg1);
  float f2 = (2.7182818284590452353602874713527);

  float res = powf(f2, f1);
  
  sc_bigint<16> result_s = res.to_int();
  sc_biguint<16> result = result_s;

  return result;
}

sc_biguint<16> SVMA::Shift(sc_biguint<16> base, sc_biguint<16> shiftamt) {
  int arg_0_int = arg_0.to_int();
  int arg_1_int = arg_1.to_int();

  int base_shift = base >> shiftamt;
  sc_biguint<16> result = base_shift;

  return result;
}

sc_biguint<16> SVMA::MultbyNegativeOne(sc_biguint<16> mult1) {

  sc_bigint<16> arg1 = mult1.to_int();
  sc_bigint<16> arg2 = -1;

  sc_bigint<16> out = arg1 * arg2;
  sc_biguint<32> result = result_s;

  return result;
}
