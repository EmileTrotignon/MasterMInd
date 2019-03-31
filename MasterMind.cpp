//
// Created by emile on 19/11/18.
//

#include "MasterMind.h"

/*template<size_t n_pins, size_t n_colors>
MasterMind<n_pins, n_colors>::MasterMind(MasterMind &g)
{
    won = g.won;
    n_pins = g.n_pins;
    n_coups_max = g.n_coups_max;
    solution = g.solution;

    for (int i = 0; i < g.tab_coups.size(); i++)
    {
        (*this).play(g.tab_coups[i]);
    }
}*/


/*
template<size_t n_pins, size_t n_colors>
MasterMind<n_pins, n_colors> &MasterMind<n_pins, n_colors>::operator=(MasterMind<n_pins, n_colors> const &g)
{
    won = g.won;
    n_pins = g.n_pins;
    n_coups_max = g.n_coups_max;
    solution.swap(uptr);
    copy_array(g.solution, solution, n_pins);

    auto tb = make_unique<int[]>(n_coups_max);
    auto tn = make_unique<int[]>(n_coups_max);
    auto tc = make_unique<unique_ptr<int[]>[]>(n_coups_max);

    tab_blancs.swap(tb);
    tab_noirs.swap(tn);
    tab_coups.swap(tc);
    return *this;
}
 */




