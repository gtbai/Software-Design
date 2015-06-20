//有向图无环路的最快算法，拓扑排序再扫描， o(n + m)
#include <iostream>
#include <cstdio>
#include <stack>
#include <queue>
#include <cstring>
const int N = 100;
using namespace std;
struct Edge{
    int ed, w; //边的终点编号和权值
    Edge* next;
    Edge(int _ed, int _w, Edge* _next)
    {
        ed = _ed; w = _w; next = _next;
    }
};

struct Node{
    int du; //记录此点的入度
    Edge* e; //邻接表存储此点所连的边
    Node()
    {
        du = 0; e = NULL;
    }
};

int n, m, x, y, w, d[N];
Node* Graph[N];

int main()
{
    printf("请输入图的顶点数和边数：");
    cin >> n >> m;
    for(int i = 1; i <= n; i++)
        Graph[i] = new Node();
    for(int i = 0; i < m; i++){
        printf("请输入第%d条边的起点，终点和权值：", i+1);
        scanf("%d%d%d", &x, &y, &w);
        Graph[x]->e = new Edge(y, w, Graph[x]->e);
        Graph[y]->du++; //更新其入度
    }
    stack<int> S; //存储入度为零的点
    queue<int> Q; //存储拓扑序列
    while(!S.empty()) S.pop();
    while(!Q.empty()) Q.pop();
    for(int i = 1; i <= n; i++)
        if(Graph[i]->du == 0) S.push(i);
    //根据入度进行拓扑排序
    while(!S.empty()){
        x = S.top(); S.pop();
        Q.push(x);
        for(Edge* p = Graph[x]->e; p != NULL; p = p->next)
        {
            Graph[p->ed]->du--;
            if(Graph[p->ed]->du == 0) S.push(p->ed);
        }
    }
    memset(d, 0x3f, sizeof(d));
    printf("请输入起点：");
    cin >> y; d[y] = 0;
    //按拓扑序列以每个点为起点更新相连的点的最短距离
    while(!Q.empty()){
        x = Q.front(); Q.pop();
        for(Edge* p = Graph[x]->e; p != NULL; p = p->next)
            d[p->ed] = min(d[p->ed], d[x] + p->w);
    }
    printf("编号为%d的点到其余所有点的最短路径长度为：\n", y);
    for(int i = 1; i <= n; i++){
        printf("%d->%d: ", y, i);
        if(d[i] == 0x3f3f3f3f)
            printf("oo");
        else
            printf("%d", d[i]);
        printf("\n");
    }
}
