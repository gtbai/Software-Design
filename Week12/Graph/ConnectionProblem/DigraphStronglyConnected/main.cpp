/*利用Tarjan算法求有向图的强连通分量*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#define MAX_VERTEX_NUM 20
using namespace std;

/*有向图的邻接表存储结构*/
typedef struct ArcNode{
    int adjvex; //该边所指向顶点在图中的位置
    ArcNode *next;  //指向下一条边的指针
}ArcNode;
typedef struct VNode{
    char data;  //顶点信息
    ArcNode *firstarc;  //指向依附于该顶点的第一条边的指针
}VNode, AdjList[MAX_VERTEX_NUM];
typedef struct{
    AdjList vertices;   //存放顶点链表的数组
    int vexnum, arcnum; //分别存放顶点数和边数
}DiGraph;

stack<int> vertexstk;   //用于存放顶点的栈

int LocateVex(DiGraph &G, char vexdata);  //在图G中确定信息为data的结点位置
void CreateDG(DiGraph &G);  //创建有向图
void FindStrConnectedComponent(DiGraph &G); //在有向图G中寻找并输出强连通分量
void Tarjan(DiGraph &G, int v0);    //在图G中从编号为v0的顶点开始运用Tarjan算法

int count;                      //用于计数DFS次序
int visited[MAX_VERTEX_NUM];    //用于存放DFS顶点被访问的次序
int low[MAX_VERTEX_NUM];        //用于存放u或u的子树能够追溯到DFS时最早被访问的顶点
bool instack[MAX_VERTEX_NUM];    //用于记录顶点是否在栈中

int main()
{
    DiGraph G;
    CreateDG(G);    //创建有向图
    FindStrConnectedComponent(G);   //用Tarjan算法寻找强连通分量
    system("pause");
    return 0;
}

int LocateVex(DiGraph &G, char vexdata){
    /*在图中寻找结点信息为vexdata的顶点，返回其在图中位置*/
    for (int i=0; i<G.vexnum; i++)
        if (vexdata == G.vertices[i].data) return i;
    return -1;
}

void CreateDG(DiGraph &G){
    /*创建有向图*/
    int i;
    printf("请输入有向图的结点个数以及边的个数（中间以空格隔开）：");
    scanf("%d %d", &G.vexnum, &G.arcnum);
    getchar();
    for (i=0; i<G.vexnum; i++){
        printf("请输入第 %d 个结点的信息：", i+1);
        G.vertices[i].data = getchar();
        getchar();
        G.vertices[i].firstarc = NULL;
    }
    for (i=0; i<G.arcnum; i++){
        char sour, dest;
        printf("请输入第 %d 个边的起点和终点（中间以空格隔开）：", i+1);
        scanf("%c %c", &sour, &dest);
        getchar();
        ArcNode* arc = (ArcNode*) malloc (sizeof(ArcNode));
        arc->adjvex = LocateVex(G, dest);
        arc->next = G.vertices[LocateVex(G, sour)].firstarc;
        G.vertices[LocateVex(G, sour)].firstarc = arc;
    }
}

void FindStrConnectedComponent(DiGraph &G){
    /*在有向图G中寻找并输出强连通分量*/
    count = 0;  //计数器初始化
    memset(visited, 0, sizeof(visited));    //数组初始化
    memset(low, 0, sizeof(low));
    memset(instack, 0, sizeof(instack));
    for (int i=0; i<G.vexnum; i++)
        if (!visited[i]) Tarjan(G, i);
}

void Tarjan(DiGraph &G, int v0){
    /*在图G中从编号为v0的顶点开始运用Tarjan算法*/
    int mini = visited[v0] = ++count;
    vertexstk.push(v0); //将顶点v0压入栈中
    instack[v0] = true;
    for (ArcNode* ptr = G.vertices[v0].firstarc; ptr; ptr=ptr->next){
        //考虑v0的每个邻接顶点
        int w = ptr->adjvex;
        if (!visited[w]){   //w是从未访问过的顶点
            Tarjan(G, w);   //先求出low[w]
            if (low[w] < mini) mini = low[w];
        }else if (instack[w] && visited[w] < mini){
            mini = visited[w];
        }
    }
    low[v0] = mini;
    if (visited[v0] == low[v0]){    //那么以v0为根的DFS子树是一个强连通分量
        printf("新的强连通分量："); //打印强连通分量
        int tmp;
        do{
            tmp = vertexstk.top();  vertexstk.pop();    //弹出栈顶元素
            instack[tmp] = false;
            printf("%c ", G.vertices[tmp].data);
        }while (tmp != v0);
        printf("\n");
    }
}
