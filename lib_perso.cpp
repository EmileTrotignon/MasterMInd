//
// Created by emile on 19/11/18.
//

#include "lib_perso.h"

using namespace std;


unsigned long fpow(long x, int p)
{
    if (p == 0) return 1;
    unsigned long y = 1;
    while (p > 1)
    {
        if (p % 2)
        {
            y *= x;
            x *= x;
        } else x *= x;
        p /= 2;

    }
    return x * y;
}


void initialize(unique_ptr<int[]> &tab, int t)
{
    for (int i = 0; i < t; i++) tab[i] = 0;
}

void initialize_random(unique_ptr<int[]> &tab, int b, int t)
{
    for (int i = 0; i < t; i++) tab[i] = rand() % b;
}

void initialize(unique_ptr<int[]> &tab, int t, const int *ptrr)
{
    for (int i = 0; i < t; i++) tab[t] = ptrr[t];
}

bool in_array(int e, const unique_ptr<int[]> &tab, int t)
{
    for (int i = 0; i < t; i++)
    {
        if (e == tab[i]) return true;
    }
    return false;
}

bool in_array_ordered(const unique_ptr<int[]> &t1, const unique_ptr<int[]> &t2, int i, int t)
{
    int e = t1[i];
    int c1 = 1;
    int c2 = 0;
    for (int k = 0; k < t; k++)
    {
        if ((t1[k] == e) && (k < i)) c1++;
        if (t2[k] == e) c2++;
    }
    return c1 <= c2;
}

