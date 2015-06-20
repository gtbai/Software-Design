#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
const int N = 1000;
const int inf = 0x3f3f3f3f;
struct Edge{
    int ed, w;
    Edge(int _ed, int _w)
    {
        ed = _ed; w = _w;
    }
};
vector<Edge> st[N];
int n, m, d[N];
bool vis[N];
void Dijkstra(int s) {
    memset(d, 0x3f, sizeof(d)); //初始时都赋值为无穷大
    memset(vis, false, sizeof(vis)); //标记是否访问
    d[s] = 0;
    for (int i = 0; i < n; i++) {
        int k = 0, mint = inf;
        //找一个到起点距离最短的点
        for (int j = 1; j <= n; j++)
            if (!vis[j] && d[j] < mint)
                mint = d[j], k = j;
        if (k == 0) break; //没找到，说明已经更新完了
        vis[k] = true; //找到的点为k
        for (int j = 0, ed, w; j < (int)st[k].size(); j++) {
            ed = st[k][j].ed, w = st[k][j].w;
            if (!vis[ed] && d[ed] > d[k] + w)
                d[ed] = d[k] + w;
        }
    }
}
int main()
{
    //freopen("Graph.txt", "r", stdin);
    printf("请输入图的顶点数和边数：");
    scanf("%d%d", &n, &m);
    for (int i=0; i<=n; i++) st[i].clear();
    for (int i=0, a, b, c; i<m; i++) {
        printf("请输入第%d条边的起点、终点以及权值：", i+1);
        scanf("%d%d%d", &a, &b, &c);
        st[a].push_back(Edge(b, c));
        st[b].push_back(Edge(a, c));
    }
    int s;
    printf("请输入起点:");
    scanf("%d", &s);
    Dijkstra(s);
    printf("编号为%d的点到其他点的最短路如下：\n", s);
    for(int i = 1; i <= n; i++)
        printf("%d: %d\n", i, d[i]);
    system("pause");
    return 0;
}
