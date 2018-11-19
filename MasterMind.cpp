//
// Created by emile on 19/11/18.
//

#include "MasterMind.h"


MasterMind::MasterMind(const unique_ptr<int[]> &sol, unsigned int nc, unsigned int np, unsigned int cmax) {
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

MasterMind::MasterMind(unsigned int nc, unsigned int np, unsigned int cmax) {
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

MasterMind::MasterMind(MasterMind &g) {
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

MasterMind::MasterMind(MasterMind g, unique_ptr<int[]> &nsol) {
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

MasterMind &MasterMind::operator=(MasterMind const &g) {
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

ostream &operator<<(ostream &s, MasterMind &game) {
    s << "MasterMind\n"
      << "    solution : ";
    output_tabb(s, game.solution, game.n_pins);
    s << "\n";
    for (int c = game.n_coups - 1; c >= 0; c--) {
        s << "    coup #" << c + 1 << "  : ";
        output_tabb(s, game.tab_coups[c], game.n_pins);
        s << " N: " << game.tab_noirs[c] << " B: " << game.tab_blancs[c] << "\n";
    }
    return s;
}

void MasterMind::change_sol(unique_ptr<int[]> &nsol) {
    copy_array(nsol, solution, n_pins);

}

bool MasterMind::play(const unique_ptr<int[]> &coup, bool log) {
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



