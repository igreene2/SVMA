// File: main.cc

#include <svma_ila.h>
#include <fstream>
#include <ilang/util/log.h>


using namespace ilang;

int main() {
  SetToStdErr(1);
  // get the ILA model
  auto svm = svma::GeSVMAIla("svmA");
  std::cout << "successfully constructed svma model\n";

  ILA_INFO << "#instr: " << svm.instr_num();
  ILA_INFO << "#input: " << svm.input_num();
  ILA_INFO << "#state: " << svm.state_num();
  for (auto i = 0; i < svm.instr_num(); i++) {
    ILA_INFO << "instr." << i << " " << svm.instr(i);
  }

  std::cout << "successfully printed state stuff\n";

  ExportSysCSim(svm, "./sim_model", false);

  // // example - export ILA to Verilog
  // ILA_INFO << "Export " << hlscnn << " to hlscnn.v";
  // std::ofstream fw("hlscnn.v");
  // hlscnn.ExportToVerilog(fw);
  // fw.close();

  return 0;
}