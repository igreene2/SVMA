#include <ilang/ilang++.h>
#include <svma_ila.h>
#include <ilang/util/log.h>
#include <uninterpreted_func.h>

namespace ilang {
namespace max {


void DefineLinearReformChild(Ila& m) {
  
    auto child = m.NewChild("linear_reform");
    auto is_child_valid = (m.state("run_svma") == BvConst(1, 1) 
    & m.state("kernel") == BvConst(0, 2) 
    & m.state("reformulation") == Bvconst(1, 1));
    child.SetValid(is_child_valid);

    // create concatenated addresses for sv and tv
    auto sv_addr = Concat(m.state("base_addr_sv_H"), m.state("base_addr_sv_L"));
    auto tv_addr = Concat(m.state("base_addr_tv_H"), m.state("base_addr_tv_L"))
    auto byte_cnt = child.NewBvState("byte_cnt", 16);
    auto final_sum = child.NewBvState("final_sum", 16);

    child.AddInit(byte_cnt == 0);

    std::cout << "defined all the kiddie state\n";

    // what we're trying to accomplish
    // sum_i(alpha_i*sv_i) dot tv - b - Th)

    // pc_sv_sum: load bytes from pc_sv and tv and multiply together and add to final_sum
    {
        std::cout << "inside pc_sv_sum linear reform\n";
        auto instr = child.NewInstr("pc_sv_sum");
        instr.SetDecode(m.state("child_state") == BvConst(0, 2));

        auto pc_sv_data = Load(mem, sv_addr + byte_cnt);
        auto tv_data = Load(mem, tv_addr + byte_cnt);
        auto mult = Mult(pc_sv_data, tv_data);

        instr.SetUpdate(final_sum, final_sum + mult);
        instr.SetUpdate(byte_cnt, byte_cnt + BvConst(1, 16));


        // If the byte counter > sv dimensionality then dot_op else dot_sum
        // look into == vs >
        instr.SetUpdate(m.state("child_state"), Ite(byte_cnt == m.state("fv_dim"), 
        BvConst(0, 2), BvConst(1, 2));

    }

    // child_end: take the sign of the result in final_sum and set the score, set child to invalid
    {
    
        std::cout << "inside child_end linear\n";
        auto instr = child.NewInstr("child_end");
        instr.SetDecode(m.state("child_state") == BvConst(1, 2));

        auto final_sum_shift = Shift(final_sum, shift1);
        auto minus_b = Sub(final_sum_shift, m.state("b"));
        auto minus_th = Sub(minus_b, m.state("th"));
        
        instr.SetUpdate(m.state("score"), minus_th);
        instr.SetUpdate(m.state("output"), Ite(minus_th > BvConst(0, 16), BvConst(1, 1), BvConst(0, 1));
        instr.SetUpdate(m.state("done"), BvConst(0, 2));
        instr.SetUpdate(m.state("child_state"), BvConst(0, 2));
        instr.setUpdate(m.state("run_svma"), BvConst(0, 1));
   

        

    }


}; // namespace max

}; // namespace ilang
