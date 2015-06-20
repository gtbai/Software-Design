#include <iostream>
#include <stdio.h>
#include <limits.h>
#define MAX_VERTEX_NUM 20   //最大顶点个数
using namespace std;
//图的邻接矩阵存储结构
typedef int AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef struct{
    char vexs[MAX_VERTEX_NUM];   //顶点向量
    AdjMatrix arcs;     //邻接矩阵
    int vexnum, arcnum; //图的定点数和边数
}MGraph;

typedef struct { //辅助数组定义
        char adjvex;    //权值最小边的另一邻接顶点
        int lowcost;    //权值最小边的权值
}Elem;

int LocateVex(MGraph &G, char v);   //在无向图G中寻找顶点v
void CreateMGraph(MGraph &G);   //创建一个以邻接矩阵为存储结构的无向图
void MiniSpanTree(MGraph &G);   //用Prim算法从顶点v开始构造无向图G的最小生成树
int Mini(int vexnum, Elem closedge[]);  //求出最小生成树中的下一个顶点

int main()
{
    MGraph G;   //定义一个图haha
    CreateMGraph(G);    //创建这个图
    MiniSpanTree(G);    //使用Prim算法求最小生成树
    getchar();
    return 0;
}

int LocateVex(MGraph &G, char v){
        /*在无向图G中寻找顶点V*/
        for (int i=0; i<G.vexnum; i++){
            if (v == G.vexs[i]){
                return i; break;
            }
        }
        return -1;
}
void CreateMGraph(MGraph &G){
    /*创建一个以邻接矩阵为存储结构的无向图*/
    printf("请输入无向图的顶点数和边数（中间以空格隔开）：");
    scanf("%d %d", &G.vexnum, &G.arcnum);
    getchar();
    int i;
    for (i=0; i<G.vexnum; i++){
        printf("请输入第%d顶点的信息：", i+1);
        scanf("%c", &G.vexs[i]);
        getchar();
    }
    for (i=0; i<G.vexnum; i++){     //初始化邻接矩阵
        for (int j=0; j<G.vexnum; j++){
            G.arcs[i][j] = INT_MAX;
        }
    }
    char sour, dest; int weight;
    for (i=0; i<G.arcnum; i++){
        printf("请输入第%d条边的起顶点、尾顶点以及权值", i+1);
        scanf("%c %c %d", &sour, &dest, &weight);
        getchar();
        G.arcs[LocateVex(G, sour)][LocateVex(G, dest)] = weight;
        G.arcs[LocateVex(G, dest)][LocateVex(G, sour)] = weight;
    }
}

int Mini(int vexnum, Elem closedge[]){
    /*求出最小生成树中的下一个顶点*/
    int pos, miniweight;   //记录权值最小边的顶点位置及其权值
    miniweight = INT_MAX;
    for (int i=0; i<vexnum; i++){
        if (closedge[i].lowcost < miniweight && closedge[i].lowcost){
            miniweight = closedge[i].lowcost;
            pos = i;
        }
    }
    return pos;
}
void MiniSpanTree(MGraph &G){
    /*用Prim算法从顶点v开始构造无向图G的最小生成树*/
    Elem closedge[MAX_VERTEX_NUM];
    printf("你想用从哪个顶点开始生成最小生成树？请输入：");
    char v = getchar();
    int k = LocateVex(G, v );    //确定顶点k在图中的位置
    int i, j;
    for (i=0; i<G.vexnum; i++){
        if (i!=k) {
            closedge[i].adjvex = v;
            closedge[i].lowcost = G.arcs[i][k];
        }
    }
    closedge[k].lowcost = 0;    //意为顶点v已经加入到最小生成树中
    printf("\n最小生成树中的边有：\t");
    for (i=1; i<G.vexnum; i++){ //选择其余G.vexnum-1个顶点
        int k = Mini(G.vexnum, closedge);
        printf("V%c-V%c\t", closedge[k].adjvex, G.vexs[k]);   //输出生成树的边
        closedge[k].lowcost = 0;    //加入生成树顶点集
        for (j=0; j<G.vexnum; j++){
            if (G.arcs[k][j] < closedge[j].lowcost){    //新顶点加入最小生成树顶点集后选择最小边
                closedge[j].adjvex = G.vexs[k];
                closedge[j].lowcost = G.arcs[k][j];
            }
        }
    }
}
