//
// Created by zanca on 27/01/25.
//

#ifndef BTREE_H
#define BTREE_H
#include <stdint.h>
#include <stdlib.h>

typedef struct TreeNode_t {
  void *node;
  struct TreeNode_t *leftNode;
  struct TreeNode_t *rightNode;
} TreeNode_t;

typedef struct BinaryTree_t {
  struct TreeNode_t *head;
  uint64_t depth;
} BinaryTree_t;




#endif //BTREE_H
