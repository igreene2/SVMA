#ifndef UNINTERPRETED_FUNC_H__
#define UNINTERPRETED_FUNC_H__

#include <ilang/ilang++.h>
#include <vector>

namespace ilang {
namespace svma {


static auto signed_type = SortRef::BV(32);
static auto comp1 = SortRef::BV(32);
static auto comp2 = SortRef::BV(32);

static FuncRef Mult("Mult", signed_type, comp1, comp2);
static FuncRef Sub("Sub", signed_type, comp1, comp2);
static FuncRef Add("Add", signed_type, comp1, comp2);
static FuncRef Exponent("Exponent", signed_type, comp1);
static FuncRef MultbyNegativeOne("MultbyNegativeOne", signed_type, comp1);
static FuncRef Shift("Shift", signed_type, comp1, comp2);
static FuncRef Greatest("Greatest", signed_type, comp1, comp1);


} // namespace ilang
} // namespace max

#endif