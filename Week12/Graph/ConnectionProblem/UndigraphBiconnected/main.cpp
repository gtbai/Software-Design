#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#define MAX_VERTEX_NUM 20

using namespace std;
/*无向图的邻接表存储结构*/
typedef struct ArcNode{
    int adjvex; //该边所指向顶点在图中的位置
    ArcNode *next;  //指向下一条边的指针
    bool been;  //标识是否已经走过该条边
}ArcNode;
typedef struct VNode{
    char data;  //顶点信息
    ArcNode *firstarc;  //指向依附于该顶点的第一条边的指针
}VNode, AdjList[MAX_VERTEX_NUM];
typedef struct{
    AdjList vertices;   //存放顶点链表的数组
    int vexnum, arcnum; //分别存放顶点数和边数
}UndiGraph;
typedef struct Edge{
    int sour;   //边的起点
    int dest;   //边的终点
}Edge;

void CreateUDG(UndiGraph &G);   //创建无向图
int LocateVex(UndiGraph &G, char vexdata);    //在图中寻找结点信息为vexdata的顶点，返回其在图中位置
void FindBiComponent(UndiGraph &G);     //在无向图G中查找并输出所有双联通分量
void DFSBiComponet(UndiGraph &G, int v0);   //从顶点v0出发，深度优先遍历图G，查找并输出双联通分量
void PrintBiComponent(UndiGraph &G, int sour, int dest);  //打印双连通分量

stack<Edge> edgestk;
int count, visited[MAX_VERTEX_NUM] = {0}, low[MAX_VERTEX_NUM] = {0};
int main()
{
    UndiGraph G;
    CreateUDG(G);
    DFSBiComponet(G, 0);
    return 0;
}

int LocateVex(UndiGraph &G, char vexdata){
    /*在图中寻找结点信息为vexdata的顶点，返回其在图中位置*/
    for (int i=0; i<G.vexnum; i++)
        if (vexdata == G.vertices[i].data) return i;
    return -1;
}

void CreateUDG(UndiGraph &G){
    /*创建无向图*/
    int i;
    printf("请输入无向图的结点个数以及边个数（中间以空格隔开）：");
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
        arc->been = false;
        arc->adjvex = LocateVex(G, dest);
        arc->next = G.vertices[LocateVex(G, sour)].firstarc;
        G.vertices[LocateVex(G, sour)].firstarc = arc;
        ArcNode* arc2 = (ArcNode*) malloc (sizeof(ArcNode));
        arc2->been = false;
        arc2->adjvex = LocateVex(G, sour);
        arc2->next = G.vertices[LocateVex(G, dest)].firstarc;
        G.vertices[LocateVex(G, dest)].firstarc = arc2;
    }
}
void DFSBiComponet(UndiGraph &G, int v0){
    /*从顶点v0出发，深度优先遍历图G，查找并输出双联通分量*/
    int mini = visited[v0] = ++count;   //给顶点v0进行深度优先编号
    for (ArcNode* ptr=G.vertices[v0].firstarc; ptr; ptr = ptr->next){
        //检查第v0个顶点的所有邻接顶点
        int w = ptr->adjvex;
        if (!ptr->been){
            Edge edge = {v0, w};
            edgestk.push(edge);
            ptr->been = true;
            for (ArcNode* ptr2=G.vertices[w].firstarc; ptr2; ptr2=ptr2->next){
                if (ptr2->adjvex == v0) {
                    ptr2->been = true; break;
                }
            }
        }
            if (!visited[w]){   //w是未访问过的新顶点
                DFSBiComponet(G, w);    //先求得low[w]
                if (low[w]<mini) mini = low[w];
                if (low[w]>=visited[v0]){   //v0是关节点
                PrintBiComponent(G, v0, w);
                }
            }else if(visited[w] < mini) //w是祖先结点
                mini = visited[w];
        }
        low[v0] = mini;
    }
void PrintBiComponent(UndiGraph &G, int sour, int dest){
    /*打印双连通分量*/
    printf("割点: %c ", G.vertices[sour].data);
    printf("新的双连通分量：");
    while(!((edgestk.top().sour==sour) && (edgestk.top().dest==dest))){
        Edge topedge = edgestk.top(); edgestk.pop();
        printf("<%c,%c>", G.vertices[topedge.sour].data, G.vertices[topedge.dest].data);
    }
    printf("<%c,%c>", G.vertices[sour].data, G.vertices[dest].data);
    printf("\n");
}
