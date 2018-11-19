//
// Created by emile on 19/11/18.
//

#ifndef MASTERMIND_LIB_PERSO_H
#define MASTERMIND_LIB_PERSO_H

#include <iostream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <math.h>
#include <vector>

using namespace std;

template <class T>
void copy_array(const unique_ptr<T> &in, unique_ptr<T> &out, int t);
unsigned long fpow(long x, int p);
void output_tabb(ostream &s, const unique_ptr<int[]> &tab, int t);
void initialize(unique_ptr<int[]> &tab, int t);
void initialize_random(unique_ptr<int[]> &tab, int b, int t);
void initialize(unique_ptr<int[]> &tab, int t, const int *ptrr);
bool in_array(int e, const unique_ptr<int[]> &tab, int t);
bool in_array_ordered(const unique_ptr<int[]> &t1, const unique_ptr<int[]> &t2, int i, int t);
void next_alphanum(unique_ptr<int[]> &tab, int base, int t);


template<typename T>
void copy_array(const unique_ptr<T> &in, unique_ptr<T> &out, int t) {
    for (int i = 0; i < t; i++) {
        out[i] = in[i];
    }
}
#endif //MASTERMIND_LIB_PERSO_H
