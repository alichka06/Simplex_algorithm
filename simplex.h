#ifndef SIMPLEX_H_INCLUDED
#define SIMPLEX_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;

typedef double T;

class Data
{
    protected:
        int n, m;
        T * c;
        T ** a;
        int f;//-1 -minimum, 1-maximum
        T M;
    public:
        Data(){};
        void getL(); //���������� ������� �-���
        void getConditions(); //������� � � �������� b
};

class Simplex: public Data
{
    private:
        T* del;
        int * x_bas;
        int plusVar;
    public:
        Simplex(){M=0; plusVar=0;};
        int init(); //���� �������� ������ �������
        void addVar(int k, int p);
        int countDelta();
        int countTeta(int k);
        void recount(int k, int l);
        void getResult();
        void Method();
};

#endif // SIMPLEX_H_INCLUDED
