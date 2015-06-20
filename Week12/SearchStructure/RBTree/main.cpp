#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

enum Color{ //颜色枚举型
    RED, BLACK
};

typedef struct RBNode{
    Color color; //结点颜色
    int key;    //结点所存数据
    struct RBNode* lchild;    //指向左孩子的指针
    struct RBNode* rchild;    //指向右孩子的指针
    struct RBNode* parent;    //指向父节点的指针
}RBNode, *RBTree;

RBTree root = NULL; //红黑树的根节点

void L_Rotate(RBTree &root, RBTree &x){
    /*左旋操作*/
    RBTree y = x->rchild;
    x->rchild = y->lchild;
    if (y->lchild) y->lchild->parent = x;
    y->parent = x->parent;  //设置y的父节点
    if (!x->parent) root = y;   //如果旋转前x是根节点
    else{
        if (x == x->parent->lchild) x->parent->lchild = y;
        else x->parent->rchild = y;
    }
    y->lchild = x;  //将x作为y的左孩子
    x->parent = y;  //设置x的父节点
}
void R_Rotate(RBTree &root, RBTree &y){
    /*右旋操作*/
    RBTree x = y->lchild;
    y->lchild = x->rchild;
    if (x->rchild) x->rchild->parent = y;
    x->parent = y->parent;  //设置x的父节点
    if (!y->parent)  root = x;//如果旋转前y是根节点
    else {
        if (y == y->parent->lchild) y->parent->lchild = x;
        else y->parent->rchild = x;
    }
    x->rchild = y;  //设置y为x的右孩子
    y->parent = x;  //设置y的父节点
}
void InsertFix(RBTree &root, RBTree &node){
    /*向红黑树中插入一个节点后进行修复，使其重新满足红黑树的特性*/
    RBTree parent, gparent; //当前节点的父节点和祖父节点
    while ((parent=node->parent) && parent->color==RED){    //父节点为红色
        gparent = parent->parent;   //祖父节点
        if (parent == gparent->lchild){ //如果父亲节点是祖父节点的左儿子
            RBTree uncle = gparent->rchild; //叔叔结点
            if (uncle && uncle->color == RED){   //Case1: 倘若叔叔结点也是红色
                parent->color = BLACK;  //将父节点设为黑色
                uncle->color = BLACK;   //将叔叔结点设为黑色
                gparent->color = RED;   //将祖父节点设为红色
                node = gparent;         //将当前节点设置为祖父节点
                continue;
            }
            if (node == parent->rchild){    //Case2: 叔叔节点是黑色，当前节点是父节点的右孩子
                RBTree tmp = parent;
                L_Rotate(root, parent); //以父节点为支点左旋
                parent = node;
                node = tmp;             //以父节点作为当前节点
            }
            //叔叔结点是黑色，且当前节点为父节点的左孩子
            parent->color = BLACK;  //将父节点设为黑色
            gparent->color = RED;   //将祖父节点设为红色
            R_Rotate(root, gparent);    //以祖父节点为支点右旋
        }
        else{   //如果父亲节点是祖父节点的右儿子
            RBTree uncle = gparent->lchild; //叔叔结点
            if (uncle && uncle->color == RED){  //Case1: 倘若叔叔结点也是红色
                parent->color = BLACK;
                uncle->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue;
            }
            if (node == parent->lchild){
                RBTree tmp = parent;
                R_Rotate(root, parent);
                parent = tmp;
                node = tmp;
            }
            parent->color = BLACK;
            gparent->color = RED;
            L_Rotate(root, gparent);
        }
    }
    root->color = BLACK;    //将根节点设为黑色
}
bool InsertNode(RBTree &root, RBTree &node){
    /*向红黑树中插入一个节点*/
    RBTree y = NULL, x = root;
    while (x){  //直到x为空
        y = x;
        if (node->key == x->key) return false;
        else if (node->key < x->key) x = x->lchild;
        else x = x->rchild;
    }
    node->parent = y;   //设置node的父节点
    if (!y) root = node; //如果本是空树
    else if (node->key < y->key) y->lchild = node;
    else y->rchild = node;
    node->color = RED;  //将插入的结点设为红色
    InsertFix(root, node);
    return true;
}
RBTree SearchNode(RBTree &root, int key){
    //在红黑树中查找关键字为key的结点
    if (!root) return NULL; //若树为空
    if (key == root->key) return root;
    if (key < root->key) return (SearchNode(root->lchild, key));    //递归查找左子树
    else return (SearchNode(root->rchild, key));    //递归地查找右子树
}
void DeleteFix(RBTree &root, RBTree  &node, RBTree &parent){
    /*用于删除节点后修复红黑树使其继续满足5条特性*/
    while ((!node || node->color == BLACK) && node!=root){
        //当当前节点为黑色，且不是根节点时
        if (node == parent->lchild){    //如果当前节点是左孩子
            RBTree brother = parent->rchild;
            if (brother->color == RED){ //case 1:兄弟节点是红色
                brother->color = BLACK; //兄弟节点染黑
                parent->color = RED;    //父节点染红
                L_Rotate(root, parent); //以父节点为支点左旋
                brother = parent->rchild;   //更新兄弟节点
            }
            if ((!brother->lchild || brother->lchild->color==BLACK) &&
                (!brother->rchild || brother->rchild->color==BLACK))
            {//case 2:兄弟节点是黑色，且兄弟节点双子均黑
                brother->color = RED;   //兄弟节点染红
                node = parent;  //当前节点设为父节点
                parent = parent->parent;    //更新父节点
            }
            else{
                if (!brother->rchild || brother->rchild->color==BLACK){
                    //case 3:兄弟节点是黑色，并且其左孩子红，右孩子黑
                    brother->lchild->color = BLACK; //兄弟节点左孩子染黑
                    brother->color = RED;   //兄弟节点染红
                    R_Rotate(root, brother);    //以兄弟节点为支点右旋
                    brother = parent->rchild;   //更新兄弟节点
                }
                //case 4: 兄弟结点黑，且其右孩子红
                brother->color = parent->color; //将父节点颜色赋给兄弟节点
                parent->color = BLACK;  //父节点染黑
                brother->rchild->color = BLACK; //兄弟节点右孩子染黑
                L_Rotate(root, parent); //以父节点为支点左旋
                node = root;    //将当前节点设为根节点
                break;  //跳出循环??????
            }
        }
        else {  //当前节点是右孩子
            RBTree brother = parent->lchild; //兄弟结点
            if (brother->color == RED){
                //case 1:兄弟结点为红色
                brother->color = BLACK;
                parent->color = RED;
                R_Rotate(root, parent);
                brother = parent->lchild;
            }
            if ((!brother->lchild || brother->lchild->color==BLACK) &&
                (!brother->rchild || brother->rchild->color==BLACK)){
                    brother->color = RED;
                    node = parent;
                    parent = parent->parent;
                }
            else{
                if (!brother->lchild || brother->lchild->color==BLACK){
                    brother->rchild->color = BLACK;
                    brother->color = RED;
                    L_Rotate(root, brother);
                    brother = parent->lchild;
                }
                brother->color = parent->color;
                parent->color = BLACK;
                brother->lchild->color = BLACK;
                R_Rotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (!node) node->color = BLACK;
}

void DeleteNode(RBTree &root, RBTree &node){
    /*从红黑树中删除node结点*/
    RBTree child, parent;
    Color color;
    if ((node->lchild) && (node->rchild)){  //左右孩子都不为空
        RBTree rp = node;
        rp = rp->rchild;    //获取取代节点
        while (rp->lchild) rp = rp->lchild;
        if (!node->parent) //如果node是根节点
            root = rp;
        else{
            if (node == node->parent->lchild)
                node->parent->lchild = rp;
            else node->parent->rchild = rp;
        }
        child = rp->rchild; //取代节点的右孩子
        parent = rp->parent; //取代节点的父节点
        color = rp->color;  //取代节点的颜色
        if (parent == node){
            parent = rp;
        }
        else{
            if (child){//右子树不为空
                child->parent = parent;
            }
            parent->lchild = child;
            rp->rchild = node->rchild;
            rp->rchild->parent = rp;
        }
        rp->parent = node->parent;
        rp->color = node->color;
        rp->lchild = node->lchild;
        rp->lchild->parent = rp;
        if (color == BLACK) DeleteFix(root, child, parent);
        free(node);
        return;
    }
    if (node->lchild){  //如果node的左子树不为空
        child = node->lchild;
    }
    else
        child = node->rchild;
    parent = node->parent;
    color = node->color;    //保存从树中删除的颜色
    if (child) child->parent = parent;
    if (!parent){   //node结点是根节点
        root = child;
    }else{
        if (node == parent->lchild) parent->lchild = child;
        else parent->rchild = child;
    }
    if (color == BLACK) DeleteFix(root, child, parent);
    free(node);
}
void DisplayTree(RBTree &T, int space_num){
    //横版打印平衡树
    if (T){
        if (T->rchild) DisplayTree(T->rchild, space_num+1);
        for (int i=0; i<2*space_num; i++) printf(" ");
        if (T->color == RED) printf("(R)");
        else printf("(B)");
        printf("%d\n", T->key);
        if (T->lchild) DisplayTree(T->lchild, space_num+1);
    }
}

int main()
{
    char op;
    int key;
    bool flag = true;
    RBTree node;
    while (flag){
        printf("===============红黑树===============\n\
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
            node = (RBTree) malloc (sizeof(RBNode));
            node->key = key;
            node->lchild = node->rchild = NULL;
            if (!InsertNode(root, node)) printf("插入失败！");
            else printf("插入成功！");
            printf("\n插入后的红黑树如下所示：\n");
            DisplayTree(root, 1);
            break;
        case '2':
            printf("请输入欲查找的关键字：");
            scanf("%d", &key);
            getchar();
            if (!SearchNode(root, key)) printf("没找到！");
            else printf("找到了！");
            printf("\n");
            break;
        case '3':
            printf("请输入欲删除的关键字：");
            scanf("%d", &key);
            getchar();
            node = SearchNode(root, key);
            if (!node) printf("删除失败！\n");
            else{
                printf("删除成功！\n");
                DeleteNode(root, node);
                DisplayTree(root, 1);
            }
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
