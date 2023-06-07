#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
    int sum;
    int flag;
} Node;


void
push(int v, int l, int r, Node *tree)
{
    tree[v].sum += tree[v].flag * (r - l);
    if (l + 1 != r) {
        tree[2 * v].flag += tree[v].flag;
        tree[2 * v + 1].flag += tree[v].flag;
    }
    tree[v].flag = 0;
}


void
relax(int v, Node *tree)
{
    tree[v].sum = tree[2 * v].sum + tree[2 * v + 1].sum;
}


void
change(int v, int l, int r, int x, int tl, int tr, Node *tree)
{
    push(v, tl, tr, tree);
    if (tr <= l || r <= tl) {
        return;
    }
    push(v, tl, tr, tree);
    if (l <= tl && tr <= r) {
        tree[v].flag += x;
        push(v, tl, tr, tree);
        return;
    }
    int m = (tl + tr) / 2;
    change(2 * v, l, r, x, tl, m, tree);
    change(2 * v + 1, l, r, x, m, tr, tree);
    relax(v, tree);
}


int
get_sum(int v, int l, int r, int tl, int tr, Node *tree)
{
    push(v, tl, tr, tree);
    if (r <= tl || l >= tr) {
        return 0;
    } else if (l <= tl && r >= tr) {
        return tree[v].sum;
    } else {
        int m = (tl + tr) / 2;
        return get_sum(2 * v, l, r, tl, m, tree) + get_sum(2 * v + 1, l, r, m, tr, tree);
    }
}


int
main(void )
{
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        exit(1);
    }
    if (!(1 <= n && n <= 10000) || !(1 <= m && m  <= 50000)) {
        exit(1);
    }

    
    Node *tree = calloc(sizeof(*tree), 4 * n);
    if (!tree) {
        exit(1);
    }
    
    
    for (int i = 0; i < m; i++) {
        int type, l, r, add;

        if (scanf("%d %d %d", &type, &l, &r) != 3) {
            exit(1);
        }
        if (!(1 <= type && type <= 2) || !(0 <= l && l <= n) || !(0 <= r && r <= n)) {
            exit(1);
        }

        if (type == 1) {
            if (scanf("%d", &add) != 1) {
                exit(1);
            }
            change(1, l, r, add, 0, n, tree);
        } else {
            printf("%d\n", get_sum(1, l, r, 0, n, tree));
        }
    }

    free(tree);
    return 0;
}

