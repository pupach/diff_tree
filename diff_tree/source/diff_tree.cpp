#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "diff_tree.h"
#include "graphviz_dump.h"
1
double Eval_value(Tree *tree, int list_vars[MAX_AMOUNT_VAR], double vars_value[MAX_AMOUNT_VAR], Node *save_top_node)
{
    if(save_top_node == nullptr) save_top_node = tree->head_node.prev;

    double res = NAN;

    switch(save_top_node->type_data)
    {
        case TWO_ARG_FUNC:
        {
            double val_left  = Eval_value(tree, list_vars, vars_value, save_top_node->left);

            double val_right = Eval_value(tree, list_vars, vars_value, save_top_node->right);

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

                case DEGREE:
                {
                    LOG(1, stderr, "beg_degree\n");

                    if(save_top_node->left == nullptr){ LOG(1, stderr, "ERRRRRRRRROOOOR"); exit(1);}

                    double val_left = Eval_value(tree, list_vars, vars_value, save_top_node->left);

                    double val_right = 0;
                    if(save_top_node->right != nullptr)
                    {
                        val_right = Eval_value(tree, list_vars, vars_value, save_top_node->right);
                    }
                    res = pow(val_left, val_right);
                    break;
                }

                default:
                    LOG(1, stderr, "undef_command_TWO_ARG_FUNC data = %d\n");
            };
            break;
        }

        case VARIABLE:
        {
            int index = find_val_var(save_top_node->data, list_vars);
            if(res < 0) LOG(1, stderr, "undef_index\n");
            else
            {
                res = vars_value[index];
            }
            LOG(1, stderr, "VARIABLE VAL = %lf  index = %d  ", res, index);
            break;
        }
        case NUMBER:
            LOG(1, stderr, "res = %lf    ", save_top_node->data);
            res = save_top_node->data;
            break;

        default:
            LOG(1, stderr, "undef_command_AT_ALL\n");
    };

    LOG(1, stderr, "res in eval type_data=%d, data=%lf, res=%lf save_top_node->data = %lf\n", save_top_node->type_data, save_top_node->data, res, save_top_node->data);
    return res;
}

int find_val_var(int int_code, int list_vars[])
{
    for(int i = 0; i < MAX_AMOUNT_VAR; i++)
    {
        if(int_code == list_vars[i]) return i;
    }
    return -1;
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
        case '^':
            cur_node = Create_Node(tree, DEGREE, left, right);
            cur_node->type_data = TWO_ARG_FUNC;
            break;

        default:
            if(isalpha(data[0]))
            {
                cur_node = Create_Node(tree, (int)data[0], left, right);
                cur_node->type_data = VARIABLE;
            }
            else
            {
                cur_node = Create_Node(tree, atoi(data), left, right);
                cur_node->type_data = NUMBER;
            }
    };//TODO: внести type_data в CREATE_NODE

    LOG(1, stderr, "R_F_N counter=%d, data=%lf  type_data=%d\n ", *counter, cur_node->data, cur_node->type_data);

    assert(cur_node != nullptr);

    if(buff[*counter] != ')')   LOG(1, stderr, "ERRRRROOOOOORRRRRRR!!!!!!!!!!!!!!");
    (*counter)++;

    if(buff[*counter] != ')')
    {
        HANDLER_ERROR(Set_new_High(tree, cur_node));
    }

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

                case DEGREE:
                    fprintf(stream_write, "(^%c", node->data, '\0');
                    break;

                default:
                    LOG(1, stderr, "undef_command\n");
            };
            break;
        }

        case VARIABLE:
            fprintf(stream_write, "(%c%c", (char)node->data, '\0');
            break;

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

Tree *Diff_Tree(Tree *tree, Node *cur_top_node)
{
    Node *save_top_node;
    if(cur_top_node == nullptr)    save_top_node = tree->head_node.prev;
    else                save_top_node = cur_top_node;
    Tree *left_tree         = nullptr;
    Tree *right_tree        = nullptr;
    Tree *right_right_tree  = nullptr;
    Tree *left_right_tree   = nullptr;
    Tree *right_left_tree   = nullptr;
    Tree *left_left_tree         = nullptr;
    Tree *res_tree          = nullptr;

    switch((int)save_top_node->type_data)
    {
        case TWO_ARG_FUNC:
        {
            switch((int)save_top_node->data)
            {
                case ADD:
                {
                    res_tree = DIFF_OP_DATA_ADD(tree, save_top_node);
                    break;
                }
                case SUB:
                {
                    res_tree = DIFF_OP_DATA_SUB(tree, save_top_node);
                    break;
                }
                case MUL:
                {
                    res_tree = DIFF_OP_DATA_MUL(tree, save_top_node);
                    break;
                }
                case DIV:
                {
                    res_tree = DIFF_OP_DATA_DIV(tree, save_top_node);
                    break;
                }
                case DEGREE:
                {
                    res_tree = DIFF_OP_DATA_DEGREE(tree, save_top_node);
                    break;
                }
                break;
            };
            break;
        }
        case NUMBER:
        {
            res_tree = init_tree();
            Node *new_node = Create_Node(res_tree, 0);
            Set_new_High(res_tree, new_node);//TODO: to do smth with it
            break;
        }
        case VARIABLE:
        {
            res_tree = init_tree();
            Node *new_node = Create_Node(res_tree, 1);
            Set_new_High(res_tree, new_node);
            break;
        }
    case ONE_ARG_FUNC:
    {
    }
    };
    HANDLER_ERROR(Graphiz_Dump_Tree(res_tree, "gr_dump/res_tree_before_ret.dot"));
    return res_tree;
}

//TODO:поменять названия функций на более понятные и адекватные

Tree *Join_Trees_in_Node(Tree *left_tree, Tree *right_tree, OPERATORS op, Node *left_top_node, Node *right_top_node)
{
    if(left_top_node == nullptr)       left_top_node = left_tree->head_node.prev;
    if(right_top_node == nullptr)      right_top_node = right_tree->head_node.prev;

    Tree *new_tree = init_tree(left_tree->copacity + right_tree->copacity + 10);

    Tree_Copy(left_tree, new_tree, left_top_node);
    Node *left_node = left_top_node;

    Tree_Copy(right_tree, new_tree, right_top_node);
    Node *right_node = new_tree->head_node.prev;

    Node *top_node = Create_Node(new_tree, op, left_node, right_node);
    top_node->type_data = 1;//TODO:сделать универсальный тип

    HANDLER_ERROR(Set_new_High(new_tree, top_node));

    return new_tree;
}


Tree *Tree_Copy(Tree *tree_from_cpy, Tree *tree_where_cpy, Node *top_source) //TODO:сделать из этого метода и init подобие конструктора
{
    if(top_source == nullptr)           top_source = tree_from_cpy->head_node.prev;

    if(tree_where_cpy == nullptr)
    {
        tree_where_cpy = init_tree(tree_from_cpy->copacity);

        tree_where_cpy->head_node.prev;
        tree_where_cpy->factor_copacity = tree_from_cpy->factor_copacity;
        tree_where_cpy->stk_of_choise = tree_from_cpy->stk_of_choise;
    }
    else
    {
        if(tree_where_cpy->copacity < (tree_from_cpy->size + tree_where_cpy->size))
        {
            HANDLER_ERROR(Increase_copacity_Tree(tree_where_cpy));
        }
        tree_where_cpy->last_stor_node->list_node += tree_where_cpy->size;
    }
    tree_where_cpy->head_node.prev = Recr_Copy_Node(tree_where_cpy, tree_from_cpy, top_source);//как можно сделать копирование дерева
    tree_where_cpy->last_stor_node->list_node -= tree_where_cpy->size;//при создании дерева с нуля size будет равен 0
    tree_where_cpy->size += tree_from_cpy->size;

    return tree_where_cpy;
}


Node *Recr_Copy_Node(Tree *tree_to_cpy, Tree *tree_from_cpy, Node *save_top_node)
{
    if(save_top_node == nullptr)    save_top_node = tree_from_cpy->head_node.prev;

    Node *cur_node = Create_Node(tree_to_cpy, save_top_node->data);
    cur_node->type_data = save_top_node->type_data;

    if(save_top_node->left != nullptr)
    {
        Node *left_ptr = Recr_Copy_Node(tree_to_cpy, tree_from_cpy, save_top_node->left);
        HANDLER_ERROR(Insert_Node_to_Tree(left_ptr, cur_node, LEFT_INS));
    }
    if(save_top_node->right != nullptr)
    {
        Node *right_ptr = Recr_Copy_Node(tree_to_cpy, tree_from_cpy, save_top_node->right);
        HANDLER_ERROR(Insert_Node_to_Tree(right_ptr, cur_node, RIGHT_INS));
    }
    return cur_node;//TODO move to tree
}

Tree *DIFF_OP_DATA_ADD(Tree *tree, Node *save_top_node)
{
    Tree *left_tree  = Diff_Tree(tree, save_top_node->left);
    Tree *right_tree = Diff_Tree(tree, save_top_node->right);

    Tree *res_tree = Join_Trees_in_Node(left_tree, right_tree, ADD);

    return res_tree;
}

Tree *DIFF_OP_DATA_SUB(Tree *tree, Node *save_top_node)
{
    Tree *left_tree  = Diff_Tree(tree, save_top_node->left);
    Tree *right_tree = Diff_Tree(tree, save_top_node->right);

    Tree *res_tree = Join_Trees_in_Node(left_tree, right_tree, SUB);
}//TODO: join to one

Tree *DIFF_OP_DATA_MUL(Tree *tree, Node *save_top_node)
{
    Tree *left_left_tree = Diff_Tree(tree, save_top_node->left);
    HANDLER_ERROR(Graphiz_Dump_Tree(left_left_tree, "gr_dump/diff_left_left_tree.dot"));

    Tree *left_tree = Join_Trees_in_Node(left_left_tree, tree, MUL, nullptr, save_top_node->right);
    HANDLER_ERROR(Graphiz_Dump_Tree(left_tree, "gr_dump/diff_left_tree.dot"));

    Tree *right_left_tree = Diff_Tree(tree, save_top_node->right);

    Tree *right_tree = Join_Trees_in_Node(right_left_tree, tree, MUL, nullptr, save_top_node->left);
    HANDLER_ERROR(Graphiz_Dump_Tree(right_tree, "gr_dump/diff_right_tree.dot"));

    Tree *res_tree = Join_Trees_in_Node(left_tree, right_tree, ADD);//TODO: исправить что два раза копируется одно и тоже
    //TODO:сделать free
    HANDLER_ERROR(Graphiz_Dump_Tree(res_tree, "gr_dump/res_tree_after_MUL1.dot"));

    return res_tree;
}

Tree *DIFF_OP_DATA_DIV(Tree *tree, Node *save_top_node)
{
    Tree *left_left_tree = Diff_Tree(tree, save_top_node->left);
    HANDLER_ERROR(Graphiz_Dump_Tree(left_left_tree, "gr_dump/diff_left_left_tree.dot"));

    Tree *left_tree = Join_Trees_in_Node(left_left_tree, tree, MUL, nullptr, save_top_node->right);

    Tree *right_left_tree = Diff_Tree(tree, save_top_node->right);

    Tree *right_tree = Join_Trees_in_Node(right_left_tree, tree, MUL, nullptr, save_top_node->left);

    Tree *add_tree = Join_Trees_in_Node(left_tree, right_tree, SUB);

    Tree *div_tree = Join_Trees_in_Node(tree, tree, MUL, save_top_node->right, save_top_node->right); //TODO: add degree

    Tree *res_tree = Join_Trees_in_Node(add_tree, div_tree, DIV);

    HANDLER_ERROR(Graphiz_Dump_Tree(res_tree, "gr_dump/res_tree_after_DIV.dot"));

    return res_tree;
}

Tree *DIFF_OP_DATA_DEGREE(Tree *tree, Node *save_top_node)
{
    Tree *right_tree = Diff_Tree(tree, save_top_node->left);

    Tree *left_left_left_tree  = tree;
    Tree *left_left_right_left_tree = tree;

    Tree *left_left_right_right_tree = init_tree();
    Node *top_node = Create_Node(left_left_right_right_tree, 1);
    Set_new_High(left_left_right_right_tree, top_node);

    Tree *left_left_right = Join_Trees_in_Node(left_left_right_left_tree, left_left_right_right_tree, SUB, save_top_node->right);

    Tree * left_left_tree = Join_Trees_in_Node(left_left_left_tree, left_left_right, DEGREE, save_top_node->left);

    HANDLER_ERROR(Graphiz_Dump_Tree(left_left_tree, "gr_dump/left_left.dot"));//TODO:сделать чтобы функция принимала вершину дерева
    HANDLER_ERROR(Graphiz_Dump_Tree(right_tree, "gr_dump/right.dot"));

    Tree *left_right_tree = tree;
    Tree *left_tree = Join_Trees_in_Node(left_left_tree, left_right_tree, MUL, nullptr, save_top_node->right);

    HANDLER_ERROR(Graphiz_Dump_Tree(left_tree, "gr_dump/left.dot"));

    Tree *res_tree = Join_Trees_in_Node(left_tree, right_tree, MUL);

    HANDLER_ERROR(Graphiz_Dump_Tree(res_tree, "gr_dump/res_tree.dot"));
    return res_tree;
}
