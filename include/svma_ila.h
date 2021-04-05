#ifndef SVMA_ILA_H__
#define SVMA_ILA_H__

#include <ilang/ilang++.h>
#include <string>
#include <ilang/util/log.h>


namespace ilang {
namespace svma {


Ila GetSVMAIla(const std::string& model_name = "svma");

void DefineLinearChild(Ila& m);
void DefineLinearReformChild(Ila& m);
void DefineTwoPolyChild(Ila& m);
void DefineTwoPolyReformChild(Ila& m);
void DefineFourPolyChild(Ila& m);
void DefineRBFChild(Ila& m);

}; // namespace max

}; // namespace ilang
#endif // MAX_TOP_H__