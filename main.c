#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define COUNT 10

int vec_left[50001] = {0};

struct avltree {
    int key;
    char* value;

    int height;
    struct avltree* left;
    struct avltree* right;
};

void avltree_free(struct avltree* tree);
struct avltree* avltree_lookup(struct avltree* tree, int key);
struct avltree* avltree_create(int key, char* value);
int avltree_height(struct avltree* tree);
int avltree_balance(struct avltree* tree);
struct avltree* avltree_add(struct avltree* tree, int key, char* value);
struct avltree* avltree_right_rotate(struct avltree* tree);
struct avltree* avltree_left_rotate(struct avltree* tree);
struct avltree* avltree_leftright_rotate(struct avltree* tree);
struct avltree* avltree_rightleft_rotate(struct avltree* tree);
int imax2(int left_height, int right_height);
void Display(struct avltree* root, int ident, FILE* fout);
/* void print2DUtil(struct avltree* root, int space, FILE* fout);
void print2D(struct avltree* root, FILE* fout); */

int main()
{
    FILE* fout;
    fout = fopen("out.txt", "w+");
    double time1, time2;
    struct avltree* tree;
    tree = avltree_create(0, "word");
    for (int i = 1; i < 100; i++) {
        // fprintf(fout, "%d ", avltree_add(tree, i, "AVL"));
        tree = avltree_add(tree, i, "AVL");
        // tree = avltree_add(tree, 2, "LOL");
        // tree = avltree_add(tree, 3, "HAH");
        // tree = avltree_add(tree, 4, "TOP");
    }
    //
    // print2D(tree, fout);
    Display(tree, 0, fout);
    avltree_free(tree);
    fclose(fout);
    return 0;
}

struct avltree* avltree_create(int key, char* value)
{
    struct avltree* node;
    node = malloc(sizeof(*node));
    if (node != NULL) {
        node->key = key;
        node->value = value;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;
    }
    return node;
}

struct avltree* avltree_add(struct avltree* tree, int key, char* value)
{
    if (tree == NULL) {
        return avltree_create(key, value);
    }
    if (key < tree->key) {
        /* Insert into left subtree */
        tree->left = avltree_add(tree->left, key, value);
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
        tree->right = avltree_add(tree->right, key, value);
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

    fprintf(fout, "%d\n", root->height);
    if (!root->left && !root->right) {
        return;
    }

    vec_left[ident] = 1;
    Display(root->left, ident + 1, fout);
    vec_left[ident] = 0;
    Display(root->right, ident + 1, fout);
}