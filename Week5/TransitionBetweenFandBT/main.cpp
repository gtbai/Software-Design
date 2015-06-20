/*森林与二叉树之间的转换*/
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NODES_NUM 100
using namespace std;

/*二叉树的二叉链表存储结构*/
typedef struct BiTNode{
    char data;      //数据域
    BiTNode* lchild, *rchild;   //指向左右孩子
}BiTNode, *BiTree;

/*树的孩子链表的存储表示*/
typedef struct CTNode{  //孩子节点
    int index;
    CTNode* next;   //指向下一个孩子节点
}* ChildPtr;
typedef struct{
    char data;
    ChildPtr firstchild;    //孩子链表的头指针
}CTBox;
typedef struct{
    CTBox nodes[MAX_NODES_NUM];
    int n, r;               //结点数和根的位置
}CTree;

BiTree CreateBiTree(void);  //建立二叉树
void HorizontalBiTree(BiTree root, int space); //横向打印二叉树, space控制每行前空格数
vector<CTree> CreateCTree(void);   //建立树
void PrintTree(CTree ctree);     //以广义表形式打印树
void DFSPrint(CTree ctree, int i);  //深搜打印树
vector<CTree> ConvertBTtoF(BiTree bt);    //将二叉树转化为森林
void RecConvertBTtoF(BiTree bt, CTree* CTptr, int index);  //将二叉树转化为森林的递归函数
BiTree ConvertFtoBT(vector<CTree> forest);      //将森林转化为二叉树
BiTree RecConvertCTtoBT(CTree ctree, int index);     //将树转化为二叉树的递归函数

int main()
{
    char op;
    printf("==========森林与二叉树的转换=========\n1.二叉树转换成森林\n2.森林转换成二叉树\n0.退出\n输入序号以执行相应操作：");
    scanf(" %c", &op);
    while (op!='0'){
         switch(op){
            case '1':{
                printf("请输入根节点的数据（‘#’代表空）：");
                 BiTree bitree = CreateBiTree();
                printf("您输入的二叉树为：\n");
                HorizontalBiTree(bitree, 1);
                vector<CTree> forest = ConvertBTtoF(bitree);
                printf("==========二叉树转化成森林的结果如下：==========\n");
                for (int i=0; i<forest.size(); i++) {
                    printf("第%d个树的广义表形式为：", i+1);
                    PrintTree(forest[i]);
                }
                break;
            }
            case '2':{
                vector<CTree> cforest = CreateCTree();
                for (int i=0; i<cforest.size(); i++) {
                    printf("第%d个树的广义表形式为：", i+1);
                    PrintTree(cforest[i]);
                }
                BiTree result = ConvertFtoBT(cforest);
                printf("==========森林转化成二叉树的结果如下：==========\n");
                HorizontalBiTree(result, 1);
                break;
                }
    }
    printf("==========森林与二叉树的转换=========\n1.二叉树转换成森林\n2.森林转换成二叉树\n0.退出\n输入序号以执行相应操作：");
    scanf(" %c", &op);
}
    return 0;
}

BiTree CreateBiTree(void){
/*建立二叉树*/
    char ch;
    BiTree root;
    scanf(" %c", &ch);
    if (ch == '#') root = NULL;
    else{
        root = (BiTree) malloc (sizeof (BiTNode));
        root->data = ch;
        printf("请输入%c的左孩子（‘#’代表空）：", ch);
        root->lchild = CreateBiTree();
        printf("请输入%c的右孩子（‘#’代表空）：", ch);
        root->rchild = CreateBiTree();
    }
    return root; //返回根节点
}
void HorizontalBiTree(BiTree root, int space) {
/*横向打印二叉树*/
    if (root){
        HorizontalBiTree(root->rchild, space+1);   //递归横向打印右子树
        for (int i=0; i<2*space; i++) printf(" ");
        printf("%c\n", root->data);                     //打印节点信息
        HorizontalBiTree(root->lchild, space+1);  //递归横向打印左子树
    }
}
vector<CTree> CreateCTree(void){
/*建立树*/
    char prefix[5] = "tree", filename[10] = "";
    vector<CTree> forest;
    for (int i=1; ; i++){
        sprintf(filename, "%s%d", prefix, i);
        ifstream in(filename);
        if (!in) break;
        CTree ctree;
        string inputStr;
        char* charIndex;
        int count = 0;
        ChildPtr tmpPtr;
        getline(in, inputStr);
        while (strcmp(inputStr.c_str(), "")){
            char* inputStr2 = new char[inputStr.length() + 1];
            strcpy(inputStr2, inputStr.c_str());
            char* firstnode = strtok(inputStr2, " ");
            ctree.nodes[count].data = firstnode[0];
            if (!(charIndex = strtok(NULL, " "))) ctree.nodes[count++].firstchild = NULL;
            else (tmpPtr = ctree.nodes[count++].firstchild = (ChildPtr)malloc(sizeof(CTNode)))->index = atoi(charIndex);
            while ((charIndex = strtok(NULL, " "))) {
                ChildPtr child  = (ChildPtr) malloc(sizeof(CTNode));
                child->index = atoi(charIndex);
                tmpPtr->next = child;
                tmpPtr = child;
            }
            tmpPtr->next = NULL;
            getline(in, inputStr);
        }
        ctree.n = count;
        ctree.r = 0;
        forest.push_back(ctree);
    }
    return forest;
}
int visit[MAX_NODES_NUM];
void PrintTree(CTree ctree){
/*以广义表形式打印树*/
    for (int i=0; i<ctree.n; i++) visit[i] = 0;
    for (int i=0; i<ctree.n; i++){  //如果当前还有未访问的节点，则继续深搜打印
        if (!visit[i])  DFSPrint(ctree, i);
    }
    printf("\n");
}
void DFSPrint(CTree ctree, int i){
/*深搜打印树*/
    bool flag = true;
    printf("%c", ctree.nodes[i].data);
    if (ctree.nodes[i].firstchild) printf("(");
    else flag = false;
    visit[i] = 1;
    ChildPtr p = ctree.nodes[i].firstchild;  //令p指向节点i的孩子链表的头节点
    while (p){
        if (!visit[p->index]) DFSPrint(ctree, p->index);
        if (p->next) printf(",");
        p = p->next;
    }
    if (flag) printf(")");
}
vector<CTree> ConvertBTtoF(BiTree bt){
    /*将二叉树转化为森林*/
    vector<CTree> CTvec;
    BiTree tmp = bt;
    while(tmp){
        CTree ctree;
        ctree.nodes[0].data = tmp->data;
        ctree.nodes[0].firstchild = NULL;
        ctree.r = 0; ctree.n = 1;
        RecConvertBTtoF(tmp->lchild, &ctree, 0);
        CTvec.push_back(ctree);
        tmp = tmp->rchild;
    }
    return CTvec;
}
void RecConvertBTtoF(BiTree bt, CTree* CTptr, int index){
    /*将二叉树转化为森林的递归函数*/
    if (bt){
        BiTree tmp = bt;
        while (tmp){
            CTptr->nodes[CTptr->n].data = tmp->data;
            CTptr->nodes[CTptr->n].firstchild = NULL;
            if(!CTptr->nodes[index].firstchild){
                ChildPtr firstptr = (ChildPtr)malloc(sizeof(CTNode));
                firstptr->index = CTptr->n;
                firstptr->next = NULL;
                CTptr->nodes[index].firstchild = firstptr;
            }
            else{
                ChildPtr child = CTptr->nodes[index].firstchild;
                while(child->next) child = child->next;
                ChildPtr ptr = (ChildPtr)malloc(sizeof(CTNode));
                ptr->index = CTptr->n;
                ptr->next = NULL;
                child->next = ptr;
            }
            CTptr->n++;
            RecConvertBTtoF(tmp->lchild, CTptr, CTptr->n-1);
            tmp = tmp->rchild;
        }
    }
}
BiTree ConvertFtoBT(vector<CTree> forest){
/*将森林转化为二叉树*/
    BiTree root, tmp;
    for (int i=0; i<forest.size(); i++){
        BiTree sibling = (BiTree)malloc(sizeof(BiTNode));
        sibling->data = forest[i].nodes[forest[i].r].data;
        sibling->lchild = RecConvertCTtoBT(forest[i], forest[i].r);
        if (!i) root = tmp = sibling;
        else{
            tmp->rchild = sibling;
            tmp = sibling;
        }
    }
    tmp->rchild = NULL;
    return root;
}
BiTree RecConvertCTtoBT(CTree ctree, int index){
    /*将树转化为二叉树的递归函数*/
    if (ctree.nodes[index].firstchild){
        BiTree child = (BiTree)malloc(sizeof(BiTNode));
        ChildPtr tmp = ctree.nodes[index].firstchild;       //tmp指针指向在树的孩子链表中当前处理到的节点
        child->data = ctree.nodes[tmp->index].data;
        child->lchild = RecConvertCTtoBT(ctree, tmp->index);
        tmp = tmp->next;
        BiTree remem = child;   //remem指向刚刚创建的二叉树节点
        for ( ; tmp; tmp = tmp->next, remem = remem->rchild){
            BiTree sibling = (BiTree)malloc(sizeof(BiTNode));
            sibling->data = ctree.nodes[tmp->index].data;
            sibling->lchild = RecConvertCTtoBT(ctree, tmp->index);
            remem->rchild = sibling;
        }
        remem->rchild = NULL;
        return child;
    }
    return NULL;
}
