#include <ilang/ilang++.h>
#include <svma_ila.h>
#include <ilang/util/log.h>
#include <uninterpreted_func.h>

namespace ilang {
namespace svma {


void DefineRBFChild(Ila& m) {
  
    auto child = m.NewChild("rbf");
    auto is_child_valid = (m.state("run_svma") == BvConst(1, 1) 
    & m.state("kernel") == BvConst(2, 2));
    child.SetValid(is_child_valid);

    // create concatenated addresses for sv and tv
    auto sv_addr = m.state("base_addr_sv");
    auto tv_addr = m.state("base_addr_tv");
    auto byte_cnt = child.NewBvState("byte_cnt", 32);
    auto addr_cnt = child.NewBvState("addr_cnt", 32);
    auto vector_cnt = child.NewBvState("vector_cnt", 32);
    auto dot_sum = child.NewBvState("dot_sum", 32);
    auto final_sum = child.NewBvState("final_sum", 32);

    child.AddInit(byte_cnt == 0);
    child.AddInit(vector_cnt == 0);
    child.AddInit(dot_sum == 0);
    child.AddInit(final_sum == 0);
    child.AddInit(addr_cnt == 0);


    // what we're trying to accomplish
    // sum_i(alpha_i(sv_i dot )tv - b - Th)

    { // vector_sub_prep: increment vector counter and reset dot_sum
        std::cout << "inside vector_sum_prep rbf\n";
        auto instr = child.NewInstr("vector_sum_prep_rbf");
        instr.SetDecode(m.state("child_state") == BvConst(0, 2));
        
        instr.SetUpdate(vector_cnt, vector_cnt + BvConst(1, 32));
        instr.SetUpdate(dot_sum, BvConst(0, 32));
        instr.SetUpdate(byte_cnt, BvConst(0, 32));

        // move to dot_sum
        instr.SetUpdate(m.state("child_state"), BvConst(1, 2));

    }

    // dot_sum: load bytes from each vector, subtract tv from sv, square that and add to dot_sum
    {
        std::cout << "inside dot_sum rbf\n";
        auto instr = child.NewInstr("dot_sum_rbf");
        instr.SetDecode(m.state("child_state") == BvConst(1, 2));

        auto tv_data = Load(m.state("mem"), tv_addr + byte_cnt);
        auto sv_data = Load(m.state("mem"), sv_addr + addr_cnt);
        auto mult = Sub(sv_data, tv_data);
        auto square = Mult(mult, mult);      
        auto square_shift = Shift(square, Concat(BvConst(0, 24), m.state("shift3")));

        instr.SetUpdate(dot_sum, dot_sum + square_shift);
        instr.SetUpdate(byte_cnt, byte_cnt + BvConst(1, 32));
        instr.SetUpdate(addr_cnt, addr_cnt + BvConst(1, 32));
       
        // If the byte counter == sv dimensionality then dot_op else dot_sum
        instr.SetUpdate(m.state("child_state"), Ite(byte_cnt == m.state("fv_dim") - BvConst(1, 32), 
        BvConst(2, 2), BvConst(1, 2)));

    }

    // dot_op: multiply by -tau, exponentiate, mult by Ai
    {   
        std::cout << "inside dot_op of rbf \n";
        auto instr = child.NewInstr("dot_op_rbf");
        instr.SetDecode(m.state("child_state") == BvConst(2, 2));

       
        auto Ai = Load(m.state("mem"), sv_addr + addr_cnt);
        auto tau = Mult(dot_sum, m.state("tau"));
        auto tau_shift = Shift(tau, Concat(BvConst(0, 24), m.state("shift1")));
        auto negative = MultbyNegativeOne(tau_shift);
        auto exponent = Exponent(negative);
        auto mult = Mult(Ai, exponent);
       

        instr.SetUpdate(final_sum, final_sum + mult);
        instr.SetUpdate(addr_cnt, addr_cnt + BvConst(1, 32));
        
        // If the vector counter == number of sv then child_end else vector_sum_prep
        instr.SetUpdate(m.state("child_state"), Ite(vector_cnt == m.state("num_sv"), 
        BvConst(3, 2), BvConst(0, 2)));

    }

    // child_end: take the sign of the result in final_sum and set the score, set child to invalid
    {
    
        std::cout << "inside child_end rbf\n";
        auto instr = child.NewInstr("child_end_rbf");
        instr.SetDecode(m.state("child_state") == BvConst(3, 2));

        auto final_sum_shift = Shift(final_sum, Concat(BvConst(0, 24), m.state("shift2")));
        auto sub_b = Sub(final_sum, m.state("b"));
        auto sub_th = Sub(sub_b, m.state("th"));

        instr.SetUpdate(m.state("score"), sub_th);
        instr.SetUpdate(m.state("output"), Ite((Greatest(sub_th, BvConst(0, 32)) == 1), BvConst(1, 1), BvConst(0, 1)));
        instr.SetUpdate(m.state("done"), BvConst(1, 1));
        instr.SetUpdate(m.state("child_state"), BvConst(0, 2));
        instr.SetUpdate(m.state("run_svma"), BvConst(0, 1));
        

    }
}


}; // namespace max

}; // namespace ilang
