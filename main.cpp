#include <iostream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <math.h>
#include <vector>
#include <array>
#include "MasterMind.h"
#include "lib_perso.h"


using namespace std;

template<size_t size>
void all_alphanum(vector<array<int, size>> &v, int n_couleurs)
{
    array<int, size> next_coup{0};
    for (int i = 0; i < v.size(); i++)
    {
        v[i] = next_coup;
        next_alphanum<size>(next_coup, n_couleurs);
    }
}

template<size_t n_pins, size_t n_colors>
bool check_sol(const MasterMind<n_pins, n_colors> &game, const array<int, n_pins> &test_sol)
{
    MasterMind<n_pins, n_colors> test_game(test_sol, game.n_coups_max);

    for (int c = 0; c < game.tab_coups.size(); c++)
    {
        test_game.play(game.tab_coups[c]);
        if ((test_game.tab_noirs[c] != game.tab_noirs[c]) || (test_game.tab_blancs[c] != game.tab_blancs[c]))
            return false;
    }
    return true;
}

template<size_t n_pins, size_t n_colors>
int solve_random(MasterMind<n_pins, n_colors> &game)
{
    auto next_coup = make_unique<int[]>(game.n_pins);
    initialize(next_coup, game.n_pins);
    while (!game.play(next_coup))
    {
        initialize_random(next_coup, game.n_couleurs, game.n_pins);
        while (!check_sol(game, next_coup))
        {
            initialize_random(next_coup, game.n_couleurs, game.n_pins);
        }
        //cout << game << endl;
    }
    return game.n_coups;
}

float moyenne(const int *tab, int t)
{
    float s = 0;
    for (int i = 0; i < t; i++)
    {
        s += tab[i];
    }
    return s / t;
}

template<size_t n_pins, size_t n_colors>
double valeur_situation(MasterMind<n_pins, n_colors> &g, vector<array<int, n_pins>> &coups_possible)
{
    int val = 0;
    for (int i = 0; i < coups_possible.size(); i++) if (check_sol(g, coups_possible[i])) val++;
    return ((double) val) / coups_possible.size();
}


template<size_t n_pins, size_t n_colors>
double
valeur_situation(MasterMind<n_pins, n_colors> &g, int prec, vector<array<int, n_pins>> &coups_possible)
{
    if (prec > coups_possible.size()) return valeur_situation(g, coups_possible);
    int val = 0;
    for (int i = 0; i < prec; i++)
    {
        if (check_sol(g, coups_possible[rand() % coups_possible.size()])) val++;
    }
    return ((double) val) / prec;
}


template<size_t n_pins, size_t n_colors>
double valeur_situation(MasterMind<n_pins, n_colors> &g, int prec)
{
    int val = 0;
    array<int, n_pins> psol;
    initialize_random<n_pins>(psol, g.n_couleurs);
    for (int i = 0; i < prec; i++)
    {
        g.n_coups--;
        while (!check_sol<n_pins, n_colors>(g, psol)) initialize_random(psol, g.n_couleurs, g.n_pins);
        g.n_coups++;
        //cout << i << endl;
        if (check_sol(g, psol)) val++;

    }
    return ((double) val) / prec;
}

template<size_t n_pins, size_t n_colors>
double
valeur_coup(MasterMind<n_pins, n_colors> &g, array<int, n_pins> &coup, vector<array<int, n_pins>> &coups_possible)
{
    double val = 0;
    array<int, n_pins> rsol = coups_possible[0];
    MasterMind<n_pins, n_colors> ng(g);
    ng.change_sol(rsol);
    ng.play(coup);
    val += valeur_situation<n_pins, n_colors>(ng, coups_possible);

    for (int i = 1; i < coups_possible.size(); i++)
    {
        rsol = coups_possible[i];
        ng.change_sol(rsol);
        ng.unplay_last_coup();
        ng.play(coup);
        val += valeur_situation(ng, coups_possible);
    }
    return val / coups_possible.size();
}


template<size_t n_pins, size_t n_colors>
double valeur_coup(MasterMind<n_pins, n_colors> &g, unique_ptr<int[]> &coup, int prec,
                   vector<unique_ptr<int[]>> &coups_possible)
{
    double val = 0;
    auto rsol = make_unique<int[]>(g.n_pins);
    copy_array(coups_possible[rand() % coups_possible.size()], rsol, g.n_pins);
    MasterMind<n_pins, n_colors> ng(g);
    ng.change_sol(rsol);
    ng.play(coup);

    for (int i = 0; i < prec; i++)
    {
        val += valeur_situation(ng, prec / 2, coups_possible, coups_possible.size());
        //cout << val << endl;
        rsol = coups_possible[rand() % coups_possible.size()];
        ng.change_sol(rsol);
        ng.tab_coups.pop_back();
        ng.play(coup);
    }
    if (val / prec == 0)
    {
        cout << g << endl;
        output_tabb(cout, coup, g.n_pins);
        cout << endl;
        cout << prec << endl;

    }
    return val / prec;
}


template<size_t n_pins, size_t n_colors>
double valeur_coup(MasterMind<n_pins, n_colors> &g, unique_ptr<int[]> &coup, int prec)
{
    double val = 0;
    auto rsol = make_unique<int[]>(g.n_pins);
    while (!check_sol(g, rsol)) initialize_random(rsol, g.n_couleurs, g.n_pins);
    MasterMind<n_pins, n_colors> ng(g);
    ng.change_sol(rsol);
    ng.play(coup);

    for (int i = 0; i < prec; i++)
    {
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


template<size_t n_pins, size_t n_colors>
int solve_opti(MasterMind<n_pins, n_colors> &g, int prec)
{
    unsigned long n_psol = fpow(n_colors, n_pins);
    double v_best_coup;
    double v_coup;

    vector<array<int, n_pins>> coups_possibles(n_psol, {0});
    all_alphanum<n_pins>(coups_possibles, n_colors);

    array<int, n_pins> next_coup({0});

    array<int, n_pins> first_coup{0};
    for (int i = 0; i < n_pins; i++) first_coup[i] = i % n_colors;
    //g.play({0, 0, 2});
    while (!g.won)
    {
        v_best_coup = 2;
        for (unsigned long i = coups_possibles.size() - 1; i >= 0 && i <= coups_possibles.size() + 10; i--)
        {
            if (i == coups_possibles.size()) i--;
            /*cout << "tested coup : ";
            output_tabb(cout, coups_possibles[i], g.n_pins);
            cout << endl;
            cout << "current best coup : " ;
            output_tabb(cout, next_coup, g.n_pins);
            cout << endl;
            cout << i << " / " << coups_possibles.size() << " / ";
            output_tabb<n_pins>(cout, coups_possibles[i]);
            cout << endl;*/
            if (check_sol(g, coups_possibles[i]))
            {
                v_coup = valeur_coup(g, coups_possibles[i], coups_possibles);
                /*cout << "vcoup : " << v_coup << endl;
                cout << "v_best_coup : " << v_best_coup << endl << endl;*/
                if (v_coup < v_best_coup)
                {
                    v_best_coup = v_coup;
                    next_coup = coups_possibles[i];
                }
            } else
            {
                coups_possibles.erase(coups_possibles.begin() + i);
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
    return g.tab_coups.size();
}

template<size_t n_pins, size_t n_colors>
double eval_strat(int it)
{
    int *val = new int[it];
    MasterMind<n_pins, n_colors> g(25);

    for (int i = 0; i < it; i++)
    {
        g = MasterMind<n_pins, n_colors>(25);
        val[i] = solve_opti(g, it);
    }
    return moyenne(val, it);
}

int main()
{
    srand(time(NULL));
    //array<int, 2> sol{0};
    //initialize_random<2>(sol, 4);
    //MasterMind<4, 2> g(25);
    //solve_opti(g, 1000);
    //cout << g << endl;
    //g.play(coup);
    //coup.reset(new int[4]{2, 1, 0});

    //cout << valeur_coup(g, coup, 500) << endl;
    //MasterMind ng(g);
    //cout << ng << endl;
    //ng.play(coup);
    //cout << ng << endl;
    //cout << valeur_coup(ng, coup, 1000) << endl;
    cout << eval_strat<3, 3>(500) << endl;
    return 0;
}