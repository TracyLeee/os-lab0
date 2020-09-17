/**
 * @file week01.c
 * @brief Exercises in C for the first week
 *
 * A few exercises, marked by `TODO`s.
 * 1. Write a string comparison function, with the same semantics as the
 *    standard `strcmp`.
 * 2. Debug the linked-list implementation.
 * 3. Implement binary tree traversals in three orders: in-order, 
 *    preorder, postorder.
 * 4. Count the occurence of letters in a file.
 *
 * @author Atri Bhattacharyya, Adrien Ghosn
 */
#include <stdlib.h>
#include "week01.h"

int w1_strcmp(const char *s1, const char *s2) {
  // Edge cases: s1 and/or s2 == NULL
  if (!s1 && !s2) return 0;
  else if (!s1) return -1;
  else if (!s2) return 1;

  while (*s1 == *s2 && *s1 && *s2) {
    ++s1;
    ++s2;
  }

  if (!*s1 && !*s2) return 0;  // *s1 == '\0' && *s2 == '\0'
  else if (*s1 < *s2) return -1;
  else return 1;
}

/* TODO: Debug the linked list implementation. The API consists of five 
 * functions:
 * - w1_create_node
 * - w1_delete_node
 * - w1_insert_node
 * - w1_remove_node 
 * - w1_size_list
 * Hint: There are two bugs.
 */
w1_node *w1_create_node(int value) {
  w1_node *node = (w1_node *)calloc(1, sizeof(w1_node));

  if(node == NULL)
    return NULL;
  
  node->element = value;
  node->next = NULL;
  return node;
}

void w1_delete_node(w1_node *node) {
  free(node);
}

w1_node *w1_insert_node(w1_node *head, w1_node *node, unsigned pos) {

  if(node == NULL)
    return NULL;

  /* Inserting into an empty list */
  if(head == NULL){
    /* Only acceptable position is 0 */
    if(pos == 0)
      return node;
    /* All other positions are illegal */
    else
      return NULL;
  }

  /* Non-empty list */
  if(pos == 0) {
    node->next = head;
    return node;
  }
  /* Parse through list till position pos */
  w1_node *prev = head;
  for(unsigned i = 1; i < pos; i++){
    prev = prev->next;
    /* This means end of list encountered before pos */
    if(prev == NULL)
      return NULL;
  }
  node->next = prev->next;
  prev->next = node;
  return head;
}

w1_node *w1_remove_node(w1_node *head, w1_node *node) {
  /* Silent failure for empty list */
  if(head == NULL || node == NULL)
    return NULL;

  /* Removing first node changes head */
  if(node == head) 
    head = head->next;
  else {
    w1_node *prev = head;
    while((prev != NULL) && (prev->next != node))
      prev = prev->next;
    /* Silent failure if end of list reached before finding node */
    if(prev == NULL)
      return NULL;
    prev->next = node->next;
  }
  node->next = NULL;

  return head;
}

unsigned w1_size_list(w1_node *head) {
  unsigned size = 0;

  while(head != NULL) {
    size++;
    head = head->next;
  }
  return size;
}

char *gen_post_order_str(Node *node, char *tmp_str) {
  if (!node) return tmp_str;
  if (!tmp_str) return NULL;

  char *tmp_res = gen_post_order_str(node->right, gen_post_order_str(node->left, tmp_str));
  char *res = malloc(sizeof(char)*(strlen(tmp_res) + 3));
  if (!tmp_res && !res) return NULL;
  sprintf(res, "%s%d ", tmp_res, node->data);

  free(tmp_res);

  return res;
}

void w1_print_post_order(Node* node, FILE* fd) {
  char *init_str = malloc(sizeof(char));
  if (!init_str) return;
  *init_str = 0;

  char *post_order_str = gen_post_order_str(node, init_str);
  if (!post_order_str) return;
  fputs(post_order_str, fd);

  free(post_order_str);
}

char *gen_pre_order_str(Node *node, char *tmp_str) {
  if (!node) return tmp_str;
  if (!tmp_str) return NULL;

  char *tmp_res = malloc(sizeof(char)*(strlen(tmp_str) + 3));
  if (!tmp_res) return NULL;
  sprintf(tmp_res, "%s%d ", tmp_str, node->data);

  free(tmp_str);

  return gen_pre_order_str(node->right, gen_pre_order_str(node->left, tmp_res));
}

void w1_print_pre_order(Node* node, FILE* fd) {
  char *init_str = malloc(sizeof(char));
  if (!init_str) return;
  *init_str = 0;

  char *pre_order_str = gen_pre_order_str(node, init_str);
  if (!pre_order_str) return;
  fputs(pre_order_str, fd);

  free(pre_order_str);
}

char *gen_in_order_str(Node *node, char *tmp_str) {
  if (!node) return tmp_str;
  if (!tmp_str) return NULL;

  char *tmp_res = gen_in_order_str(node->left, tmp_str);
  char *res = malloc(sizeof(char)*(strlen(tmp_res) + 3));
  if (!tmp_res && !res) return NULL;
  sprintf(res, "%s%d ", tmp_res, node->data);

  free(tmp_res);

  return gen_in_order_str(node->right, res);
}

void w1_print_in_order(Node* node, FILE* fd) {
  char *init_str = malloc(sizeof(char));
  if (!init_str) return;
  *init_str = 0;

  char *in_order_str = gen_in_order_str(node, init_str);
  if (!in_order_str) return;
  fputs(in_order_str, fd);

  free(in_order_str);
}

w1_count_result_t w1_count_letter_freq(char* file) {
  FILE *fd = fopen(file, "r");
  if (!fd) return NULL;

  w1_count_result_t freq_arr = (w1_count_result_t)malloc(sizeof(double)*FREQ_LEN);
  memset(freq_arr, 0, sizeof(double)*FREQ_LEN);
  int tmp_char;

  while (tmp_char = fgetc(fd) != EOF) {
    if ('A' <= tmp_char && tmp_char <= 'Z') tmp_char += 'a' - 'A';

    if ('a' <= tmp_char && tmp_char <= 'z') ++freq_arr[tmp_char - 'a'];
  }

  fclose(fd);

  return freq_arr;
}


