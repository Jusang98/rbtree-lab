#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}
// 왼쪽으로 회전
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
// left_lotate 의 반대
void right_rotate(rbtree *t, node_t *x)
{
  // x의 왼쪽 자식 y 선언
  node_t *y = x->left;

  // y의 오른쪽 자식을 x의 왼쪽 자식으로
  x->left = y->right;

  // y의 오른쪽 자식이 nil이 아니면
  if (y->right != t->nil)
    // y의 오른쪽 자식의 부모를 x로
    y->right->parent = x;

  // y의 부모를 x의 부모로
  y->parent = x->parent;

  // x의 부모가 nil이면 루트 노드를 y로
  if (x->parent == t->nil)
    t->root = y;
  // x가 x의 부모의 왼쪽 자식이면 x의 부모의 왼쪽 자식으로 y로
  else if (x == x->parent->left)
    x->parent->left = y;
  // x가 x의 부모의 오른쪽 자식이면 x의 부모의 오른쪽 자식을 y로
  else
    x->parent->right = y;

  // y의 오른쪽 자식을 x로
  y->right = x;
  // x의 부모를 y로
  x->parent = y;
}
// 후위 순위
void destroyTree(node_t *n, node_t *nil_node)
{
  if (n == nil_node || n == NULL)
    return;

  // 왼쪽 하위 트리 소멸
  destroyTree(n->left, nil_node);

  // 오른쪽 하위 트리 소멸
  destroyTree(n->right, nil_node);

  // 루트 노드 소멸
  if (n != NULL)
    free(n); // s
}

void delete_rbtree(rbtree *t)
{
  node_t *x = t->nil;
  destroyTree(x, t->nil);
  free(t->nil); // s
  free(t);
}

node_t *rb_insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else
    {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;

  return 0;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  // TODO: implement insert
  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil)
  {
    y = x;
    if (key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }

  z->parent = y;
  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rb_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *z = t->root;
  while (z != t->nil)
  {
    if (z->key == key)
    {
      return z;
    }
    else if (key < z->key)
    {
      z = z->left;
    }
    else
    {
      z = z->right;
    }
  }
  return NULL;
}
node_t *erasemin(node_t *n, node_t *nil)
{
  node_t *min = n;
  while (n != nil)
  {
    min = n;
    n = n->left;
  }
  return min;
}
node_t *rbtree_min(const rbtree *t)
{
  node_t *min;
  min = erasemin(t->root, t->nil);
  return min;
}

node_t *rbtree_max(const rbtree *t)
{
  node_t *x = t->root;
  // node_t *max = t->nil;
  while (x->right != t->nil)
  {
    // max = x;
    x = x->right;
  }
  return x;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}
void rb_delete_fixup(rbtree *t, node_t *x)
{

  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z)
{
  // TODO: implement erase
  node_t *y = z; // 삭제할 노드 z를 y에 할당
  color_t y_color = y->color;
  node_t *x;

  if (z->left == t->nil)
  {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  else if (z->right == t->nil)
  {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  else
  {
    // y = rbtree_min(t);
    y = erasemin(z->right, t->nil);
    y_color = y->color;
    x = y->right;
    if (y->parent == z)
    {
      x->parent = y;
    }
    else
    {
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  free(z);
  z = t->nil;
  if (y_color == RBTREE_BLACK)
  {
    rb_delete_fixup(t, x);
  }
  t->nil->parent = NULL;
  t->nil->right = NULL;
  t->nil->left = NULL;
  t->nil->color = RBTREE_BLACK;

  return 0;
}

void inorder_getarr(node_t *root, node_t *nil, key_t *arr, int *index)
{
  if (root == nil)
    return;
  inorder_getarr(root->left, nil, arr, index);
  arr[(*index)++] = root->key;
  inorder_getarr(root->right, nil, arr, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  int *index = calloc(1, sizeof(int));
  inorder_getarr(t->root, t->nil, arr, index);
  free(index);
  return 0;
}
