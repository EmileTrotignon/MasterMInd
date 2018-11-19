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

class MasterMind {

private:
    unique_ptr<int[]> solution;

public:

    bool won;
    unsigned int n_couleurs;
    unsigned int n_pins;
    unsigned int n_coups_max;
    unsigned int n_coups;
    unique_ptr<int[]> tab_noirs;
    unique_ptr<int[]> tab_blancs;
    unique_ptr<unique_ptr<int[]>[]> tab_coups;

    MasterMind(const unique_ptr<int[]> &sol, unsigned int nc, unsigned int np, unsigned int cmax);

    MasterMind(unsigned int nc, unsigned int np, unsigned int cmax);

    MasterMind(MasterMind &g);

    MasterMind(MasterMind g, unique_ptr<int[]> &nsol);

    MasterMind &operator=(MasterMind const &g);

    friend ostream &operator<<(ostream &s, MasterMind &game);

    void change_sol(unique_ptr<int[]> &nsol);

    bool play(const unique_ptr<int[]> &coup, bool log = false);
};


#endif //MASTERMIND_MASTERMIND_H
