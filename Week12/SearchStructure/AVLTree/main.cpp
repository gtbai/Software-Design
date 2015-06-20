#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

//结点（平衡树）的存储结构
typedef struct Node{
    int data;   //结点数据域
    int bf;     //平衡因子
    Node* lchild, *rchild;  //左孩子和右孩子
}AVLNode, *AVLTree;

void R_Rotate(AVLTree &p){
    //对以*p为根节点的树做右旋处理
    AVLTree lc = p->lchild;
    p->lchild = lc->rchild;
    lc->rchild = p; p = lc;
}
void L_Rotate(AVLTree &p){
    //对以*p为根节点的树做左旋处理
    AVLTree rc = p->rchild;
    p->rchild = rc->lchild;
    rc->lchild = p; p = rc;
}
void LeftBalance(AVLTree &T){
    //对以*T为根节点的树做左平衡处理
    AVLTree lc = T->lchild; //根节点的左孩子
    switch(lc->bf){
    case 1: //左子树高，单向右旋
        R_Rotate(T);
        T->bf = T->rchild->bf = 0;  //更新平衡因子
        break;
    case -1: //右子树高，先左旋再右旋
        AVLTree rd = lc->rchild;    //根节点的左孩子的右孩子
        L_Rotate(T->lchild);   //对左子树左旋
        R_Rotate(T);    //对整个子树右旋
        T->bf = 0;      //更新平衡因子
        switch(rd->bf){
        case 1: //插入到了左子树的右子树的左子树上
            T->lchild->bf = 0;
            T->rchild->bf = -1;
            break;
        case -1:    //插入到了左子树的右子树的右子树上
            T->rchild->bf = 0;
            T->lchild->bf = 1;
            break;
        //case 0?
        }
        break;
    }
}
void RightBalance(AVLTree &T){
    //对以*T为根节点的树做右平衡处理
    AVLTree rc = T->rchild; //根节点的右孩子
    switch(rc->bf){
    case -1:    //新结点插入到了右孩子的右子树上
        L_Rotate(T);    //单向左旋
        T->bf = T->lchild->bf = 0;  //更新平衡因子
        break;
    case 1:     //新结点插入到了右孩子的左子树上，先右旋再左旋
        AVLTree ld = rc->lchild;    //根节点的右孩子的左孩子
        R_Rotate(T->rchild);   //先对根结点的右子树右旋
        L_Rotate(T);    //再对整个树左旋
        T->bf = 0;      //更新平衡因子
        switch(ld->bf){
        case 1: //插入到了根节点的右孩子的左孩子的左子树上
            T->lchild->bf = 0;
            T->rchild->bf = -1;
            break;
        case -1:
            T->lchild->bf = 1;
            T->rchild->bf = 0;
            break;
        }
        break;
    }
}
bool InsertNode(AVLTree &T, int e, bool &taller){
    /*向以*T为根的平衡二叉树中插入一个与e有相同关键字的结点
    插入成功返回true，否则返回false，taller反映平衡树长高与否*/
    if (!T){    //*T是空树
        T = (AVLTree) malloc (sizeof(AVLNode));
        T->data = e;
        T->bf = 0;
        T->lchild = T->rchild = NULL;
        taller = true;
    }else{
        if (T->data == e){
            taller = false;
            return false;
        }
        if (e<T->data){ //那么结点应插入到*T的左子树中
            if (!InsertNode(T->lchild, e, taller)) return false;    //插入不成功
            if (taller){    //左子树长高了
                switch (T->bf){ //检查T的平衡因子
                case 1:
                    LeftBalance(T); taller = false; break;
                case 0:
                    T->bf = 1; taller = true;   break;
                case -1:
                    T->bf = 0;  taller = false; break;
                }
            }
        }
        else {  //那么结点应插入到*T的右子树中
            if (!InsertNode(T->rchild, e, taller)) return false;    //插入不成功
            if (taller){    //右子树长高了
                switch(T->bf){
                case 1:
                    T->bf = 0; taller = false; break;
                case 0:
                    T->bf = -1; taller = true; break;
                case -1:
                    RightBalance(T); taller = false; break;
                }
            }
        }
    }
    return true;
}
bool DeleteNode(AVLTree &T, int e, bool &lower){
    /*从平衡树中删除一个关键字为e的结点*/
    bool return_flag;    //标记删除操作是否成功
    int lfort; //标记是删除左子树上的结点还是右子树上的结点
    if (!T) return false;   //空树，删除失败
    if (T->data == e){
        //找到了需要删除的结点
        if (T->rchild == NULL){ //如果该节点无右孩子
            T = T->lchild;
            lower = true;
            return true;
        }else{  //该节点有右孩子
            AVLTree rc = T->rchild; //找到右子树中关键字最小结点
            while (rc->lchild) rc = rc->lchild;
            T->data = rc->data;
            return_flag = DeleteNode(T->rchild, rc->data, lower);
            lfort = -1; //标记删除的为右子树上的结点
        }
    }else if(e<T->data){
        return_flag = DeleteNode(T->lchild, e, lower);  //在左子树上删除该结点
        lfort = 1;
    }else{
        return_flag = DeleteNode(T->rchild, e, lower);  //在右子树上删除该结点
        lfort = -1;
    }
    if (lower){ //如果子树变矮了
        switch(lfort){
        case 1: //左子树变矮了
            switch(T->bf){
            case 1:
                T->bf = 0; lower = true;   break;
            case 0:
                T->bf = -1; lower = false; break;
            case -1:
                RightBalance(T); lower = true; break;
            }
            break;
        case -1:    //右子树变矮了
            switch(T->bf){
            case 1:
                LeftBalance(T); lower = true; break;
            case 0:
                T->bf = 1; lower = false; break;
            case -1:
                T->bf = 0; lower = true; break;
            }
            break;
        }
    }
    return return_flag;
}
AVLTree SearchNode(AVLTree &T, int key){
    //在平衡树中查找关键字为key的结点
    if (!T) return NULL;    //树为空的情况
    if (T->data == key) return T;
    else if (key < T->data) return (SearchNode(T->lchild, key));
    else return (SearchNode(T->rchild, key));
}
void DisplayTree(AVLTree &T, int space_num){
    //横版打印平衡树
    if (T){
        if (T->rchild) DisplayTree(T->rchild, space_num+1);
        for (int i=0; i<2*space_num; i++) printf(" ");
        printf("%d\n", T->data);
        if (T->lchild) DisplayTree(T->lchild, space_num+1);
    }
}
int main()
{
    char op;
    AVLTree T = NULL;
    int key;
    bool taller, lower, flag = true;
    while (flag){
        printf("==========平衡树（AVL树）==========\n\
1. 插入\n\
2. 查找\n\
3. 删除\n\
0. 退出\n\
====================================\n\
输入序号以执行相应操作：");
        op = getchar();
        switch(op){
        case '1':
            printf("请输入欲插入的关键字：");
            scanf("%d", &key);
            getchar();
            taller = false;
            if (!InsertNode(T, key, taller)) printf("插入失败！");
            else printf("插入成功！");
            printf("\n插入后的平衡树如下所示：\n");
            DisplayTree(T, 1);
            break;
        case '2':
            printf("请输入欲查找的关键字：");
            scanf("%d", &key);
            getchar();
            if (!SearchNode(T, key)) printf("没找到！");
            else printf("找到了！");
            printf("\n");
            break;
        case '3':
            printf("请输入欲删除的关键字：");
            scanf("%d", &key);
            getchar();
            lower = false;
            if (!DeleteNode(T, key, lower)) printf("删除失败！");
            else printf("删除成功！");
            printf("\n删除后的平衡树如下所示：\n");
            DisplayTree(T, 1);
            break;
        case '0':
            flag = false;
            printf("已退出！\n");
            break;
        }
    }
    system("pause");
    return 0;
}
