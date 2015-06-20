#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <queue>
#include <cstring>
#define N 1006
using namespace std;

struct node{
    int x, y, w, next;
}e[100006];
int a[N], d[N], head[N],
    ans, tot, n, m, x, y, w;
bool v[N];
queue <int> Q;
void addEdge(int x,int y,int w)
{
    tot++;
    e[tot].x = x; e[tot].y = y; e[tot].w = w;
    e[tot].next = head[x]; head[x] = tot;
}

void spfa(int S)
{
    int x, k;
    memset(d, 0x3f, sizeof(d));
    memset(v, 0, sizeof(v));
    Q.push(S); v[S] = true; //将源点标记为在队列中
    d[S] = 0;
    while(!Q.empty()) //队列空时说明最短路径已经稳定下来了
    {
        x = Q.front(); Q.pop(); //取队首元素
        v[x] = false; k = head[x];
        for (k=head[x]; k!=-1; k=e[k].next)
        {
            if(d[x]+e[k].w < d[e[k].y]) //如从S经x到y的路径长度小于现有的从S到y的路径长度
            {
                d[e[k].y] = d[x] + e[k].w;  //更新最短路径长度
                if(!v[e[k].y])  //如果顶点y不在队列中，将其加入队列并标记
                {
                    Q.push(e[k].y);
                    v[e[k].y] = true;
                }
            }
        }
    }
}
int main()
{
    printf("请输入图的顶点数和边数：");
    scanf("%d%d", &n, &m);
    memset(head, -1, sizeof(head));	//初始化head数组
    for(int i = 0; i < m; i++){
        printf("请输入第%d条边的起点、终点和权值：", i+1);
        scanf("%d%d%d", &x, &y, &w);
        addEdge(x, y, w);
        //addEdge(y, x, w); //针对无向图的操作
    }
    int st;
    printf("请输入最短路径的起点：");
    scanf("%d", &st);
    spfa(st);
    printf("从顶点%d到其他顶点的最短路径长度如下：\n", st);
    for (int i=1; i<=n; i++){
        printf("%d->%d: ", st, i);
        if (d[i] >= 0x3f3f3f3f) printf("oo");
        else printf("%d", d[i]);
        printf("\n");
    }
    return 0;
}
