#ifndef READ_EXPR_TO_TREE
#define READ_EXPR_TO_TREE

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
#include "graphviz_dump.h"
#include "diff_tree.h"
#include "read_expr_to_tree.h"

CODE_ERRORS read_expr_to_tree(Tree *tree, char *buff);

int read_last_2_op_priorety(Tree *tree, char *beg_cur_buff, Node **top_node);

int read_last_op_priorety(Tree *tree, char *beg_cur_buff, Node **top_node);

int read_last_1_op_priorety(Tree *tree, char *beg_cur_buff, Node **top_node);

int read_brackets_in_expr(Tree *tree, char *beg_cur_buff, Node **top_node);

#endif
