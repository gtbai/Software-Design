#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
typedef enum{Link, Thread} PointTag;
typedef struct Node{    //二叉树的线索链表表示法
    char data;
    Node* lchild, *rchild, *parent;     //指向左右孩子及父节点
    PointTag Ltag, Rtag;    //左右线索标志
}BiNode, *BiTree;
BiTree CreateBiTree(void);  //前序建立二叉树
void CopyBiTree(BiTree &bt, BiTree &bt2);   //复制二叉树
void ThreadingBiTree(BiTree &H, BiTree T, int option);   //线索化二叉树
void PreThreading(BiTree T);    //前序线索化二叉树
void PreOrderTraverse(BiTree T); //前序遍历前序线索化二叉树
void InOrderTraverse(BiTree H); //中序遍历前序线索化二叉树
void PostThreading(BiTree T);   //后序线索化二叉树
BiTree Parent(BiTree &H, BiTree &P); //寻找节点P的父节点
void PostOrderTraverse(BiTree T);   //后序遍历后序线索化二叉树

int main()
{
    printf("请输入根节点的值（‘#’代表空）：");
    BiTree bt = CreateBiTree(), bt2, head, head2;
    CopyBiTree(bt, bt2);
    ThreadingBiTree(head, bt, 1);
    printf("\n前序遍历前序线索化二叉树的结果是：\n"); PreOrderTraverse(head);
    printf("\n中序遍历前序线索化二叉树的结果是：\n"); InOrderTraverse(head);
    ThreadingBiTree(head2, bt2, 2);
    printf("\n后序遍历后序线索化二叉树的结果是：\n"); PostOrderTraverse(head2);
    printf("\n");
    system("pause");
    return 0;
}

BiTree CreateBiTree(void){
/*前序建立二叉树*/
    char ch;
    BiTree root;
    scanf(" %c", &ch);
    if (ch == '#') root = NULL;
    else{
        root = (BiTree) malloc (sizeof (BiNode));
        root->data = ch;
        printf("请输入%c的左孩子（‘#’代表空）：", ch);
        root->lchild = CreateBiTree(); root->Ltag = Link;
        if (root->lchild) root->lchild->parent = root;
        printf("请输入%c的右孩子（‘#’代表空）：", ch);
        root->rchild = CreateBiTree(); root->Rtag = Link;
        if (root->rchild) root->rchild->parent = root;
    }
    return root; //返回根节点
}
void CopyBiTree(BiTree &bt, BiTree &bt2){
/*复制二叉树*/
    if (bt){
        bt2 = (BiTree)malloc(sizeof(BiNode));
        bt2->data = bt->data;
        CopyBiTree(bt->lchild, bt2->lchild);    //递归复制左子树
        bt2->Ltag = Link;
        CopyBiTree(bt->rchild, bt2->rchild);    //递归复制右子树
        bt2->Rtag = Link;
    }
    else{
        bt2 = NULL;
    }
}
BiTree pre;
void ThreadingBiTree(BiTree &H, BiTree T, int option){
/*线索化二叉树*/
    H = (BiTree) malloc(sizeof(BiNode));    //为头节点申请空间
    H->Ltag = Link; H->Rtag = Thread;
    H->rchild = H;  //右指针回指
    if(!T)  H->lchild  = H;     //若二叉树空，则左指针回指
    else{
        pre = H;   //将H设置为pre指针
        H->lchild = T;  //H的左儿子应该是二叉树的根节点
        T->parent = H;  //根节点的父结点是头结点
        if (option == 1)PreThreading(T);    //前序线索化二叉树
        else if (option == 2) PostThreading(T);
        if (!pre->rchild) {
            pre->rchild = H;    //前序遍历的最后一个节点的右线索指向头节点
            pre->Rtag = Thread;
        }
        H->rchild = pre;
    }
}
void PreThreading(BiTree T){
/*前序线索化二叉树*/
    if (T){     //判断树是否为空
        if (!T->lchild){    //将现在节点的左子树指针域指向前导节点
            T->lchild = pre;
            T->Ltag = Thread;
        }
        if (!pre->rchild)   //将刚刚处理过的节点的右子树指针域指向后继节点
        {
            pre->rchild = T;
            pre->Rtag = Thread;
        }
        pre = T;
        if (T->Ltag == Link) PreThreading(T->lchild);   //递归地前序线索化当前节点的左子树
        if (T->Rtag == Link) PreThreading(T->rchild);   //递归地前序线索化当前节点的右子树
    }
}
void PreOrderTraverse(BiTree H){

/*前序遍历前序线索化二叉树*/
    BiTree root = H->lchild;
    while (root != H){  //判断是否遍历结束
        printf("%c ", root->data);
        if (root->Ltag == Link) root = root->lchild;    //可否改为if(root->lchild)???
        else root = root->rchild;
    }
}
void InOrderTraverse(BiTree H){
/*中序遍历前序线索化二叉树*/
    BiTree root = H->lchild;
    while (root->lchild && (root->Ltag == Link)) root = root->lchild;   //寻找中序遍历的第一个结点
    while (root != H){  //判断遍历是否结束
        printf("%c ", root->data);
        if (root->Rtag==Link){  //右子树，包括：左右都子树，以及左线索，右子树
            root = root->rchild;
            while (root->lchild && (root->Ltag==Link)) root = root->lchild;   //找右子树中序遍历的第一个结点
        }
        else if ((root->Ltag==Link) && (root->Rtag==Thread)){   //左子树，右线索
            if (root->rchild == H) root = H;
            else root = root->parent;
        }
        else if ((root->Ltag==Thread) && (root->Rtag==Thread)){ //左右都线索
            if (root==root->parent->lchild)    //是父节点的左孩子
                root = root->parent;
            else if (root == root->parent->rchild){ //是父节点的右孩子
                if (root->rchild == H) root = H;    //如果右线索指向头节点，那么循环结束
                else root = root->rchild->parent;   //如果右线索不指向头节点，那么指向右线索的父节点
            }
        }
    }

}
void PostThreading(BiTree T){
/*后序线索化二叉树*/
    if(T){
        PostThreading(T->lchild);   //递归地后序线索化当前节点的左子树
        PostThreading(T->rchild);   //递归地后序线索化当前节点的右子树
        if (!T->lchild){    //将现在节点的左子树指针域指向前导节点
            T->lchild = pre;
            T->Ltag = Thread;
        }
        if (!pre->rchild){  //将刚刚处理过的节点的右子树指针域指向后继节点
            pre->rchild = T;
            pre->Rtag = Thread;
        }
        pre = T;
    }
}
BiTree Parent(BiTree &H, BiTree &P){
/*寻找节点P的父节点*/
    BiTree tmp = P;
    if (H->lchild == P) return H;   //父节点是头节点
    else {
        tmp = H->lchild;  //tmp指向二叉树的根
        while(tmp->lchild != P && tmp->rchild != P){    //如果还未找到父节点
            if (tmp->Rtag == Link) tmp = tmp->rchild;   //如果当前节点有右子树，则往右走
            else tmp = tmp->lchild; //否则往左走，无论是子树还是线索
        }
        return tmp;
    }
}
void PostOrderTraverse(BiTree H){
/*后序遍历后序线索化二叉树*/
    BiTree tmp = H, par;
    while (1){      //找到后序遍历的第一个节点
        while (tmp->Ltag == Link) tmp = tmp->lchild;
        if (tmp->Rtag == Link) tmp = tmp->rchild;
        else break;
    }
    while (tmp != H){   //当还没有遍历整棵树时
        printf("%c ", tmp->data);
        par = Parent(H, tmp);
        if (par == H) tmp = H;  //如果头节点是父节点的话，那么当前节点是根节点，无后继
        else{
            if ((par ->rchild == tmp) || (par->Rtag == Thread) ) tmp = par; //如果tmp是父节点的右孩子或者独生左孩子
            else{   //否则tmp是其父节点的左孩子
                par = par->rchild;
                while (1){
                    while (par->Ltag == Link) par = par->lchild;
                    if (par->Rtag == Link) par = par->rchild;
                    else break;
                }
                tmp = par;
            }
        }
    }

}
