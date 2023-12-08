#ifndef DIFF_TREE
#define DIFF_TREE

#include <stdlib.h>
#include <math.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../my_lib/work_with_file.h"
#include "../../tree/source/tree.h"
#include "../../tree/source/akinator.h"


enum OPERATORS
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    DEGREE = 5

};

static const int MAX_AMOUNT_VAR = 5;

const char POISON_NAME_VAR = '0';

const int POISON_INT_NAME_VAR = 0;

Node *Recr_Copy_Node(Tree *tree_to_cpy, Tree *tree_from_cpy, Node *save_top_node=nullptr);

Tree *Join_Trees_in_Node(Tree *left_tree, Tree *right_tree, OPERATORS op, Node *left_top_node=nullptr, Node *right_top_node=nullptr);

Tree *Diff_Tree(Tree *tree, Node *cur_top_node);

Tree *DIFF_OP_DATA_ADD(Tree *tree, Node *save_top_node);

Tree *DIFF_OP_DATA_SUB(Tree *tree, Node *save_top_node);

Tree *DIFF_OP_DATA_MUL(Tree *tree, Node *save_top_node);

Tree *DIFF_OP_DATA_DIV(Tree *tree, Node *save_top_node);

Tree *DIFF_OP_DATA_DEGREE(Tree *tree, Node *save_top_node);

CODE_ERRORS Write_inf_about_node_to_File_Diff_Tree(FILE *stream_write, Node *node);

Tree *Tree_Copy(Tree *tree_from_cpy, Tree *tree_where_cpy, Node *top_source=nullptr); //TODO:сделать из этого метода и init подобие конструктора

int find_val_var(int int_code, int list_vars[]);

double Eval_value(Tree *tree, int list_vars[MAX_AMOUNT_VAR], double vars_value[MAX_AMOUNT_VAR], Node *save_top_node=nullptr);
//TODO: сделать нормальные значения по умолчанию

Node *Read_from_file_Node_Diff_Tree(char *buff, Tree *tree, int *counter);

#endif
