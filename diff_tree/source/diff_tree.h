#ifndef DIFF_TREE
#define DIFF_TREE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../../my_lib/work_with_file.h"
#include "../../tree/source/tree.h"
#include "../../tree/source/akinator.h"


enum TYPE_DATA_TREE
{
    NUMBER = 0,
    TWO_ARG_FUNC = 1,
    ONE_ARG_FUNC = 2,
    UNDEF_TYPE = -1
};

enum OPERATORS
{
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4
};

double Eval_value(Tree *tree);

Node *Read_from_file_Node_Diff_Tree(char *buff, Tree *tree, int *counter);

#endif
