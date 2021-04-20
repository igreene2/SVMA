/// \file the ila example of svma accelerator: 
/// support vector machine accelerator for biomedical purposes,
//  receives MMIO instruction
/// Isabel Greene (igreene@princeton.edu)
///

#include <ilang/ilang++.h>
#include <svma_ila.h>
#include <ilang/util/log.h>

namespace ilang {
namespace svma {

    Ila GetSVMAIla(const std::string& model_name) {
            // construct the model
            auto m = ilang::Ila("SVMA");
            std::cout << "made svma\n";

            // inputs
            m.NewBvInput("mode", 1); 
            m.NewBvInput("addr_in", 32);
            m.NewBvInput("data_in", 32);

            // internal arch state 
            m.NewBvState("base_addr_sv", 32);
            m.NewBvState("base_addr_tv", 32);
            m.NewBvState("tau", 32);
            m.NewBvState("c", 32);
            m.NewBvState("b", 32);
            m.NewBvState("fv_dim", 32);
            m.NewBvState("num_sv", 32);
            m.NewBvState("th", 32);
            m.NewBvState("shift1", 8);
            m.NewBvState("shift2", 8);
            m.NewBvState("shift3", 8);
            m.NewBvState("cmd_bits", 16);

            // the memory
            m.NewMemState("mem", 32, 32);

            // the output
            m.NewBvState("score", 32); 
            m.NewBvState("output_proc", 32);


            // internal state
            m.NewBvState("run_svma", 1);
            m.NewBvState("interrupt_enable", 1); 
            m.NewBvState("reformulation", 1);
            m.NewBvState("kernel", 2);
            m.NewBvState("order_poly", 1);
            m.NewBvState("output", 1);
            m.NewBvState("done", 1);
            m.NewBvState("child_state", 2);
        
            m.AddInit(m.state("child_state") == 0);
        
            std::cout << "declared all the state\n";
            m.SetValid(m.input("addr_in") >= 0x0000 & m.input("addr_in") < 0x25C2);
            std::cout << "did the valid\n";



        { // SVM_SV_BASEADDR_L
            std::cout << "inside  SVM_SV_BASEADDR\n";
            auto instr = m.NewInstr("SVM_SV_BASEADDR");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25A4)); 

            instr.SetUpdate(m.state("base_addr_sv"), m.input("data_in"));
            

        }



        { // SVM_TV_BASEADDR_L
            std::cout << "inside  SVM_TV_BASEADDR\n";
            auto instr = m.NewInstr("SVM_TV_BASEADDR");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25A8)); 

            instr.SetUpdate(m.state("base_addr_tv"), m.input("data_in"));
            

        }


        { // SVM_GAMMA (tau)
            std::cout << "inside  SVM_GAMMA \n";
            auto instr = m.NewInstr("SVM_GAMMA");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25AA)); 

            instr.SetUpdate(m.state("tau"), m.input("data_in"));
            

        }


        { // SVM_C
            std::cout << "inside  SVM_C\n";
            auto instr = m.NewInstr("SVM_C");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25AC)); 

            instr.SetUpdate(m.state("c"), m.input("data_in"));
            

        }


        { // SVM_B
            std::cout << "inside  SVM_B\n";
            auto instr = m.NewInstr("SVM_B");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25AE)); 

            instr.SetUpdate(m.state("b"), m.input("data_in"));
            

        }

        { // SVM_FV_DIM
            std::cout << "inside SVM_FV_DIM\n";
            auto instr = m.NewInstr("SVM_FV_DIM");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25B0)); // 
   
            instr.SetUpdate(m.state("fv_dim"), m.input("data_in"));
            

        }


        { // SVM_NUMSV
            std::cout << "inside  SVM_NUMSV\n";
            auto instr = m.NewInstr("SVM_NUMSV");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25B2)); 

            instr.SetUpdate(m.state("num_sv"), m.input("data_in"));
            

        }
        

        { // SVM_SHIFT12
            std::cout << "inside  SVM_SHIFT12\n";
            auto instr = m.NewInstr("SVM_SHIFT12");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25B4)); 

            instr.SetUpdate(m.state("shift1"), Extract(m.input("data_in"), 7, 0));
            instr.SetUpdate(m.state("shift2"), Extract(m.input("data_in"), 15, 8));

        }

        { // SVM_SHIFT3
            std::cout << "inside  SVM_SHIFT3\n";
            auto instr = m.NewInstr("SVM_SHIFT3");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25B6));

            instr.SetUpdate(m.state("shift3"), Extract(m.input("data_in"), 7, 0));
        }


        { // SVM_TH
            std::cout << "inside  SVM_TH\n";
            auto instr = m.NewInstr("SVM_TH");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25B8)); 

            instr.SetUpdate(m.state("th"), m.input("data_in"));
            
            
        }

        { // SVM_CMD_STATUS
            std::cout << "inside SVM_CMD_STATUS\n";
            auto instr = m.NewInstr("SVM_CMD_STATUS");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") == 0x25A0));

            // child valid bit
            instr.SetUpdate(m.state("run_svma"), SelectBit(m.input("data_in"), 7));
            // child decode bits 

            instr.SetUpdate(m.state("interrupt_enable"), SelectBit(m.input("data_in"), 6));

            instr.SetUpdate(m.state("reformulation"), SelectBit(m.input("data_in"), 5));

            instr.SetUpdate(m.state("kernel"), Extract(m.input("data_in"), 4, 3));
           
            instr.SetUpdate(m.state("order_poly"), SelectBit(m.input("data_in"), 2));
    

            DefineLinearChild(m);
            DefineLinearReformChild(m);
            DefineTwoPolyChild(m);
            DefineTwoPolyReformChild(m);
            DefineFourPolyChild(m);
            DefineRBFChild(m);

       
        }


        { // SVM_STORE_DATA
            std::cout << "inside STORE_DATA\n";
            auto instr = m.NewInstr("STORE_DATA");
            instr.SetDecode((m.input("mode") == 1) & (m.input("addr_in") < 0x25A2));

            auto update_memory_at_addrin = Store(m.state("mem"), m.input("addr_in"), m.input("data_in"));
            instr.SetUpdate(m.state("mem"), update_memory_at_addrin);
            std::cout << "outside STORE_DATA\n";
        }

        { // SVM_CMD_STATUS
            std::cout << "inside SVM_CMD_STATUS_OUT\n";
            auto instr = m.NewInstr("SVM_CMD_STATUS_OUT");
            instr.SetDecode((m.input("mode") == 0) & (m.input("addr_in") == 0x25A0));

            auto combined = Concat(m.state("done"), m.state("output"));
            instr.SetUpdate(m.state("output_proc"), Concat(BvConst(0, 30), combined));

        }

        { // SVM_SCORE
            std::cout << "inside SVM_SCORE\n";
            auto instr = m.NewInstr("SVM_SCORE");
            instr.SetDecode((m.input("mode") == 0) & (m.input("addr_in") == 0x25BA));

            instr.SetUpdate(m.state("output_proc"), m.state("score"));

        }



        return m;
    }




    };
};