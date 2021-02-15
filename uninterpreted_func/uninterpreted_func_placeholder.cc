#include <SVMA.h>
#include <systemc.h>
#include <ap_int.h>
#include <nvhls_int.h>
#include <nvhls_types.h>
#include <nvhls_vector.h>
#include <nvhls_module.h>

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_math.h>
#include <ac_math/ac_sigmoid_pwl.h>
#include <ac_math/ac_tanh_pwl.h>
#include <ac_math/ac_div.h>
#include <ac_math/ac_pow_pwl.h>
#include <ac_math/ac_inverse_sqrt_pwl.h>
#include "SM6Spec.h"
#include "AdpfloatUtils.h"
#include "AdpfloatSpec.h"

#include <fstream>


sc_biguint<16> SVMA::Mult(sc_biguint<16> mult1, sc_biguint<16> mult2) {

  return 0;
}

sc_biguint<16> SVMA::Sub(sc_biguint<16> sub1, sc_biguint<16> sub2) {

  return 0;
}

sc_biguint<16> SVMA::Exponent(sc_biguint<16> exponent) {

  return 0;
}

sc_biguint<16> SVMA::MultbyNegativeOne(sc_biguint<16> mult1) {

  return 0;
}

sc_biguint<16> SVMA::Shift(sc_biguint<16> base, sc_biguint<16> shiftamt) {
  return 0;
}