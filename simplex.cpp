#include "simplex.h"
#include <bits/stdc++.h>
using namespace std;

void Data::getL()
{
    cout << "Is it min or max task? (-1) -min, 1 - max: ";
    cin >> f;
    cout << "Please, enter number of variables: ";
    cin >> n;
    c = new T[n];
    cout << "Enter " << n << " coefficients of the objective function: \n";
    for (int i=0; i<n; i++)
    {
        cout << "x" << (i+1) << ": ";
        cin >> c[i];
        if (abs(c[i])>M) M=abs(c[i])+1;
    }
    return;
}

void Data::getConditions()
{
    cout << "Please, enter number of conditions: ";
    cin >> m;
    a= new T*[m];
    cout << "Please, enter matrix " << m << "x" << n+1 << ": \n";
    cout << "//First " << n << " are coefficients that variables are multiplied by and the last one is constant (b[i])\n";
    for (int i=0; i<m; i++)
    {
        a[i] = new T[n+m+1];
        for (int j=0; j<=n; j++)
        {
            cin >> a[i][j];
            if (abs(a[i][j])>M) M=abs(a[i][j])+1;
        }
    }
}

int Simplex::init()
{
    del = new T[n];
    x_bas = new int [m];
    for (int i=0; i<m; i++)
        x_bas[i] = -1;
    for (int i=0; i<n; i++)
    {
        int flag = 1;
        int pos = -1;
        for (int j=0; j<m; j++)
        {
            if (a[j][i] == 1)
            {
                if (flag != 1) flag = -1; // знайшли другу 1
                else {pos = j; flag = 0;} //знайшли одну 1
            }
            if (a[j][i] == -1)
            {
                if (flag == 1) {pos = j; flag = -2;}
                else flag = -1;
            }
            if (a[j][i]!=0 && a[j][i]!=1 && a[j][i]!=-1) flag = -1;
        }
        if (flag == 0) x_bas[pos]=i;
        if (flag == -2) {plusVar++; addVar(plusVar, pos); x_bas[pos]=n+plusVar-1;}
    }
    n+=plusVar;
    for (int i=0; i<m; i++)
        if (x_bas[i] == -1) return -1;
    return 1;
}

void Simplex::addVar(int k, int p)
{
    for (int i=0; i<m; i++)
    {
        a[i][n+k]=a[i][n+k-1];
        a[i][n+k-1]=0;
    }
    a[p][n+k-1]=1;
    c[n] = (-1)*f*M;
}

int Simplex::countDelta()
{
    int minDel = -2;
    T maximum = 0;

    for (int j=0; j<n; j++)
    {
        bool flag = true;
        T sum = 0;
        for (int i=0; i<m; i++)
        {
            sum += (c[x_bas[i]]*a[i][j]);
            if (a[i][j] > 0) flag = false;
        }
        del[j] =f*(sum-c[j]);
        if (flag && (del[j] < 0)) return -1;
        if (del[j] < maximum)
        {
            maximum = del[j];
            minDel = j;
        }
    }
    return minDel;
}

int Simplex::countTeta(int k)
{
    int l = -1;
    T teta = -1;
    for (int i=0; i<m; i++)
    {
        T xx = a[i][n]/a[i][k];
        if (a[i][k] > 0)
            if (l == -1 || teta > xx) {teta = xx; l = i;}
    }
    return l;
}

void Simplex::recount(int k, int l)
{
    T** b = new T*[m];
    for (int i=0; i<m; i++)
    {
        b[i]=new T[n+1];
        for (int j=0; j<=n; j++)
            b[i][j]=a[i][j];
    }
    T el = a[l][k];
    for (int i=0; i<m; i++)
    {
        if (i==l) continue;
        for (int j=0; j<=n; j++)
            a[i][j]=b[i][j]-(b[l][j]*b[i][k])/el;
    }
    for (int i=0; i<=n; i++)
        a[l][i] /= el;
    x_bas[l]=k;
    for (int i=0; i<m; i++)
        delete [] b[i];
    delete b;
}

void Simplex::getResult()
{

    T ans;
    T* ansVector = new T[n];
    for (int i=0; i<n; i++)
        ansVector[i] = 0;
    for (int i=0; i<m; i++)
    {
        ansVector[x_bas[i]] = a[i][n];
        ans+=(c[x_bas[i]]*a[i][n]);
    }
    cout << "Optimal solution: (";
    for (int i=0; i<n-1-plusVar; i++)
        cout << ansVector[i] << ", ";
    cout << ansVector[n-1-plusVar] << ")\n";
    cout << "L = " << ans;
}

void Simplex::Method()
{
    int check = init();
    if (check == -1) {cout << "Something wrong";return;}

    bool flag = 1;
    while (flag)
    {
        int k = countDelta();
        if (k==-1)
        {
            cout << "Sorry, there is no answer :(";
            return;
        }
        if (k==-2)
        {
            flag = false;
            getResult();
            continue;
        }
        int l = countTeta(k);
        recount(k,l);
    }

    return;
}



