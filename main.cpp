#include <iostream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <math.h>
#include <vector>

#define N 2000

using namespace std;

template
class std::unique_ptr<int[]>;

template<typename T>
void copy_array(const unique_ptr<T> &in, unique_ptr<T> &out, int t) {
    for (int i = 0; i < t; i++) {
        out[i] = in[i];
    }
}

unsigned long fpow(long x, int p) {
    if (p == 0) return 1;
    unsigned long y = 1;
    while (p > 1) {
        if (p % 2) {
            y *= x;
            x *= x;
        } else x *= x;
        p /= 2;

    }
    return x * y;
}


void output_tabb(ostream &s, const unique_ptr<int[]> &tab, int t) {
    for (int i = 0; i < t; i++) s << tab[i] << " ";
}

void initialize(unique_ptr<int[]> &tab, int t) {
    for (int i = 0; i < t; i++) tab[i] = 0;
}

void initialize_random(unique_ptr<int[]> &tab, int b, int t) {
    for (int i = 0; i < t; i++) tab[i] = rand() % b;
}

void initialize(unique_ptr<int[]> &tab, int t, const int *ptrr) {
    for (int i = 0; i < t; i++) tab[t] = ptrr[t];
}

bool in_array(int e, const unique_ptr<int[]> &tab, int t) {
    for (int i = 0; i < t; i++) {
        if (e == tab[i]) return true;
    }
    return false;
}

bool in_array_ordered(const unique_ptr<int[]> &t1, const unique_ptr<int[]> &t2, int i, int t) {
    int e = t1[i];
    int c1 = 1;
    int c2 = 0;
    for (int k = 0; k < t; k++) {
        if ((t1[k] == e) && (k < i)) c1++;
        if (t2[k] == e) c2++;
    }
    return c1 <= c2;
}


class MasterMind {

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

    MasterMind(const unique_ptr<int[]> &sol, unsigned int nc, unsigned int np, unsigned int cmax) {
        won = false;
        n_couleurs = nc;
        n_pins = np;
        n_coups_max = cmax;
        n_coups = 0;
        auto uptr = make_unique<int[]>(np);
        solution.swap(uptr);
        copy_array(sol, solution, np);

        auto tb = make_unique<int[]>(cmax);
        auto tn = make_unique<int[]>(cmax);
        auto tc = make_unique<unique_ptr<int[]>[]>(cmax);

        tab_blancs.swap(tb);
        tab_noirs.swap(tn);
        tab_coups.swap(tc);

    }

    MasterMind(unsigned int nc, unsigned int np, unsigned int cmax) {
        won = false;
        n_couleurs = nc;
        n_pins = np;
        n_coups_max = cmax;
        n_coups = 0;
        auto uptr = make_unique<int[]>(np);
        solution.swap(uptr);
        for (int p = 0; p < np; p++) solution[p] = rand() % nc;

        auto tb = make_unique<int[]>(cmax);
        auto tn = make_unique<int[]>(cmax);
        auto tc = make_unique<unique_ptr<int[]>[]>(cmax);

        tab_blancs.swap(tb);
        tab_noirs.swap(tn);
        tab_coups.swap(tc);
    }

    MasterMind(MasterMind &g) {
        won = g.won;
        n_couleurs = g.n_couleurs;
        n_pins = g.n_pins;
        n_coups_max = g.n_coups_max;
        n_coups = 0;
        auto uptr = make_unique<int[]>(n_pins);
        solution.swap(uptr);
        copy_array(g.solution, solution, n_pins);

        auto tb = make_unique<int[]>(n_coups_max);
        auto tn = make_unique<int[]>(n_coups_max);
        auto tc = make_unique<unique_ptr<int[]>[]>(n_coups_max);

        tab_blancs.swap(tb);
        tab_noirs.swap(tn);
        tab_coups.swap(tc);
        for (int i = 0; i < g.n_coups; i++) {
            (*this).play(g.tab_coups[i]);
        }
    }

    MasterMind(MasterMind g, unique_ptr<int[]> &nsol) {
        won = g.won;
        n_couleurs = g.n_couleurs;
        n_pins = g.n_pins;
        n_coups_max = g.n_coups_max;
        n_coups = 0;
        auto uptr = make_unique<int[]>(n_pins);
        solution.swap(uptr);
        copy_array(nsol, solution, n_pins);

        auto tb = make_unique<int[]>(n_coups_max);
        auto tn = make_unique<int[]>(n_coups_max);
        auto tc = make_unique<unique_ptr<int[]>[]>(n_coups_max);

        tab_blancs.swap(tb);
        tab_noirs.swap(tn);
        tab_coups.swap(tc);
        for (int i = 0; i < g.n_coups; i++) {
            (*this).play(g.tab_coups[i]);
        }
    }

    MasterMind &operator=(MasterMind const &g) {
        won = g.won;
        n_couleurs = g.n_couleurs;
        n_pins = g.n_pins;
        n_coups_max = g.n_coups_max;
        n_coups = g.n_coups;
        auto uptr = make_unique<int[]>(n_pins);
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

    friend ostream &operator<<(ostream &s, MasterMind &game) {
        s << "MasterMind\n"
          << "    solution : ";
        output_tabb(s, game.solution, game.n_pins);
        s << "\n";
        for (int c = game.n_coups - 1; c >= 0; c--) {
            s << "    coup #" << c << "  : ";
            output_tabb(s, game.tab_coups[c], game.n_pins);
            s << " N: " << game.tab_noirs[c] << " B: " << game.tab_blancs[c] << "\n";
        }
        return s;
    }

    void change_sol(unique_ptr<int[]> &nsol) {
        copy_array(nsol, solution, n_pins);

    }

    bool play(const unique_ptr<int[]> &coup, bool log = false) {
        if (n_coups == n_coups_max - 1) {
            won = true;
            return true;
        }
        int noirs = 0;
        int blancs = 0;
        for (int p = 0; p < n_pins; p++) {
            if (solution[p] == coup[p]) {
                noirs++;
                blancs--;
            }
            if (in_array_ordered(coup, solution, p, n_pins)) {
                if (log) {
                    output_tabb(cout, solution, n_pins);
                    cout << "    " << coup[p] << "  " << p << endl;
                }
                blancs++;
            }
        }
        tab_noirs[n_coups] = noirs;
        tab_blancs[n_coups] = blancs;

        auto new_coup = make_unique<int[]>(n_pins);
        tab_coups[n_coups].swap(new_coup);
        copy_array(coup, tab_coups[n_coups], n_pins);
        n_coups++;
        won = (noirs == n_pins);
        return won;
    }
};

void next_alphanum(unique_ptr<int[]> &tab, int base, int t) {
    for (int i = 0; i < t; i++) {
        if (tab[i] == base - 1) tab[i] = 0;
        else {
            tab[i]++;
            return;
        }
    }
}

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
    sol.reset(new int[5]{2, 1, 0, 3});
    unique_ptr<int[]> coup = make_unique<int[]>(4);
    coup.reset(new int[4]{1, 2, 2, 3});
    MasterMind g(sol, 4, 4, 25);
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