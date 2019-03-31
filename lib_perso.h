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
#include <array>
#include <queue>
#include <stack>

using namespace std;

//template <class T> void copy_array(const unique_ptr<T> &in, unique_ptr<T> &out, int t);

unsigned long fpow(long x, int p);

template<size_t size>
void output_tabb(ostream &s, const array<int, size> &tab)
{
    for (int i = 0; i < size; i++) s << tab[i] << " ";
}
//void initialize(unique_ptr<int[]> &tab, int t);

template<size_t size>
void initialize_random(array<int, size> &tab, int b)
{
    for (int i = 0; i < size; i++) tab[i] = rand() % b;
}


void initialize(unique_ptr<int[]> &tab, int t, const int *ptrr);

//bool in_array(int e, const unique_ptr<int[]> &tab, int t);

template<size_t size>
bool in_array_ordered(const array<int, size> &t1, const array<int, size> &t2, int i)
{
    int e = t1[i];
    int c1 = 1;
    int c2 = 0;
    for (int k = 0; k < size; k++)
    {
        if ((t1[k] == e) && (k < i)) c1++;
        if (t2[k] == e) c2++;
    }
    return c1 <= c2;
}

template<size_t size>
void next_alphanum(array<int, size> &tab, int base)
{
    for (int i = 0; i < size; i++)
    {
        if (tab[i] == base - 1) tab[i] = 0;
        else
        {
            tab[i]++;
            return;
        }
    }
}

template<class T>
void inverse_queue(queue<T> q);

bool parenthesis(string s);

/*template<typename T>
void copy_array(const unique_ptr<T> &in, unique_ptr<T> &out, int t) {
    for (int i = 0; i < t; i++) {
        out[i] = in[i];
    }
}*/

#endif //MASTERMIND_LIB_PERSO_H
