//
// Created by emile on 19/11/18.
//

#include <iostream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <math.h>
#include <vector>

#ifndef MASTERMIND_MASTERMIND_H
#define MASTERMIND_MASTERMIND_H
#include "lib_perso.h"



using namespace std;

template<size_t n_pins, size_t n_colors>
class MasterMind {

private:
    array<int, n_pins> solution;

public:

    bool won;
    size_t n_coups_max;
    vector<int> tab_noirs;
    vector<int> tab_blancs;
    vector<array<int, n_pins>> tab_coups;

    MasterMind(const array<int, n_pins> &sol, size_t cmax) : solution(sol),
                                                             n_coups_max(cmax),
                                                             won(false)
    {
    }

    explicit MasterMind(size_t cmax) : n_coups_max(cmax),
                                       won(false)
    {
        for (size_t p = 0; p < n_pins; p++) solution[p] = rand() % n_colors;
    }

    //MasterMind(MasterMind &g);

    MasterMind(MasterMind g, array<int, n_pins> &nsol)
    {
        won = g.won;
        n_coups_max = g.n_coups_max;
        auto uptr = make_unique<int[]>(n_pins);
        solution.swap(uptr);
        copy_array(nsol, solution, n_pins);

        auto tb = make_unique<int[]>(n_coups_max);
        auto tn = make_unique<int[]>(n_coups_max);
        auto tc = make_unique<unique_ptr<int[]>[]>(n_coups_max);

        tab_blancs.swap(tb);
        tab_noirs.swap(tn);
        tab_coups.swap(tc);
        for (int i = 0; i < g.n_coups; i++)
        {
            (*this).play(g.tab_coups[i]);
        }

    }

    //MasterMind &operator=(MasterMind const &g);

    friend ostream &operator<<(ostream &s, MasterMind &game)
    {
        s << "MasterMind\n"
          << "    solution : ";
        output_tabb<n_pins>(s, game.solution);
        s << "\n";
        for (int c = game.tab_coups.size() - 1; c >= 0; c--)
        {
            s << "    coup #" << c + 1 << "  : ";
            output_tabb<n_pins>(s, game.tab_coups[c]);
            s << " N: " << game.tab_noirs[c] << " B: " << game.tab_blancs[c] << "\n";
        }
        return s;
    }

    void change_sol(array<int, n_pins> &nsol)
    {
        solution = nsol;
    }

    bool play(const array<int, n_pins> &coup, bool log = false)
    {
        if (tab_coups.size() == n_coups_max - 1)
        {
            won = true;
            return true;
        }
        int noirs = 0;
        int blancs = 0;
        for (int p = 0; p < n_pins; p++)
        {
            if (solution[p] == coup[p])
            {
                noirs++;
                blancs--;
            }
            if (in_array_ordered<n_pins>(coup, solution, p))
            {
                if (log)
                {
                    output_tabb<n_pins>(cout, solution);
                    cout << "    " << coup[p] << "  " << p << endl;
                }
                blancs++;
            }
        }
        tab_noirs.push_back(noirs);
        tab_blancs.push_back(blancs);

        tab_coups.push_back(coup);
        won = (noirs == n_pins);
        return won;

    }

    void unplay_last_coup()
    {
        tab_coups.pop_back();
        tab_noirs.pop_back();
        tab_blancs.pop_back();

    }
};


#endif //MASTERMIND_MASTERMIND_H
