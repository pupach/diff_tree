#include "graphviz_dump.h"


CODE_ERRORS Graphiz_Dump_Tree(Tree *tree, char *filename)
{
    bool free_file_name = false;
    if(filename[0] == '_')
    {
        filename = Create_Filename_by_Time();
    }

    FILE *stream_write = fopen(filename, "w");   //обернуть в define
    if(stream_write == nullptr)     return PTR_NULL;

    HANDLER_ERROR(Write_Graphiz_Dump_to_File_Tree(stream_write, tree));

    fclose(stream_write);

    HANDLER_ERROR(Create_Png_And_Show(filename));

    if(free_file_name)
    {
        free(filename);
    }
}

CODE_ERRORS Recr_Write_Node_Dump(FILE *stream_write, Node *cur_node, int deep_recr)
{
    assert(cur_node != nullptr);
//        LOG(1, stderr, "Write_Graphiz_Dump_to_File_Tree i=%d, node = %p, data="data_spec" node_left5 = %p, node-right = %p\n", deep_recr ,cur_node, cur_node->data, cur_node->left, cur_node->right);

        const char *color = "#fff3e0";
        fprintf(stream_write,
                "node%p[label = \" data_type = %d | data = %lf | \", fillcolor = \"%s\", rank = %d];\n",
                cur_node, cur_node->data.type_data, cur_node->data.data,
                color, deep_recr);

        if(cur_node->left != nullptr)    Recr_Write_Node_Dump(stream_write, cur_node->left, deep_recr + 1);

        if(cur_node->right != nullptr)    Recr_Write_Node_Dump(stream_write, cur_node->right, deep_recr + 1);

        return ALL_GOOD;
}

CODE_ERRORS Write_Graphiz_Dump_to_File_Tree(FILE *stream_write, Tree *tree)
{

    fprintf(stream_write, "digraph\n{\n");

    fprintf(stream_write, "node [shape = Mrecord, color  = \"navy\", style = \"filled\"];\n");

    HANDLER_ERROR(Recr_Write_Node_Dump(stream_write, tree->head_node.prev, 0));

    /*
    fprintf(stream_write, "fillcolor = \"#33ff66\"];\n");

    fprintf(stream_write, "\nedge [color = \"orange\"];\n\n");
    Elem_list cur_elem= list->arr[0];

    fprintf(stream_write, "node%d -> node%d;\n", 0, cur_elem.val);

    fprintf(stream_write, "\nedge [color = \"cornFlowerBlue\"];\n\n");*/

    //LOG(1, stderr, "tree->size = %d\n", tree->size);

    HANDLER_ERROR(Recr_depend_tree_to_Dump(tree->head_node.prev, stream_write));

    fprintf(stream_write, "}\n");

    return ALL_GOOD;
}

CODE_ERRORS Recr_depend_tree_to_Dump(Node *cur_node, FILE *stream_out)
{
    if(cur_node->left != nullptr)
    {
        fprintf(stream_out, "node%p -> node%p;\n", cur_node, cur_node->left);
        HANDLER_ERROR(Recr_depend_tree_to_Dump(cur_node->left, stream_out));
    }
    if(cur_node->right != nullptr)
    {
        fprintf(stream_out, "node%p -> node%p;\n", cur_node, cur_node->right);
        HANDLER_ERROR(Recr_depend_tree_to_Dump(cur_node->right, stream_out));
    }
    return ALL_GOOD;
}
