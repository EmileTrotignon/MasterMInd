#include <iostream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <math.h>
#include <vector>
#include "lib_perso.h"
#include "MasterMind.h"
#define N 2000

using namespace std;

template
class std::unique_ptr<int[]>;



void all_alphanum(vector<unique_ptr<int[]>> &v, unsigned long n_psol, int n_pins, int n_couleurs) {
    auto next_coup = make_unique<int[]>(n_pins);
    initialize(next_coup, n_pins);
    for (int i = 0; i < n_psol; i++) {
        v[i].reset(new int[n_pins]);
        copy_array(next_coup, v[i], n_pins);
        next_alphanum(next_coup, n_couleurs, n_pins);
    }
}

bool check_sol(const MasterMind &game, const unique_ptr<int[]> &test_sol) {
    MasterMind test_game(test_sol, game.n_coups, game.n_pins, game.n_coups_max);

    for (int c = 0; c < game.n_coups; c++) {
        test_game.play(game.tab_coups[c]);
        if ((test_game.tab_noirs[c] != game.tab_noirs[c]) || (test_game.tab_blancs[c] != game.tab_blancs[c]))
            return false;
    }
    return true;
}

int solve_random(MasterMind &game) {
    auto next_coup = make_unique<int[]>(game.n_pins);
    initialize(next_coup, game.n_pins);
    while (!game.play(next_coup)) {
        initialize_random(next_coup, game.n_couleurs, game.n_pins);
        while (!check_sol(game, next_coup)) {
            initialize_random(next_coup, game.n_couleurs, game.n_pins);
        }
        //cout << game << endl;
    }
    return game.n_coups;
}

float moyenne(const int *tab, int t) {
    float s = 0;
    for (int i = 0; i < t; i++) {
        s += tab[i];
    }
    return s / t;
}

double valeur_situation(MasterMind &g, vector<unique_ptr<int[]>> &coups_possible) {
    int val = 0;
    for (int i = 0; i < coups_possible.size(); i++) if (check_sol(g, coups_possible[i])) val++;
    return ((double) val) / coups_possible.size();
}


double valeur_situation(MasterMind &g, int prec, vector<unique_ptr<int[]>> &coups_possible, int n_sol_left) {
    if (prec > n_sol_left) return valeur_situation(g, coups_possible);
    int val = 0;
    for (int i = 0; i < prec; i++) {
        if (check_sol(g, coups_possible[rand() % n_sol_left])) val++;
    }
    return ((double) val) / prec;
}


double valeur_situation(MasterMind &g, int prec) {
    int val = 0;
    unique_ptr<int[]> psol = make_unique<int[]>(g.n_pins);
    initialize_random(psol, g.n_couleurs, g.n_pins);
    for (int i = 0; i < prec; i++) {
        g.n_coups--;
        while (!check_sol(g, psol)) initialize_random(psol, g.n_couleurs, g.n_pins);
        g.n_coups++;
        //cout << i << endl;
        if (check_sol(g, psol)) val++;

    }
    return ((double) val) / prec;
}

double valeur_coup(MasterMind &g, unique_ptr<int[]> &coup, vector<unique_ptr<int[]>> &coups_possible) {
    double val = 0;
    auto rsol = make_unique<int[]>(g.n_pins);
    MasterMind ng(g);
    copy_array(coups_possible[0], rsol, g.n_pins);
    ng.change_sol(rsol);
    ng.play(coup);
    val += valeur_situation(ng, coups_possible);

    for (int i = 1; i < coups_possible.size(); i++) {
        copy_array(coups_possible[i], rsol, g.n_pins);
        ng.change_sol(rsol);
        ng.n_coups--;
        ng.play(coup);
        val += valeur_situation(ng, coups_possible);
    }
    return val / coups_possible.size();
}

double valeur_coup(MasterMind &g, unique_ptr<int[]> &coup, int prec,
                   vector<unique_ptr<int[]>> &coups_possible, unsigned long n_sol_left) {
    double val = 0;
    auto rsol = make_unique<int[]>(g.n_pins);
    copy_array(coups_possible[rand() % n_sol_left], rsol, g.n_pins);
    MasterMind ng(g);
    ng.change_sol(rsol);
    ng.play(coup);

    for (int i = 0; i < prec; i++) {
        val += valeur_situation(ng, prec / 2, coups_possible, n_sol_left);
        //cout << val << endl;
        copy_array(coups_possible[rand() % n_sol_left], rsol, g.n_pins);
        ng.change_sol(rsol);
        ng.n_coups--;
        ng.play(coup);
    }
    if (val / prec == 0) {
        cout << g << endl;
        output_tabb(cout, coup, g.n_pins);
        cout << endl;
        cout << prec << endl;

    }
    return val / prec;
}

double valeur_coup(MasterMind &g, unique_ptr<int[]> &coup, int prec) {
    double val = 0;
    auto rsol = make_unique<int[]>(g.n_pins);
    while (!check_sol(g, rsol)) initialize_random(rsol, g.n_couleurs, g.n_pins);
    MasterMind ng(g);
    ng.change_sol(rsol);
    ng.play(coup);

    for (int i = 0; i < prec; i++) {
        val += valeur_situation(ng, prec / 2);
        //cout << val << endl;
        while (!check_sol(g, rsol)) initialize_random(rsol, g.n_couleurs, g.n_pins);
        ng.change_sol(rsol);

    }
    /*if (val / prec == 0) {
        cout << g << endl;
        output_tabb(cout, coup, g.n_pins);
        cout << endl;
        cout << prec << endl;
        exit(EXIT_FAILURE);

    }*/
    return val / prec;
}


int solve_opti(MasterMind &g, int prec) {
    unsigned long n_psol = fpow(g.n_couleurs, g.n_pins);
    unsigned long n_sol_left = n_psol;
    double v_best_coup;
    double v_coup;

    vector<unique_ptr<int[]>> coups_possibles(n_psol);
    all_alphanum(coups_possibles, n_psol, g.n_pins, g.n_couleurs);

    auto next_coup = make_unique<int[]>(g.n_pins);
    initialize(next_coup, g.n_pins);

    while (!g.won) {
        v_best_coup = 2;
        for (unsigned long i = n_sol_left - 1; i >= 0 && i < n_sol_left; i--) {
            /*cout << "tested coup : ";
            output_tabb(cout, coups_possibles[i], g.n_pins);
            cout << endl;
            cout << "current best coup : " ;
            output_tabb(cout, next_coup, g.n_pins);
            cout << endl;*/
            cout << i << " / " << n_sol_left << " / ";
            output_tabb(cout, coups_possibles[i], g.n_pins);
            cout << endl;
            if (check_sol(g, coups_possibles[i])) {
                v_coup = valeur_coup(g, coups_possibles[i], coups_possibles);
                /*cout << "vcoup : " << v_coup << endl;
                cout << "v_best_coup : " << v_best_coup << endl << endl;*/ 
                if (v_coup < v_best_coup) {
                    v_best_coup = v_coup;
                    copy_array(coups_possibles[i], next_coup, g.n_pins);
                }
            } else {
                coups_possibles.erase(coups_possibles.begin() + i);
                n_sol_left -= 1;
            }
            /*for (int j=0; j<n_sol_left; j++){
                output_tabb(cout, coups_possibles[i], g.n_pins);
                cout << " | ";
            }*/
            /*cout << "nsolleft : " << n_sol_left << endl;
            cout <<endl;*/

        }
        g.play(next_coup);
        cout << g << endl;
    }
    return g.n_coups;
}

double eval_strat(int it) {
    int *val = new int[it];
    MasterMind g(8, 4, 25);

    unique_ptr<int[]> coup = make_unique<int[]>(4);
    for (int i = 0; i < it; i++) {
        g = MasterMind(8, 4, 25);
        val[i] = solve_opti(g, it);
    }
    return moyenne(val, it);
}

int main() {
    srand(time(NULL));
    unique_ptr<int[]> sol = make_unique<int[]>(3);
    unique_ptr<int[]> coup = make_unique<int[]>(4);
    sol.reset(new int[5]{0, 0, 0, 0, 0});
    coup.reset(new int[5]{0, 1, 2, 3, 0});
    MasterMind g(sol, 4, 4, 25);
    g.play(coup);
    solve_opti(g, 1000);
    cout << g << endl;
    //g.play(coup);
    //coup.reset(new int[4]{2, 1, 0});

    //cout << valeur_coup(g, coup, 500) << endl;
    //MasterMind ng(g);
    //cout << ng << endl;
    //ng.play(coup);
    //cout << ng << endl;
    //cout << valeur_coup(ng, coup, 1000) << endl;
    //cout << eval_strat(1000) << endl;
    return 0;
}