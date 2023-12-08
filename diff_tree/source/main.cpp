#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "graphviz_dump.h"
#include "diff_tree.h"
#include "read_expr_to_tree.h"

#define LIST_DUMP(list)\
    List_Dump(list, __LINE__, __PRETTY_FUNCTION__)

#define STDOUT stderr


int main()
{
    FILE *str = fopen("NUL.txt", "w");
    fprintf(str, "fsd%c", '\0');
    fclose(str);

    fprintf(stdout, "вы хотите загрузить данные из базы?");
    bool ans_in = Analaze_Ans_on_quest("no", "yes");

    Tree *tree;

    if(ans_in or (tree==nullptr))
    {
        FILE *stream_read = open_file("database/new.txt", "r");//сделать динамическую генерацию имени
        tree = init_tree(20);
        len_arr *buff = read_from_file_to_buff(stream_read);

        HANDLER_ERROR(read_expr_to_tree(tree, (char *)buff->arr));
        HANDLER_ERROR(Graphiz_Dump_Tree(tree, "gr_dump/new_tree.dot"));

        int counter = 0;
        free_mem_buf(buff);
    }

    HANDLER_ERROR(Graphiz_Dump_Tree(tree, "gr_dump/begin_tree.dot"));

    int list_vars[5] = {(int)'x', 0, 0, 0, 0};
    double val_vars[5] = {3, 0, 0, 0, 0};

    LOG(1, stderr, "\n Eval_value = %lf \n", Eval_value(tree, list_vars, val_vars));

    Tree *diff_tree = Diff_Tree(tree, tree->head_node.prev);

    HANDLER_ERROR(Graphiz_Dump_Tree(diff_tree, "gr_dump/end_tree.dot"));

    LOG(1, stderr, "\n DIFF_value = %lf \n", Eval_value(diff_tree, list_vars, val_vars));

    HANDLER_ERROR(Destructor_Tree(tree));
}


/*
Tree *Join_Trees_in_Node(Tree *left_tree, Tree *right_tree, OPERATORS op)
{
    Tree *new_tree = init_tree(left_tree->copacity + right_tree->copacity + 10);

    long int delta_mem_left = Copy_Node_from_one_Tree_to_another(new_tree, left_tree);

    long int delta_mem_right = Copy_Node_from_one_Tree_to_another(new_tree, left_tree);

    Node *top_node = Create_Node(new_tree, op, (Node *)((long int)left_tree->beg_node + delta_mem_left), (Node *)((long int)right_tree->beg_node + delta_mem_right));

    HANDLER_ERROR(Set_new_High(new_tree, top_node));

    return new_tree;
}
*/

/*
long int Copy_Node_from_one_Tree_to_another(Tree *tree_to_cpy, Tree *tree_from_cpy)
{
    if(tree_to_cpy->copacity < tree_to_cpy->size + tree_from_cpy->size)
    {
        Increase_copacity_Tree(tree_to_cpy);
    }
    memcpy(tree_to_cpy->list_node + tree_to_cpy->size, tree_from_cpy->list_node, tree_from_cpy->size * sizeof(Node));

    long int delta_mem = (long int)(tree_to_cpy->list_node + tree_to_cpy->size) - (long int)tree_from_cpy->list_node;
    for(int i = tree_to_cpy->size; i < tree_from_cpy->size + tree_to_cpy->size; i++)
    {
        tree_to_cpy->list_node[i].left = (Node *)((long int)tree_to_cpy->list_node[i].left + delta_mem);
        tree_to_cpy->list_node[i].right = (Node *)((long int)tree_to_cpy->list_node[i].right + delta_mem);
        tree_to_cpy->list_node[i].prev = (Node *)((long int)tree_to_cpy->list_node[i].prev + delta_mem);
    }
    tree_to_cpy->size += tree_from_cpy->size;

    return delta_mem;
}
*/
