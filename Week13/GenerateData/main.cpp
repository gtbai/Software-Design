#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
int A[1000000];
int main()
{
    int i, pos;
    ofstream out;
    out.open("repeat.txt");
    out<<"1000000"<<endl;
//    for (i=0; i<1000000; i++) A[i] = i+1;
//    srand((unsigned)time(NULL));
//    for (i=1000000-1; i>=0; i--){
//        pos = rand()*rand() % (i+1);
//        swap(A[pos], A[i]);
//    }
    for (i=0; i<1000000; i++) out<<233<<endl;
    system("pause");
    return 0;
}
