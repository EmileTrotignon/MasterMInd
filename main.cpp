#include <iostream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

template <typename T>
void copy_array(const T *in, T *out, int t){
    for (int i=0; i<t; i++){
        out[i] = in[i];
    }
}

void output_tabb(ostream& s, const int *tab, int t){
    for (int i=0; i<t; i++) s << tab[i] << " ";
}

bool in_array(int e, const int *tab, int t){
    for (int i=0; i < t; i++){
        if (e == tab[i]) return true;
    }
    return false;
}

int index(int e, const int *tab, int t){
    for (int i=0; i < t; i++){
        if (e == tab[i]) return i;
    }
}

bool in_array_ordered(const int *t1, const int *t2, int i, int t) {
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

    const int *solution;

public:

    int n_couleurs;
    int n_pins;
    int n_coups_max;
    int n_coups;
    int *tab_noirs;
    int *tab_blancs;
    int **tab_coups;

    MasterMind(const int *sol, int nc, int np, int cmax){
        solution = sol;
        n_couleurs = nc;
        n_pins = np;
        n_coups_max = cmax;
        n_coups = 0;
        tab_blancs = new int[cmax];
        tab_noirs = new int[cmax];
        tab_coups = new int *[cmax];
    }

    MasterMind(int nc, int np, int cmax){
        auto sol = new int[np];
        for (int p=0; p<np; p++) sol[p] = rand() % nc;
        solution = sol;
        n_couleurs = nc;
        n_pins = np;
        n_coups_max = cmax;
        n_coups = 0;
        tab_blancs = new int[cmax];
        tab_noirs = new int[cmax];
        tab_coups = new int *[cmax];
    }

    ~MasterMind(){
        for (int i=0; i < n_coups; i++) delete[] tab_coups[i];
    }

    friend ostream& operator<<(ostream &s, MasterMind game){
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

    bool play(const int *coup, bool log=false) {
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
        tab_coups[n_coups] = new int[n_pins];
        copy_array<int>(coup, tab_coups[n_coups], n_pins);
        n_coups++;
        return noirs == n_pins;
    }

};

void next_alphanum(int *tab, int base, int t) {
    for (int i=0; i < t; i++) {
        if (tab[i] == base - 1) tab[i] = 0;
        else {
            tab[i]++;
            return;
        }
    }
}

bool check_sol(const MasterMind &game, const int *test_sol) {
    MasterMind test_game(test_sol, game.n_coups, game.n_pins, game.n_coups_max);

    for (int c=0; c < game.n_coups; c++) {
        test_game.play(game.tab_coups[c]);
        if ((test_game.tab_noirs[c] != game.tab_noirs[c]) || (test_game.tab_blancs[c] != game.tab_blancs[c]))
            return false;
    }
    return true;
}

int solve(MasterMind &game) {
    auto next_coup = new int[game.n_pins] {0};

    while (!game.play(next_coup)) {
        delete[] next_coup;
        next_coup = new int[game.n_pins]{0};
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
    int sol[] = {7, 7, 7, 7};
    int solt[] = {3, 2, 1, 2};
    int coupt[] = {2, 2, 2, 1};
    MasterMind gt(solt, 8, 4, 25);
    gt.play(coupt, true);
    cout << gt << endl;
    MasterMind g(sol, 8, 4, 25);
    int n;
    cout << in_array(1, sol, 4) << endl;
    n = solve(g);
    cout << g;
    std::cout << n << std::endl;

    int *val = new int[2000];
    for (int i=0; i<2000; i++){
        g = MasterMind(8, 4, 25);
        val[i] = solve(g);
    }
    cout << "\n" << moyenne(val, 2000) << endl;
    return 0;
}