#include <systemc>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"
#include "SVMA.h"

using json = nlohmann::json;

SC_MODULE(Source) {
  sc_in<bool> clk{"clk"};

  sc_out< sc_biguint<32> > SVMA_data_in;
  sc_out< sc_biguint<1> > SVMA_mode;
  sc_out< sc_biguint<32> > SVMA_addr_in;

  sc_out < sc_biguint<1> > input_done;


  SC_CTOR(Source) {
    SC_THREAD(source_input);
    sensitive << clk.pos();
  }

  void source_input() {
    // reset the port
    SVMA_data_in = 0;
    SVMA_mode = 0;
    SVMA_addr_in= 0;

    input_done = 0;
    // read program fragment from file
    std::ifstream fin;
    fin.open("./sim_info/mnist_linear.json", ios::in);
    
    //parse the json file
    json cmd_seq;
    fin >> cmd_seq;

    // pass the command to the ports
    for (int i = 0; i < cmd_seq["program fragment"].size(); i++) {
      SVMA_mode = std::stol(cmd_seq["program fragment"][i]["mode"].get<std::string>(), nullptr, 16);
      std::string addr = cmd_seq["program fragment"][i]["addr"].get<std::string>();
      SVMA_addr_in = std::stol(addr, nullptr, 16);
      // extract each data byte from data
      SVMA_data_in = std::stol(cmd_seq["program fragment"][i]["data"].get<std::string>(), nullptr, 2);
     
      wait(10, SC_NS);
    }

    input_done = 1;
}

};

SC_MODULE(testbench) {
  // SC_HAS_PROCESS(testbench);
  SVMA svma_inst;
  Source src;

  sc_clock clk;

  sc_signal< sc_biguint<1> > SVMA_mode_sig;
  sc_signal< sc_biguint<32> > SVMA_data_in_sig;
  sc_signal< sc_biguint<32> > SVMA_addr_in_sig;

  sc_signal< sc_biguint<1> > input_done;


  SC_CTOR(testbench) :
    clk("clk", 1, SC_NS),
    svma_inst("svma_inst"),
    src("source")
  {
    // binding the signals 
    src.clk(clk);
    src.SVMA_mode(SVMA_mode_sig);
    src.SVMA_addr_in(SVMA_addr_in_sig);
    src.SVMA_data_in(SVMA_data_in_sig);
    src.input_done(input_done);
    

    // // connecting signals to svma
    svma_inst.SVMA_mode_in(SVMA_mode_sig);
    svma_inst.SVMA_addr_in_in(SVMA_addr_in_sig);
    svma_inst.SVMA_data_in_in(SVMA_data_in_sig);

    svma_inst.instr_log;

 
    SC_THREAD(run);
  }

  void run() {
    svma_inst.instr_log.open("./sim_info/instr_log_svma.txt", ofstream::out | ofstream::trunc);
    svma_inst.instr_update_log.open("./sim_info/instr_update.log", ios::out | ios::trunc);

    std::cout << "start running" << std::endl;
    std::cout << "*********** simulation start ***********" << std::endl;
    wait(10, SC_NS);

    while (input_done == 0) {
		  std::cout << "current simulation time: " << '\t' << sc_time_stamp() << "\r" << std::flush;
      wait(10, SC_NS);
    }

    wait(100000, SC_NS);
    std::cout << '\n' << std::endl;
    std::cout << "************* sc_stop **************" << std::endl;
    svma_inst.instr_log.close();
    // make this more readable
    std::cout << "output: " << std::endl;
    std::cout << svma_inst.SVMA_output << std::endl;
    std::cout << "score: " << std::endl;
    std::cout << svma_inst.SVMA_score << std::endl;


	
    std::cout << "\ntestbench done" << std::endl;

    sc_stop();
  }
};

int sc_main(int argc, char *argv[]) {
  std::cout << "test start" << std::endl;
  testbench tb("tb");
  sc_start();

  return 0;
}
