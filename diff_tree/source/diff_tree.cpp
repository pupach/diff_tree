#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "diff_tree.h"

double Eval_value(Tree *tree)
{
    Node *save_top_node = tree->beg_node;
    double res = NAN;

    switch(tree->beg_node->type_data)
    {
        case TWO_ARG_FUNC:
        {
            tree->beg_node = save_top_node->left;
            double val_left = Eval_value(tree);

            tree->beg_node = save_top_node->right;
            double val_right = Eval_value(tree);

            switch((int)save_top_node->data)
            {
                case ADD:
                    res = val_left + val_right;
                    break;

                case SUB:
                    res = val_left - val_right;
                    break;

                case MUL:
                    res = val_left * val_right;
                    break;

                case DIV:
                    res = val_left / val_right;
                    break;

                default:
                    LOG(1, stderr, "undef_command\n");
            };
            break;
        }
        case ONE_ARG_FUNC:
        {
            if(save_top_node->left == nullptr) LOG(1, stderr, "ERRRRRRRRROOOOR");

            tree->beg_node = save_top_node->left;
            double val_left = Eval_value(tree);

            double val_right = NAN;
            if(save_top_node->right != nullptr)
            {
                tree->beg_node = save_top_node->right;
                val_right = Eval_value(tree);
            }

            switch((int)save_top_node->data)
            {
                default:
                    LOG(1, stderr, "undef_command\n");
            }
            break;
        }

        case NUMBER:
            LOG(1, stderr, "res = %lf    ", save_top_node->data);
            res = save_top_node->data;
            break;

        default:
            LOG(1, stderr, "undef_command\n");
    };
    tree->beg_node = save_top_node;

    LOG(1, stderr, "res in eval type_data=%d, data=%lf, res=%lf save_top_node->data = %lf\n", save_top_node->type_data, save_top_node->data, res, save_top_node->data);
    return res;
}


Node *Read_from_file_Node_Diff_Tree(char *buff, Tree *tree, int *counter)
{
    if(buff[*counter] != '(')
    {
        if(strcmp(buff + *counter, "nill") == 0)
        {
            *counter += strlen(buff + *counter) + 1;
            return nullptr;
        }
        else    exit(1);
    }
    (*counter)++;

    char *data = (char *)calloc(sizeof(char), MAX_SIZE_DATA_STR);

    LOG(1, stderr, "R_F_N counter=%d, data=%s\n", *counter, buff + *counter);

    strcpy(data, buff + *counter);
    *counter += strlen(buff + *counter) + 1;

    Node *left  = Read_from_file_Node_Diff_Tree(buff, tree, counter);
    Node *right = Read_from_file_Node_Diff_Tree(buff, tree, counter);

    Node *cur_node = nullptr;
    switch(data[0])
    {
        case '+':
            cur_node = Create_Node(tree, ADD, left, right);
            cur_node->type_data = TWO_ARG_FUNC;
            break;

        case '-':
            cur_node = Create_Node(tree, SUB, left, right);
            cur_node->type_data = TWO_ARG_FUNC;
            break;

        case '*':
            cur_node = Create_Node(tree, MUL, left, right);
            cur_node->type_data = TWO_ARG_FUNC;
            break;

        case '/':
            cur_node = Create_Node(tree, DIV, left, right);
            cur_node->type_data = TWO_ARG_FUNC;
            break;

        default:
            cur_node = Create_Node(tree, atoi(data), left, right);
            cur_node->type_data = NUMBER;
    };

    assert(cur_node != nullptr);

    if(buff[*counter] != ')')   LOG(1, stderr, "ERRRRROOOOOORRRRRRR!!!!!!!!!!!!!!");
    (*counter)++;

    free(data);
    return cur_node;
}


CODE_ERRORS Write_inf_about_node_to_File_Diff_Tree(FILE *stream_write, Node *node)
{
    switch(node->type_data)
    {
        case NUMBER:
            fprintf(stream_write, "(%d%c", node->data, '\0');
            break;

        case TWO_ARG_FUNC:
        {
            switch((int)node->data)
            {
                case ADD:
                    fprintf(stream_write, "(+%c", node->data, '\0');
                    break;

                case SUB:
                    fprintf(stream_write, "(-%c", node->data, '\0');
                    break;

                case MUL:
                    fprintf(stream_write, "(*%c", node->data, '\0');
                    break;

                case DIV:
                    fprintf(stream_write, "(/%c", node->data, '\0');
                    break;

                default:
                    LOG(1, stderr, "undef_command\n");
            };
            break;
        }

        default:
            LOG(1, stderr, "undef_command\n");

    };

    if(node->left != nullptr)
    {
        HANDLER_ERROR(Write_inf_about_node_to_File_Diff_Tree(stream_write, node->left));
    }
    else
    {
        fprintf(stream_write, "nill%c", '\0');
    }

    if(node->right != nullptr)
    {
        HANDLER_ERROR(Write_inf_about_node_to_File_Diff_Tree(stream_write, node->right));
    }
    else
    {
        fprintf(stream_write, "nill%c", '\0');
    }
    fprintf(stream_write, ")");
}
