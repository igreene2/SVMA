#include <ilang/ilang++.h>
#include <svma_ila.h>
#include <ilang/util/log.h>
#include <uninterpreted_func.h>

namespace ilang {
namespace svma {


void DefineLinearChild(Ila& m) {
  
    auto child = m.NewChild("linear");
    auto is_child_valid = (m.state("run_svma") == BvConst(1, 1) 
    & m.state("kernel") == BvConst(0, 2) 
    & m.state("reformulation") == BvConst(0, 1));
    child.SetValid(is_child_valid);

    // create concatenated addresses for sv and tv
    auto sv_addr = m.state("base_addr_sv");
    auto tv_addr = m.state("base_addr_tv");
    auto tv_test = child.NewBvState("tv", 32);
    auto addr_cnt = child.NewBvState("addr_cnt", 32);
    auto byte_cnt = child.NewBvState("byte_cnt", 32);
    auto vector_cnt = child.NewBvState("vector_cnt", 32);
    auto dot_sum = child.NewBvState("dot_sum", 32);
    auto final_sum = child.NewBvState("final_sum", 32);
    auto alpha = child.NewBvState("alpha", 32);

    child.AddInit(byte_cnt == 0);
    child.AddInit(addr_cnt == 0);
    child.AddInit(vector_cnt == 0);
    child.AddInit(dot_sum == 0);
    child.AddInit(alpha == 0);
    child.AddInit(tv_test == 0);

    //std::cout << "defined all the kiddie state\n";

    // what we're trying to accomplish
    // sum_i(alpha_i(sv_i dot )tv - b - Th)


    { // vector_sum_prep: increment vector counter and reset dot_sum
        std::cout << "inside vector_sum_prep linear\n";
        auto instr = child.NewInstr("vector_sum_prep_linear");
        instr.SetDecode(m.state("child_state") == BvConst(0, 2));
        std::cout << "inside vector_sum_prep past decode\n";
        
        instr.SetUpdate(vector_cnt, vector_cnt + BvConst(1, 32));
        instr.SetUpdate(dot_sum, BvConst(0, 32));
        instr.SetUpdate(tv_test, tv_addr);

        // move to dot_sum
        instr.SetUpdate(m.state("child_state"), BvConst(1, 2));

    }

    // dot_sum: load bytes from each vector, multiply thme and add them to dot_sum
    {
        std::cout << "inside dot_sum linear\n";
        auto instr = child.NewInstr("dot_sum_linear");
        instr.SetDecode(m.state("child_state") == BvConst(1, 2));
        auto tv_data = Load(m.state("mem"), tv_addr + byte_cnt);
        auto sv_data = Load(m.state("mem"), sv_addr + addr_cnt);
        auto mult = Mult(tv_data, sv_data);

        instr.SetUpdate(dot_sum, Add(dot_sum, mult));
        instr.SetUpdate(addr_cnt, addr_cnt + BvConst(1, 32));
        instr.SetUpdate(byte_cnt, byte_cnt + BvConst(1, 32));

        // If the byte counter > sv dimensionality then dot_op else dot_sum
        instr.SetUpdate(m.state("child_state"), Ite(byte_cnt == m.state("fv_dim") - BvConst(1, 32), 
        BvConst(2, 2), BvConst(1, 2)));

    }

    // dot_op: multiply by Ai, add to final_sum 
    {   
        std::cout << "inside dot_op of linear \n";
        auto instr = child.NewInstr("dot_op_linear");
        instr.SetDecode(m.state("child_state") == BvConst(2, 2));

        
        auto Ai = Load(m.state("mem"), sv_addr + addr_cnt);  
        auto dot_sum_shift = Shift(dot_sum, Concat(BvConst(0, 24), m.state("shift1")));
        auto mult = Mult(dot_sum_shift, Ai);
        instr.SetUpdate(final_sum, Add(final_sum, mult));
        instr.SetUpdate(addr_cnt, addr_cnt + BvConst(1, 32));
        instr.SetUpdate(byte_cnt, BvConst(0, 32));
      
        
        // If the vector counter == number of sv then child_end else vector_sum_prep
        instr.SetUpdate(m.state("child_state"), Ite(vector_cnt == m.state("num_sv"), 
        BvConst(3, 2), BvConst(0, 2)));

    }

    // child_end: take the sign of the result in final_sum and set the score, set child to invalid
    {
    
        std::cout << "inside child_end linear\n";
        auto instr = child.NewInstr("child_end_linear");
        instr.SetDecode(m.state("child_state") == BvConst(3, 2));    
        auto final_sum_shift = Shift(final_sum, Concat(BvConst(0, 24), m.state("shift2")));
        auto temp_sub = Sub(final_sum_shift, m.state("b"));
        auto sub = Sub(temp_sub, m.state("th"));

        instr.SetUpdate(m.state("score"), sub);
        instr.SetUpdate(m.state("output"), Ite((Greatest(sub, BvConst(0, 32)) == 1), BvConst(1, 1), BvConst(0, 1)));
        instr.SetUpdate(m.state("done"), BvConst(1, 1));
        instr.SetUpdate(m.state("child_state"), BvConst(0, 2));
        instr.SetUpdate(m.state("run_svma"), BvConst(0, 1));
   

        

    }
}


}; // namespace max

}; // namespace ilang