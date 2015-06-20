#include <cstdio>
#include <cstring>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <iostream>
#define MAX_VERTEX_NUM 20
using namespace std;
const int oo = 10000;

int n, m, x, y, w, d[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
void Floyd()
{
    for(int k = 1; k <= n; k++)
        for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);

}

int Check()
{
    for(int i = 1; i <= n; i++)
        if(d[i][i] < 0){
        //第i个点到第i个点距离被更新了，存在负权回路
            return false;
        }
    return true;
}

int main()
{
//    freopen("");
    printf("请输入图的顶点数和边数：");
    scanf("%d%d", &n, &m);
    memset(d, oo, sizeof(d));
    for(int i = 1; i <= n; i++)
        d[i][i] = 0;
    for(int i = 0; i < m; i++)
    {
        printf("请输入第%d条边的起点、终点以及权值：", i+1);
        scanf("%d%d%d", &x, &y, &w);
        d[x][y] = w;
    }
    Floyd();

    if(Check() == false){
        printf("存在负权回路！\n");
        return 0;
    }
    cout << "请输入起点:";
    cin >> x;
    cout << endl;
    printf("编号为%d的点到其他点的最短路为\n", x);
    for(int i = 1; i <= n; i++)
        if(d[x][i] >= oo)
            printf("%d->%d: oo\n", x, i);
        else
            printf("%d->%d: %d\n", x, i, d[x][i]);
    system("pause");
    return 0;
}
