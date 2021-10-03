#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#define COUNT 10

int vec_left[50001] = {0};

struct avltree {
    int key;
    char* value;
    int deleted;

    int height;
    struct avltree* left;
    struct avltree* right;
};

void avltree_free(struct avltree* tree);
struct avltree* avltree_lookup(struct avltree* tree, int key);
struct avltree* avltree_create(int key, char* value, float* total_cnt);
int avltree_height(struct avltree* tree);
int avltree_balance(struct avltree* tree);
struct avltree*
avltree_add(struct avltree* tree, int key, char* value, float* total_cnt);
struct avltree* avltree_right_rotate(struct avltree* tree);
struct avltree* avltree_left_rotate(struct avltree* tree);
struct avltree* avltree_leftright_rotate(struct avltree* tree);
struct avltree* avltree_rightleft_rotate(struct avltree* tree);
int imax2(int left_height, int right_height);
void Display(struct avltree* root, int ident, FILE* fout);
struct avltree* avltree_delete(
        struct avltree* tree, int key, float* deleted_cnt, float* total_cnt);
/* void print2DUtil(struct avltree* root, int space, FILE* fout);
void print2D(struct avltree* root, FILE* fout); */

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int getrand(int min, int max)
{
    return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

int main()
{
    srand(time(0));
    FILE* fout;
    float deleted_cnt = 0, total_cnt = 0;
    fout = fopen("out.txt", "w+");
    double time1 = 0, time2 = 0, res;
    int random;
    struct avltree *tree, *node;
    tree = avltree_create(1, "word", &total_cnt);
    for (int i = 2; i <= 10; i++) {
        // fprintf(fout, "%d ", avltree_add(tree, i, "AVL"));
        tree = avltree_add(tree, i, "AVL", &total_cnt);

        if (i % 1 == 0) {
            random = getrand(i - 5000, i);
            time1 = wtime();
            // ctime1 = clock();
            int x = time1 * 2;
            avltree_lookup(tree, (random));
            // ctime2 = clock();
            time2 = wtime();
            res = time2 - time1;
            /*  fprintf(fout,
                     "random = %d; n = %d; time = %.8lf\n",
                     random,
                     i,
                     res); */
        }
        time1 = 0;
        time2 = 0;
        // tree = avltree_add(tree, 2, "LOL");
        // tree = avltree_add(tree, 3, "HAH");
        // tree = avltree_add(tree, 4, "TOP");
    }

    //
    // printf("\nTotal time: %lf", time2 - time1);
    // print2D(tree, fout);
    printf("Total_cnt = %f\n", total_cnt);
    for (int k = 4; k < 10; k++) {
        tree = avltree_delete(tree, k, &deleted_cnt, &total_cnt);
    }
    /* tree = avltree_delete(tree, 3, &deleted_cnt, &total_cnt);
    tree = avltree_delete(tree, 4, &deleted_cnt, &total_cnt);
    tree = avltree_delete(tree, 5, &deleted_cnt, &total_cnt); */
    Display(tree, 0, fout);
    avltree_free(tree);
    fclose(fout);
    return 0;
}

struct avltree*
avltree_rebuild(struct avltree* tree, struct avltree* rb_tree, float* total_cnt)
{
    if (tree->deleted == 0) {
        *total_cnt = *total_cnt + 1;
        rb_tree = avltree_add(rb_tree, tree->key, tree->value, total_cnt);
    }
    if (tree->left != NULL)
        avltree_rebuild(tree->left, rb_tree, total_cnt);
    if (tree->right != NULL)
        avltree_rebuild(tree->right, rb_tree, total_cnt);
    return rb_tree;
}

struct avltree* avltree_delete(
        struct avltree* tree, int key, float* deleted_cnt, float* total_cnt)
{
    struct avltree* del_node = avltree_lookup(tree, key);
    if (del_node != NULL) {
        *deleted_cnt = *deleted_cnt + 1;
        del_node->deleted = 1;
        float k = *deleted_cnt / *total_cnt;
        if ((*deleted_cnt / (*total_cnt)) >= 0.5) {
            *total_cnt = 0;
            *deleted_cnt = 0;
            struct avltree* rb_tree = avltree_rebuild(tree, rb_tree, total_cnt);
            return rb_tree;
        } else
            return tree;
    }
}

struct avltree* avltree_create(int key, char* value, float* total_cnt)
{
    struct avltree* node;
    node = malloc(sizeof(*node));
    if (node != NULL) {
        *total_cnt = *total_cnt + 1;
        node->key = key;
        node->value = value;
        node->deleted = 0;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;
    }
    return node;
}

struct avltree*
avltree_add(struct avltree* tree, int key, char* value, float* total_cnt)
{
    if (tree == NULL) {
        return avltree_create(key, value, total_cnt);
    }
    if ((tree->deleted == 1) && (tree->key == key)) {
        return avltree_create(key, value, total_cnt);
    }
    if (key < tree->key) {
        /* Insert into left subtree */
        tree->left = avltree_add(tree->left, key, value, total_cnt);
        if (avltree_height(tree->left) - avltree_height(tree->right) == 2) {
            /* Subtree is unbalanced */
            if (key < tree->left->key) {
                /* Left left case */
                tree = avltree_right_rotate(tree);
            } else {
                /* Left right case */
                tree = avltree_leftright_rotate(tree);
            }
        }
    } else if (key > tree->key) {
        /* Insert into right subtree */
        tree->right = avltree_add(tree->right, key, value, total_cnt);
        if (avltree_height(tree->right) - avltree_height(tree->left) == 2) {
            /* Subtree is unbalanced */
            if (key > tree->right->key) {
                /* Right right case */
                tree = avltree_left_rotate(tree);
            } else {
                /* Right left case */
                tree = avltree_rightleft_rotate(tree);
            }
        }
    }
    tree->height
            = imax2(avltree_height(tree->left), avltree_height(tree->right))
            + 1;
    return tree;
}

int avltree_height(struct avltree* tree)
{
    return (tree != NULL) ? tree->height : -1;
}
int avltree_balance(struct avltree* tree)
{
    return avltree_height(tree->left) - avltree_height(tree->right);
}

struct avltree* avltree_right_rotate(struct avltree* tree)
{
    struct avltree* left;
    left = tree->left;
    tree->left = left->right;
    left->right = tree;
    tree->height
            = imax2(avltree_height(tree->left), avltree_height(tree->right))
            + 1;
    left->height = imax2(avltree_height(left->left), tree->height) + 1;
    return left;
}

struct avltree* avltree_left_rotate(struct avltree* tree)
{
    struct avltree* right;
    right = tree->right;
    tree->right = right->left;
    right->left = tree;
    tree->height
            = imax2(avltree_height(tree->left), avltree_height(tree->right))
            + 1;
    right->height = imax2(avltree_height(right->right), tree->height) + 1;
    return right;
}

struct avltree* avltree_leftright_rotate(struct avltree* tree)
{
    tree->left = avltree_left_rotate(tree->left);
    return avltree_right_rotate(tree);
}

struct avltree* avltree_rightleft_rotate(struct avltree* tree)
{
    tree->right = avltree_right_rotate(tree->right);
    return avltree_left_rotate(tree);
}

void avltree_free(struct avltree* tree)
{
    if (tree == NULL)
        return;
    avltree_free(tree->left);
    avltree_free(tree->right);
    free(tree);
}

int imax2(int left_height, int right_height)
{
    if (left_height > right_height)
        return left_height;
    else
        return right_height;
}

/* void print2DUtil(struct avltree* root, int space, FILE* fout)
{
    char spce = ' ';
    char nt = '\n';
    char ls = '/';
    char rs = '\\';
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space, fout);

    // Print current node after space
    // count
    fprintf(fout, "%c", nt);
    for (int i = COUNT; i < space; i++) {
        fprintf(fout, "%c", spce);
    }
    fprintf(fout, "%d\n", root->key);

    // Process left child
    print2DUtil(root->left, space, fout);
}

void print2D(struct avltree* root, FILE* fout)
{
    // Pass initial space count as 0
    print2DUtil(root, 0, fout);
} */

void Display(struct avltree* root, int ident, FILE* fout)
{
    if (ident > 0) {
        for (int i = 0; i < ident - 1; ++i) {
            fprintf(fout, vec_left[i] ? "│   " : "    ");
        }
        fprintf(fout, vec_left[ident - 1] ? "├── " : "└── ");
    }

    if (!root) {
        fprintf(fout, "(null)\n");
        return;
    }

    fprintf(fout, "%d\n", root->key);
    if (!root->left && !root->right) {
        return;
    }

    vec_left[ident] = 1;
    Display(root->left, ident + 1, fout);
    vec_left[ident] = 0;
    Display(root->right, ident + 1, fout);
}

struct avltree* avltree_lookup(struct avltree* tree, int key)
{
    while (tree != NULL) {
        if (key == tree->key) {
            return tree;
        } else if (key < tree->key) {
            tree = tree->left;
        } else {
            tree = tree->right;
        }
    }
    return tree;
}
