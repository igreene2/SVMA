#include <ilang/ilang++.h>
#include <svma_ila.h>
#include <ilang/util/log.h>
#include <uninterpreted_func.h>

namespace ilang {
namespace svma {


void DefineTwoPolyReformChild(Ila& m) {
  
    auto child = m.NewChild("Twopoly_reform");
    auto is_child_valid = (m.state("run_svma") == BvConst(1, 1) 
    & m.state("kernel") == BvConst(1, 2) 
    & m.state("reformulation") == BvConst(1, 1)
    & m.state("order_poly") == BvConst(0, 1));
    child.SetValid(is_child_valid);

    // create concatenated addresses for sv and tv
    auto sv_addr = Concat(m.state("base_addr_sv_H"), m.state("base_addr_sv_L"));
    auto tv_addr = Concat(m.state("base_addr_tv_H"), m.state("base_addr_tv_L"));
    auto byte_cnt = child.NewBvState("byte_cnt", 32);
    auto final_sum = child.NewBvState("final_sum", 32);
    auto x_transpose_total = child.NewBvState("x_tranpose_total", 32);
    auto tv_element_cnt = child.NewBvState("mem_cnt", 32);
    auto addr_cnt = child.NewBvState("addr_cnt", 32);
    auto dot_sum = child.NewBvState("dot_sum", 32);


    child.AddInit(byte_cnt == 0);
    child.AddInit(dot_sum == 0);
    child.AddInit(tv_element_cnt == 0);
    child.AddInit(addr_cnt == 0);
    child.AddInit(x_transpose_total == 0);
    child.AddInit(final_sum == 0);

    std::cout << "defined all the kiddie state\n";

    // what we're trying to accomplish
    // tv_t * pc_matrix * tv - b - Th)

    // first the dot product (be nice if i could have a function that does this)
    // assume something exists called sv_addr and tv_addr


    // x_tranpose_matrix_multiply: load matrix and tv element, multipy, add to x_transpose_total
    {
        std::cout << "inside x_tranpose_matrix_mulitply poly reform\n";
        auto instr = child.NewInstr("x_transpose_matrix_multiply_poly2_reform");
        instr.SetDecode(m.state("child_state") == BvConst(0, 2));

        auto pc_mat_data = Load(m.state("mem"), sv_addr + addr_cnt);
        auto tv_data = Load(m.state("mem"), tv_addr + byte_cnt);
        auto mult  = Mult(pc_mat_data, tv_data);
  
   
        instr.SetUpdate(x_transpose_total, x_transpose_total + mult);
        instr.SetUpdate(byte_cnt, byte_cnt + BvConst(1, 32));
        instr.SetUpdate(addr_cnt, addr_cnt + BvConst(1, 32));
        // If the byte counter > sv dimensionality then dot_op else dot_sum
        // look into == vs >
        instr.SetUpdate(m.state("child_state"), Ite(byte_cnt == m.state("fv_dim"), 
        BvConst(1, 2), BvConst(0, 2)));

    }

    // x_transpose_reset: multiply x_total by element of tv and add to final_sum
    {   
        std::cout << "inside x_transpose_reset of 2poly reformulated \n";
        auto instr = child.NewInstr("x_tranpose_reset_poly2_reform");
        instr.SetDecode(m.state("child_state") == BvConst(1, 2));

        auto x_tt_shift = Shift(x_transpose_total, Concat(BvConst(0, 24), m.state("shift1")));
        auto tv_data = Load(m.state("mem"), tv_addr + tv_element_cnt);
        auto mult = Mult(x_tt_shift, tv_data);
     

        instr.SetUpdate(tv_element_cnt, tv_element_cnt + BvConst(1, 32));
        instr.SetUpdate(final_sum, final_sum + mult);
        instr.SetUpdate(byte_cnt, BvConst(0, 32));
        
        // If the vector counter > number of sv then child_end else vector_sum_prep
        instr.SetUpdate(m.state("child_state"), Ite(tv_element_cnt == m.state("fv_dim"), 
        BvConst(2, 2), BvConst(0, 2)));

    }

    // child_end: take the sign of the result in final_sum and set the score, set child to invalid
    {
        std::cout << "inside child_end 2poly reformulated\n";
        auto instr = child.NewInstr("child_end_2poly_reform");
        instr.SetDecode(m.state("child_state") == BvConst(2, 2));

        auto final_sum_shift = Shift(final_sum, Concat(BvConst(0, 24), m.state("shift2")));
        auto sub_b = Sub(final_sum_shift, m.state("b"));
        auto sub_th = Sub(sub_b, m.state("th"));

        instr.SetUpdate(m.state("score"), sub_th);
        instr.SetUpdate(m.state("output"), Ite((Greatest(sub_th, BvConst(0, 32)) == 1), BvConst(1, 1), BvConst(0, 1)));
        instr.SetUpdate(m.state("done"), BvConst(1, 1));
        instr.SetUpdate(m.state("child_state"), BvConst(0, 2));
        instr.SetUpdate(m.state("run_svma"), BvConst(0, 1));
   
        // SHIFT? truncation??
        

    }
}

}; // namespace max

}; // namespace ilang