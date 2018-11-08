#include <iostream>

using namespace std;

bool in_array(int e, const int *tab, int t){
    for (int i=0; i < t; i++){
        if (e == tab[i]) return true;
    }
    return false;
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

    ~MasterMind(){
        for (int i=0; i < n_coups; i++) delete[] tab_coups[i];
        delete[] tab_coups;
        delete[] tab_blancs;
        delete[] tab_noirs;
    }

    bool play(const int *coup) {
        int noirs = 0;
        int blancs = 0;
        for (int i=0; i < n_pins; i++) {
            if (solution[i] == coup[i]) noirs++;
            else if (in_array(coup[i], solution, n_pins)) blancs++;
        }
        tab_noirs[n_coups] = noirs;
        tab_blancs[n_coups] = blancs;
        copy(coup, coup + n_pins, tab_coups[n_coups]);
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

    for (int i=0; i < game.n_coups; i++) {
        test_game.play(game.tab_coups[i]);
        if ((test_game.tab_noirs[i] != game.tab_noirs[i]) || (test_game.tab_blancs[i] != game.tab_blancs[i]))
            return false;
    }
    return true;
}

int solve(MasterMind game) {
    cout << "coucou";
    auto next_coup = new int[game.n_pins] {0};

    while (!game.play(next_coup)) {
        delete[] next_coup;
        next_coup = new int[game.n_pins]{0};
        while (!check_sol(game, next_coup)) {
            next_alphanum(next_coup, game.n_couleurs, game.n_pins);
        }
    }
    return game.n_coups;
}

int main() {
    cout << "kkkrkr";
    int sol[] = {1, 4, 8, 9};
    MasterMind g(sol, 10, 4, 25);
    int n;
    n = solve(g);
    /*cout << "lol";
    std::cout << n << std::endl;
    return 0;*/
}