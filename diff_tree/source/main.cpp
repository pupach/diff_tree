#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "diff_tree.h"

#define LIST_DUMP(list)\
    List_Dump(list, __LINE__, __PRETTY_FUNCTION__)

#define STDOUT stderr


int main()
{
    fprintf(stdout, "вы хотите загрузить данные из базы?");
    bool ans_in = Analaze_Ans_on_quest("no", "yes");

    Tree *tree;

    if(ans_in or (tree==nullptr))
    {
        FILE *stream_read = open_file("database/new.txt", "r");//сделать динамическую генерацию имени
        tree = init_tree();
        len_arr *buff = read_from_file_to_buff(stream_read);

        int counter = 0;
        tree->beg_node = Read_from_file_Node_Diff_Tree((char *)buff->arr, tree, &counter);//переписать на CODE_ERRORS

        free_mem_buf(buff);
    }

    LOG(1, stderr, "\n Eval_value = %lf \n", Eval_value(tree));

    HANDLER_ERROR(Destructor_Tree(tree));
}
