#include <iostream>
#include <algorithm>
#include <stdio.h>
#define MAX_VERTEX_NUM 20
#define MAX_EDGE_NUM 190
using namespace std;

/*使用森林表示并查集，其中树形结构采用双亲表示法；
并查集上的优化有：1.在并查集中求两个集合并集时，总是将元素少的集合并入元素多的集合中
              2.在并查集中查找某个节点所在的集合时“压缩路径”*/
typedef struct{
    int parent; //存储父结点的位置
    bool parentag;  //标志parent域是存放父节点位置还是子集所含成员的个数
    char data;  //此结点的数据域
}Cell;
typedef struct{
    int vexnum;                 //存放并查集中的初始集合（结点）数
    Cell vexs[MAX_VERTEX_NUM];  //存放并查集中的初始集合（结点）序列
}MFset;

typedef struct{
    char start, end; //边的起始结点和终止结点
    int weight;     //边的权值
}Edge;

int CreateGraph(MFset &MST, Edge edges[]); //创建图
bool cmpare(Edge e1, Edge e2);  //比较两条边的权值大小
int find_mfset(MFset &MST, int i);  //在并查集中查找元素i所在的子集
void merge_mfset(MFset &MST, int i, int j); //在并查集中将元素i和元素j所在的子集合并
void MiniSpanTree_Kruskal(MFset &MST, Edge edges[], int arcnum);    //使用Kruskal\
算法求最小生成树
int LocateVex(MFset &MST, char data); //求结点在图中的位置

int main()
{
    MFset MST;
    Edge edges[MAX_EDGE_NUM];
    int arcnum = CreateGraph(MST, edges);    //创建图
    MiniSpanTree_Kruskal(MST, edges, arcnum);//使用Kruskal算法求最小生成树
    system("pause");
    return 0;
}

int CreateGraph(MFset &MST, Edge edges[]){
    /*创建图*/
    int arcnum; //用于记录边数
    printf("==========最小生成树Kruskal算法==========\n");
    printf("请输入顶点个数以及边的个数(中间以空格隔开)：");
    scanf("%d %d", &MST.vexnum, &arcnum);
    getchar();
    for (int i=0; i<MST.vexnum; i++){
        printf("请输入第%d个顶点的信息：", i+1);
        scanf("%c", &MST.vexs[i].data);
        getchar();
        MST.vexs[i].parent = 1;
        MST.vexs[i].parentag = false;
    }
    for (int i=0; i<arcnum; i++){
        printf("请输入第%d条边的起始结点、终止结点以及权值：", i+1);
        scanf("%c %c %d", &edges[i].start, &edges[i].end, &edges[i].weight);
        getchar();
    }
    return arcnum;  //返回边数
}
bool cmpare(Edge e1, Edge e2){
    /*比较两条边的权值大小*/
    return e1.weight < e2.weight;
}
int find_mfset(MFset &MST, int i){
    /*在并查集中查找元素i所在的子集*/
    int j, k;
    for (j=i; MST.vexs[j].parentag; j=MST.vexs[j].parent);
    for (k=i; k!=j; k=MST.vexs[k].parent) MST.vexs[k].parent = j;   //压缩路径
    return j;
}
void merge_mfset(MFset &MST, int i, int j){
    /*在并查集中将元素i和元素j所在的子集合并*/
    if (MST.vexs[i].parent < MST.vexs[j].parent){
        MST.vexs[i].parent += MST.vexs[j].parent;
        MST.vexs[j].parent = i; MST.vexs[j].parentag = true;
    }else{
        MST.vexs[j].parent += MST.vexs[i].parent;
        MST.vexs[i].parent = j; MST.vexs[i].parentag = true;
    }
}
void MiniSpanTree_Kruskal(MFset &MST, Edge edges[], int arcnum){
    /*使用Kruskal算法求最小生成树*/
    int rest = MST.vexnum, cost = 0;   //记录并查集中剩余集合数
    sort(edges, edges+arcnum, cmpare);  //对存放边的数组按照权值升序排序
    printf("最小生成树中的边有：\t");
    for (int i=0; i<arcnum&&rest>1; i++){   ///其实只需要扫描一次就够了
        int start_root = find_mfset(MST, LocateVex(MST, edges[i].start)), \
            end_root = find_mfset(MST, LocateVex(MST, edges[i].end));
        if (start_root == end_root) continue;   //如果权值最小边的起点和终点在同一连通分支
        else{
            merge_mfset(MST, start_root, end_root); //将起点和终点所在的子集合并
            printf("V%c-V%c\t", edges[i].start, edges[i].end);    //输出此条边
            cost += edges[i].weight;    //将该条边的权值加入到最小生成树的权值中
            rest--; //并查集中剩余集合数减一
        }
    }
    printf("\n最小生成树的权值是：%d\n", cost);
}
int LocateVex(MFset &MST, char data){
    /*求结点在图中的位置*/
    for (int i=0; i<MST.vexnum; i++){
        if (MST.vexs[i].data == data){
            return i; break;
        }
    }
    return -1;  //如果没找到对应节点，则返回-1
}
