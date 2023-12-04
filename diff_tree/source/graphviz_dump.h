#ifndef GRAPHIZ_DUMP
#define GRAPHIZ_DUMP

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "../../Stack/source/stack.h"
#include "../../my_lib/work_with_file.h"
#include "../../tree/source/tree.h"

CODE_ERRORS Write_Graphiz_Dump_to_File_Tree(FILE *stream_write, Tree *tree);

CODE_ERRORS Recr_depend_tree_to_Dump(Node *cur_node, FILE *stream_out);

CODE_ERRORS Graphiz_Dump_Tree(Tree *tree, char *filename="_");

CODE_ERRORS Recr_Write_Node_Dump(FILE *stream_write, Node *cur_node, int deep_recr);

#endif
