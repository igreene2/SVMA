/// \file the ila example of svma accelerator: 
/// support vector machine accelerator for biomedical purposes,
//  receives MMIO instruction
/// Isabel Greene (igreene@princeton.edu)
///

#include <ilang/ilang++.h>
#include <max_ila.h>
#include <ilang/util/log.h>

namespace ilang {
namespace svma {

    Ila GetSVMAIla(const std::string& model_name) {
            // construct the model
            auto m = ilang::Ila("SVMA");
            std::cout << "made svma\n";

            // inputs
            // 0 - read, 1 - write
            m.NewBvInput("mode", 1); // do i need this?
            m.NewBvInput("addr_in", 16);
            m.NewBvInput("data_in", 16);

            // internal arch state 
            m.NewBvState("base_addr_sv_L", 16);
            m.NewBvState("base_addr_sv_H", 16);
            m.NewBvState("base_addr_tv_L" , 16);
            m.NewBvState("base_addr_tv_H" , 16);
            m.NewBvState("tau" , 16);
            m.NewBvState("c" , 16);
            m.NewBvState("b" , 16);
            m.NewBvState("fv_dim" , 16);
            m.NewBvState("num_sv" , 16);
            m.NewBvState("th" , 16);
            m.NewBvState("shift1" , 8);
            m.NewBvState("shift2", 8);
            m.NewBvState("shift3", 8);
            m.NewBvState("cmd_bits" , 16);

            // the memory: 160 bytes - how big actually is this?
            m.NewMemState("mem", 16, 32);

            // the output
            m.NewBvState("score", 32); 
            // remember to write back this to the address of score

            // internal state
            m.NewBvState("run_svma", 1);
            m.NewBvState("interrupt_enable", 1); // do i need this?
            m.NewBvState("reformulation", 1);
            m.NewBvState("kernel", 2);
            m.NewBvState("order_poly", 1);
            m.NewBvState("output", 1);
            m.NewBvState("done", 1);
            m.NewBvState("child_state", 2);
        
            m.AddInit(m.state("child_state") == 0);
            
            // remember to write back done and output to the address in cmd instruction!
        
            std::cout << "declared all the state\n";
            m.SetValid(m.input("addr_in") > 0x01A0 & m.input("addr_in") < 0x01BA);
            std::cout << "did the valid\n";


        { // SVM_SV_BASEADDR_H
            std::cout << "inside SVM_SV_BASEADDR_H\n";
            auto instr = m.NewInstr("SVM_SV_BASEADDR_H");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01A2));

            instr.SetUpdate(m.state("base_addr_sv_H"), m.input("data_in")); // update a start_addr

            

        }


        { // SVM_SV_BASEADDR_L
            std::cout << "inside  SVM_SV_BASEADDR_L\n";
            auto instr = m.NewInstr("SVM_SV_BASEADDR_L");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01A4)); 

            instr.SetUpdate(m.state("base_addr_sv_L"), m.input("data_in"));
            

        }


        { // SVM_TV_BASEADDR_H
            std::cout << "inside  SVM_TV_BASEADDR_H\n";
            auto instr = m.NewInstr("SVM_TV_BASEADDR_H");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01A6)); 

            instr.SetUpdate(m.state("base_addr_tv_H"), m.input("data_in"));
            

        }

        { // SVM_TV_BASEADDR_L
            std::cout << "inside  SVM_TV_BASEADDR_L\n";
            auto instr = m.NewInstr("SVM_TV_BASEADDR_L");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01A8)); 

            instr.SetUpdate(m.state("base_addr_tv_L"), m.input("data_in"));
            

        }


        { // SVM_GAMMA (tau)
            std::cout << "inside  SVM_GAMMA \n";
            auto instr = m.NewInstr("SVM_GAMMA");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01AA)); // is addr_in meant to be a decode thing?
            // what happens if this instruction is first? need some guarantee about which of these is first
            // maybe and if then else in both? 
            // might also just need to store as the size they come and then concat later?
            // can i put data_in in something that isn't the same size?? like if its bigger than what I want from it? 
            instr.SetUpdate(m.state("tau"), m.input("data_in"));
            

        }


        { // SVM_C
            std::cout << "inside  SVM_C\n";
            auto instr = m.NewInstr("SVM_C");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01AC)); 

            instr.SetUpdate(m.state("c"), m.input("data_in"));
            

        }


        { // SVM_B
            std::cout << "inside  SVM_B\n";
            auto instr = m.NewInstr("SVM_B");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01AE)); 

            instr.SetUpdate(m.state("b"), m.input("data_in"));
            

        }

        { // SVM_FV_DIM
            std::cout << "inside SVM_FV_DIM\n";
            auto instr = m.NewInstr("SVM_FV_DIM");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01B0)); // 
   
            instr.SetUpdate(m.state("fv_dim"), m.input("data_in"));
            

        }


        { // SVM_NUMSV
            std::cout << "inside  SVM_NUMSV\n";
            auto instr = m.NewInstr("SVM_NUMSV");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01B2)); 

            instr.SetUpdate(m.state("num_sv"), m.input("data_in"));
            

        }
        

        { // SVM_SHIFT12
            std::cout << "inside  SVM_SHIFT12\n";
            auto instr = m.NewInstr("SVM_SHIFT12");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01B4)); 

            instr.SetUpdate(m.state("shift1"), Extract(m.input("data_in"), 7, 0));
            instr.SetUpdate(m.state("shift2"), Extract(m.input("data_in"), 15, 8));

        }

        { // SVM_SHIFT3
            std::cout << "inside  SVM_SHIFT3\n";
            auto instr = m.NewInstr("SVM_SHIFT3");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01B6));

            instr.SetUpdate(m.state("shift3"), Extract(m.input("data_in"), 0, 7));
        }


        { // SVM_TH
            std::cout << "inside  SVM_TH\n";
            auto instr = m.NewInstr("SVM_TH");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01B8)); 

            instr.SetUpdate(m.state("Th"), m.input("data_in"));
            

        }

        { // SVM_CMD_STATUS
            std::cout << "inside SVM_CMD_STATUS\n";
            auto instr = m.NewInstr("SVM_CMD_STATUS");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x01A0));

            // child valid bit
            instr.SetUpdate(m.state("run_svma", SelectBit(m.input("data_in"), 7)));
            // child decode bits 
            instr.SetUpdate(m.state("interupt_enable", SelectBit(m.input("data_in"), 6)));
            instr.SetUpdate(m.state("reformulation", SelectBit(m.input("data_in"), 5)));
            instr.SetUpdate(m.state("kernel", Extract(m.input("data_in"), 4, 3)));
            instr.SetUpdate(m.state("order_poly", SelectBit(m.input("data_in"), 2)));
            // can i do stuff with cmd_bits here (above) so as not to do it in each child? 

            std::cout << "going to the kiddies\n";
            DefineLinearChild(m);
            DefineLinearReformChild(m);
            Define2PolyChild(m);
            Define2PolyReformChild(m);
            Define4PolyChild(m);
            DefineRBFChild(m);

       
        }


        { // SVM_STORE_DATA
            std::cout << "inside STORE_DATA\n";
            auto instr = m.NewInstr("STORE_DATA");
            instr.SetDecode((m.input("mode") == 0) & (m.input("addr_in") != 0x01BA));

            auto update_memory_at_addrin = Store(m.state("mem"), m.input("addr_in"), m.input("data_in"));
            instr.SetUpdate(m.state("mem"), update_memory_at_addrin);
            std::cout << "outside STORE_DATA\n";
        }

        return m;
    }




    };
};