#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX_VERTEX_NUM  20
using namespace std;

//图的邻接表存储结构
typedef struct ArcCell{
    int adjvex; //边的邻接顶点在图中的位置
    int weight; //边的权值
    struct ArcCell* next;  //指向下一条边
}ArcCell;
typedef struct{
    char data;  //存放顶点的信息
    ArcCell* firstarc;  //依附于此顶点的第一条边
}Node;
typedef struct{
    int vexnum, arcnum; //存放图的顶点数目和边的数目
    Node vexs[MAX_VERTEX_NUM+1];   //存放顶点的数组
}Gragh;

typedef struct { //辅助数组定义
        int startnum;   //起点编号
        int adjvex;    //权值最小边的另一邻接顶点
        int lowcost;    //权值最小边的权值
}Elem;

void CreateGraph(Gragh &G); //创建无向图
int LocateVex(Gragh &G, char data); //在图中确定数据为data的顶点的位置
void MiniSpanTree_Prim(Gragh &G); //用Prim算法构造图G的最小生成树
void Swap(int i, int j); //交换closedge数组中下标为i和j的两个元素
void PushDown(int first, int last); //整理堆，将closedge[first]下推到堆\
中适当位置
void MakeHeap(int num);    //将Elem类型的数组的前n个元素整理成堆

int main()
{
    Gragh G;    //定义一个无向图
    CreateGraph(G); //创建无向图
    MiniSpanTree_Prim(G);   //生成最小生成树
    return 0;
}

int LocateVex(Gragh &G, char data){
    /*在图中确定数据为data的顶点的位置*/
    for (int i=1; i<=G.vexnum; i++){
        if (G.vexs[i].data == data){
            return i; break;
        }
    }
    return -1;  //若没找到该顶点的话返回-1
}
void CreateGraph(Gragh &G){
    /*创建无向图*/
    printf("请输入图的顶点数和边数（中间以空格分隔）：");
    scanf("%d %d", &G.vexnum, &G.arcnum);
    getchar();
    int i;
    for (i=1; i<=G.vexnum; i++){
        printf("请输入第%d个顶点的信息", i);
        scanf("%c", &G.vexs[i].data);
        getchar();
        G.vexs[i].firstarc = NULL;
    }
    for (i=0; i<G.arcnum; i++){
        char start_data, end_data;  //用于暂时存放用户输入的边的信息
        int start, end, weight;
        printf("请输入第%d条边的起始顶点、终止顶点和权值", i+1);
        scanf("%c %c %d", &start_data, &end_data, &weight);
        getchar();
        start = LocateVex(G, start_data); end = LocateVex(G, end_data);
        ArcCell* arcptr = (ArcCell*) malloc (sizeof(ArcCell));
        arcptr->adjvex = end;
        arcptr->weight = weight;
        arcptr->next = G.vexs[start].firstarc;  //使这条边成为依附于start的第一条边
        G.vexs[start].firstarc = arcptr;
        ArcCell* arcptr2 = (ArcCell*) malloc (sizeof(ArcCell));
        arcptr2->adjvex = start;
        arcptr2->weight = weight;
        arcptr2->next = G.vexs[end].firstarc;
        G.vexs[end].firstarc = arcptr2;
    }
}
int pos[MAX_VERTEX_NUM+1];  //记录编号为i的顶点在堆中pos[i]的位置
Elem closedge[MAX_VERTEX_NUM+1];    //辅助数组

void MiniSpanTree_Prim(Gragh &G){
    /*用Prim算法构造图G的最小生成树*/
    printf("你想用从哪个顶点开始生成最小生成树？请输入：");
    char v = getchar();
    int k = LocateVex(G, v);    //确定顶点k在图中的位置
    int i;
    pos[k] = k;
    closedge[k].startnum = k;
    closedge[k].lowcost = 0;    //表明顶点k已经加入到最小生成树中了
    for (i=1; i<=G.vexnum; i++){
        if (i!=k){
            pos[i] = i;
            closedge[i].startnum = i;
            closedge[i].adjvex = k;
            closedge[i].lowcost = INT_MAX;  //先将最近距离初始化为无穷
        }
    }
    for (ArcCell* ptr=G.vexs[k].firstarc; ptr; ptr=ptr->next){  //考察依附与顶点k的所有边
        closedge[ptr->adjvex].lowcost = ptr->weight;    //将与k邻接顶点的最近距离更改
    }
    int record = G.vexnum;  //记录存储边的堆中元素个数
    MakeHeap(record); //将所有边整理成堆
    while (!closedge[1].lowcost){  //取出已经加入到最小生成树的顶点
        Swap(1, record);
        record--;
        PushDown(1, record);
    }
    printf("加入到最小生成树中的边有：\t");
    for (i=1; i<=G.vexnum-1; i++){  //向最小生成树中加入|V|-1个顶点
        printf("V%c-V%c\t",G.vexs[closedge[1].adjvex].data, \
               G.vexs[closedge[1].startnum].data);
        closedge[1].lowcost = 0;
        for (ArcCell* ptr = G.vexs[closedge[1].startnum].firstarc; ptr; \
        ptr=ptr->next ){    //考虑刚加入结点所有邻接边
            if (ptr->weight < closedge[pos[ptr->adjvex]].lowcost){  //更新权值最小边
                closedge[pos[ptr->adjvex]].lowcost = ptr->weight;
                closedge[pos[ptr->adjvex]].adjvex =\
                closedge[1].startnum;
            }
        }
        Swap(1, record); //将刚刚加入最小生成树的顶点移出堆
        record--;   //堆中元素个数减一
        PushDown(1, record);    //整理堆
    }
}
void Swap(int i, int j){
    /*交换closedge数组中下标为i和j的两个元素*/
    Elem tmp = closedge[i]; //交换堆中两个元素
    closedge[i] = closedge[j];
    closedge[j] = tmp;
    pos[closedge[i].startnum] = i;
    pos[closedge[j].startnum] = j;
}
void PushDown(int first, int last){
    /*整理堆，将closedge[first]下推到堆中适当位置*/
    int r;
    r = first;
    while (r <= last/2){    //此时可能还需要再整理
        if (2*r == last){
            if (closedge[r].lowcost > closedge[last].lowcost){
                Swap(r, last);
            }
            r = last;
        }else if (closedge[2*r].lowcost <= closedge[2*r+1].lowcost && \
                  closedge[r].lowcost > closedge[2*r].lowcost){
                    Swap(r, 2*r); //交换r和r的左儿子
                    r = 2*r;

        }else if (closedge[2*r].lowcost > closedge[2*r+1].lowcost && \
                 closedge[r].lowcost > closedge[2*r+1].lowcost)
                {
                    Swap(r, 2*r+1); //交换r和r的右儿子
                    r = 2*r + 1;
        }else{
            r = last;   //符合堆的定义，循环结束
        }
    }
}
void MakeHeap(int num){
    /*将Elem类型的数组的前n个元素整理成堆*/
    for (int i=num/2; i>=1; i--){
        PushDown(i, num);
    }
}
