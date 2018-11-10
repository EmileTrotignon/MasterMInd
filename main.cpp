#include <iostream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory>

#define N 2000

using namespace std;

template class std::unique_ptr<int[]>;

template <typename T>
void copy_array(const unique_ptr<T> &in, unique_ptr<T> &out, int t) {
    for (int i = 0; i < t; i++) {
        out[i] = in[i];
    }
}

void initialize(unique_ptr<int[]> &tab, int t) {
    for (int i = 0; i < t; i++) tab[t] = 0;
}

void initialize(unique_ptr<int[]> &tab, int t, const int *ptrr) {
    for (int i = 0; i < t; i++) tab[t] = ptrr[t];
}


void output_tabb(ostream& s, const unique_ptr<int[]> &tab, int t){
    for (int i=0; i<t; i++) s << tab[i] << " ";
}

bool in_array(int e, const unique_ptr<int[]> &tab, int t){
    for (int i=0; i < t; i++){
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


class MasterMind{

    unique_ptr<int[]>             solution;

public:

    unsigned int                  n_couleurs;
    unsigned int                  n_pins;
    unsigned int                  n_coups_max;
    unsigned int                  n_coups;
    unique_ptr<int[]>             tab_noirs;
    unique_ptr<int[]>             tab_blancs;
    unique_ptr<unique_ptr<int[]>[]> tab_coups;

    MasterMind(unique_ptr<int[]> &sol, unsigned int nc, unsigned int np, unsigned int cmax){
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

    MasterMind(unsigned int nc, unsigned int np, unsigned int cmax){
        n_couleurs = nc;
        n_pins = np;
        n_coups_max = cmax;
        n_coups = 0;
        auto uptr = make_unique<int[]>(np);
        solution.swap(uptr);
        for (int p=0; p<np; p++) solution[p] = rand() % nc;

        auto tb = make_unique<int[]>(cmax);
        auto tn = make_unique<int[]>(cmax);
        auto tc = make_unique<unique_ptr<int[]>[]>(cmax);

        tab_blancs.swap(tb);
        tab_noirs.swap(tn);
        tab_coups.swap(tc);
    }

    MasterMind(MasterMind &g){
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
    }

    ~MasterMind(){
        /*for (int i=0; i < n_coups_max; i++) if (tab_coups[i]) delete[] tab_coups[i];
        if (tab_coups) delete[] tab_coups;
        tab_coups = nullptr;
        if (tab_blancs) delete[] tab_blancs;
        tab_blancs = nullptr;
        if (tab_noirs) delete[] tab_noirs;
        tab_noirs = nullptr;
        if (solution) delete[] solution;
        solution = nullptr;*/
    }

    MasterMind &operator=(MasterMind const &g){
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

    friend ostream& operator<<(ostream &s, MasterMind &game){
        s << "MasterMind\n"
          << "    solution : ";
        output_tabb(s, game.solution, game.n_pins);
        s << "\n";
        for (int c=game.n_coups-1; c>=0; c--){
            s <<  "    coup #" << c << "  : ";
            output_tabb(s, game.tab_coups[c], game.n_pins);
            s << " N: " << game.tab_noirs[c] << " B: " << game.tab_blancs[c] << "\n";
        }
        return s;
    }

    bool play(const unique_ptr<int[]> &coup, bool log=false) {
        if (n_coups == n_coups_max - 1) return true;
        int noirs = 0;
        int blancs = 0;
        for (int p=0; p < n_pins; p++) {
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
        return noirs == n_pins;
    }
};

void next_alphanum(unique_ptr<int[]> &tab, int base, int t) {
    for (int i=0; i < t; i++) {
        if (tab[i] == base - 1) tab[i] = 0;
        else {
            tab[i]++;
            return;
        }
    }
}

bool check_sol(const MasterMind &game, unique_ptr<int[]> &test_sol) {
    MasterMind test_game(test_sol, game.n_coups, game.n_pins, game.n_coups_max);

    for (int c=0; c < game.n_coups; c++) {
        test_game.play(game.tab_coups[c]);
        if ((test_game.tab_noirs[c] != game.tab_noirs[c]) || (test_game.tab_blancs[c] != game.tab_blancs[c]))
            return false;
    }
    return true;
}

int solve(MasterMind &game) {
    auto next_coup = make_unique<int[]>(game.n_pins);
    initialize(next_coup, game.n_pins);
    while (!game.play(next_coup)) {
        initialize(next_coup, game.n_pins);
        while (!check_sol(game, next_coup)) {
            next_alphanum(next_coup, game.n_couleurs, game.n_pins);
        }
        //cout << game << endl;
    }
    return game.n_coups;
}

float moyenne(const int *tab, int t){
    float s = 0;
    for (int i=0; i<t; i++){
        s += tab[i];
    }
    return s / t;
}

int main() {
    srand(time(NULL));
    unique_ptr<int[]> sol;
    sol.reset(new int[4]{7, 7, 7, 7});
    //int solt[4] = {7, 7, 7, 7};
    //initialize(sol, 4, solt);;
    MasterMind g(sol, 8, 4, 25);
    int n;
    n = solve(g);
    cout << g;
    std::cout << n << std::endl;

    int *val = new int[N];
    for (int i=0; i<N; i++){
        g = MasterMind(8, 4, 25);
        val[i] = solve(g);
    }
    cout << "\n" << moyenne(val, N) << endl;
    return 0;
}