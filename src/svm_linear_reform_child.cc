#include <ilang/ilang++.h>
#include <svma_ila.h>
#include <ilang/util/log.h>
#include <uninterpreted_func.h>

namespace ilang {
namespace svma {


void DefineLinearReformChild(Ila& m) {
  
    auto child = m.NewChild("linear_reform");
    auto is_child_valid = (m.state("run_svma") == BvConst(1, 1) 
    & m.state("kernel") == BvConst(0, 2) 
    & m.state("reformulation") == BvConst(1, 1));
    child.SetValid(is_child_valid);

    // create concatenated addresses for sv and tv
    auto sv_addr = m.state("base_addr_sv");
    auto tv_addr = m.state("base_addr_tv");
    auto byte_cnt = child.NewBvState("byte_cnt", 32);
    auto final_sum = child.NewBvState("final_sum", 32);

    child.AddInit(byte_cnt == 0);


    // what we're trying to accomplish
    // sum_i(alpha_i*sv_i) dot tv - b - Th)

    // pc_sv_sum: load bytes from pc_sv and tv and multiply together and add to final_sum
    {
        std::cout << "inside pc_sv_sum linear reform\n";
        auto instr = child.NewInstr("pc_sv_sum_linear_reform");
        instr.SetDecode(m.state("child_state") == BvConst(0, 2));

        auto pc_sv_data = Load(m.state("mem"), sv_addr + byte_cnt);
        auto tv_data = Load(m.state("mem"), tv_addr + byte_cnt);
        auto mult = Mult(pc_sv_data, tv_data);

        instr.SetUpdate(final_sum, Add(final_sum, mult));
        instr.SetUpdate(byte_cnt, byte_cnt + BvConst(1, 32));


        // If the byte counter == sv dimensionality then dot_op else dot_sum
        instr.SetUpdate(m.state("child_state"), Ite(byte_cnt == m.state("fv_dim") - BvConst(1, 32), 
        BvConst(1, 2), BvConst(0, 2)));

    }

    // child_end: take the sign of the result in final_sum and set the score, set child to invalid
    {
    
        std::cout << "inside child_end linear reform\n";
        auto instr = child.NewInstr("child_end_linear_reform");
        instr.SetDecode(m.state("child_state") == BvConst(1, 2));

        auto final_sum_shift = Shift(final_sum, Concat(BvConst(0, 24), m.state("shift1")));
        auto minus_b = Sub(final_sum_shift, m.state("b"));
        auto minus_th = Sub(minus_b, m.state("th"));
        
        instr.SetUpdate(m.state("score"), minus_th);
        instr.SetUpdate(m.state("output"), Ite((Greatest(minus_th, BvConst(0, 32)) == 1), BvConst(1, 1), BvConst(0, 1)));
        instr.SetUpdate(m.state("done"), BvConst(1, 1));
        instr.SetUpdate(m.state("child_state"), BvConst(0, 2));
        instr.SetUpdate(m.state("run_svma"), BvConst(0, 1));
   

        

    }
}


}; // namespace max

}; // namespace ilang
