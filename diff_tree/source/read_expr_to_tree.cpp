#include "read_expr_to_tree.h"

CODE_ERRORS read_expr_to_tree(Tree *tree, char *buff)
{
    Node *top_node;
    int counter = read_last_op_priorety(tree, buff, &top_node);
    tree->head_node.prev = top_node;


    if(buff[counter] == '\0') return ALL_GOOD;

    LOG(1, stderr, " WRONG_DATA %c %d \n", buff[counter], buff[counter]);

    return WRONG_DATA;
}

int read_last_op_priorety(Tree *tree, char *beg_cur_buff, Node **top_node)
{
    Node *ptr_on_left_top_node = nullptr;
    Node *ptr_on_right_op_node = nullptr;
    Node *cur_top;
    struct TREE_DATA data = {};

    int counter = read_last_1_op_priorety(tree, beg_cur_buff, &ptr_on_left_top_node);
    cur_top = ptr_on_left_top_node;
    int delta_count = 0;
    do
    {
        counter += delta_count;

        switch (beg_cur_buff[counter])
        {
            case '+':
                data.type_data = TWO_ARG_FUNC;
                data.data = ADD;
                counter += 2;
                break;
            case '-':
                data.type_data = TWO_ARG_FUNC;
                data.data = SUB;
                counter += 2;
                break;
            default:
                LOG(1, stderr, "numb or variable\n");
        };
        delta_count = read_last_1_op_priorety(tree, beg_cur_buff + counter, &ptr_on_right_op_node);

        if(delta_count != 0)
        {
            cur_top = Create_Node(tree, data, nullptr, nullptr);
            cur_top->data.type_data = data.type_data;
            HANDLER_ERROR(Insert_Node_to_Tree(ptr_on_left_top_node, cur_top,  LEFT_INS));
            HANDLER_ERROR(Insert_Node_to_Tree(ptr_on_right_op_node, cur_top, RIGHT_INS));
        }
        ptr_on_left_top_node = cur_top;

        LOG(1, stderr, "read_last_op_priorety counter = %d delta_count = %d data = %d\n", counter, delta_count, data);
    }while(delta_count != 0);

    *top_node = ptr_on_left_top_node;

    return counter;
}

int read_last_1_op_priorety(Tree *tree, char *beg_cur_buff, Node **top_node)
{
    Node *ptr_on_left_top_node = nullptr;
    Node *ptr_on_right_op_node = nullptr;
    Node *cur_top;
    TREE_DATA data = {};

    int counter = read_last_2_op_priorety(tree, beg_cur_buff, &ptr_on_left_top_node);
    assert((ptr_on_left_top_node != nullptr) or (counter == 0));
    cur_top = ptr_on_left_top_node;
    int delta_count = 0;
    do
    {
        counter+= delta_count;
        switch (beg_cur_buff[counter])
        {
            case '*':
                data.type_data = TWO_ARG_FUNC;
                data.data = MUL;
                counter += 2;
                break;

            case '/':
                data.type_data = TWO_ARG_FUNC;
                data.data = DIV;
                counter += 2;
                break;
        };
        delta_count = read_last_2_op_priorety(tree, beg_cur_buff + counter, &ptr_on_right_op_node);

        if(delta_count != 0)
        {
            LOG(1, stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            cur_top = Create_Node(tree, data, nullptr, nullptr);
            HANDLER_ERROR(Insert_Node_to_Tree(ptr_on_left_top_node, cur_top,  LEFT_INS));
            HANDLER_ERROR(Insert_Node_to_Tree(ptr_on_right_op_node, cur_top, RIGHT_INS));
        }
        ptr_on_left_top_node = cur_top;

        LOG(1, stderr, "read_last_1_op_priorety counter = %d delta_count = %d data = %d\n", counter, delta_count, data);
    }while(delta_count != 0);

    *top_node = ptr_on_left_top_node;

    return counter;
}

int read_last_2_op_priorety(Tree *tree, char *beg_cur_buff, Node **top_node)
{
    Node *ptr_on_left_top_node = nullptr;
    Node *ptr_on_right_op_node = nullptr;
    Node *cur_top;
    TREE_DATA data = {};

    int counter = read_brackets_in_expr(tree, beg_cur_buff, &ptr_on_left_top_node);
    assert((ptr_on_left_top_node != nullptr) or (counter == 0));
    cur_top = ptr_on_left_top_node;
    int delta_count = 0;
    do
    {
        counter+= delta_count;
        switch (beg_cur_buff[counter])
        {
            case '^':
                data.type_data = TWO_ARG_FUNC;
                data.data = DEGREE;
                counter += 2;
                break;
        };
        delta_count = read_brackets_in_expr(tree, beg_cur_buff + counter, &ptr_on_right_op_node);

        if(delta_count != 0)
        {
            cur_top = Create_Node(tree, data, nullptr, nullptr);
            HANDLER_ERROR(Insert_Node_to_Tree(ptr_on_left_top_node, cur_top,  LEFT_INS));
            HANDLER_ERROR(Insert_Node_to_Tree(ptr_on_right_op_node, cur_top, RIGHT_INS));
        }
        ptr_on_left_top_node = cur_top;

        LOG(1, stderr, "read_last_2_op_priorety counter = %d delta_count = %d data = %d\n", counter, delta_count, data);
    }while(delta_count != 0);

    *top_node = ptr_on_left_top_node;

    return counter;
}

int read_brackets_in_expr(Tree *tree, char *beg_cur_buff, Node **top_node)
{
    int counter = 0;
    char nn[20] = {};

    if(beg_cur_buff[counter] == '(')
    {
        counter++;
        counter += read_last_op_priorety(tree, beg_cur_buff + counter, top_node);

        if(beg_cur_buff[counter] != ')') return 10000;
        counter++;
    }
    else
    {
        int numb = -1;
        if(beg_cur_buff[counter] == '0')
        {
            numb = 0;
        }
        else
        {
            numb = atoi(beg_cur_buff + counter);
            strcpy(nn, beg_cur_buff + counter);
            counter += strlen(beg_cur_buff + counter);

            TREE_DATA data = {};

            if(isalpha(nn[0]) != 0)
            {
                data.data = (int)nn[0];
                data.type_data = VARIABLE;
                (*top_node) = Create_Node(tree, data, nullptr, nullptr);
                counter++;
            }
            else if((numb != 0))
            {
                data.data = numb;
                data.type_data = NUMBER;
                (*top_node) = Create_Node(tree, data, nullptr, nullptr);
                counter++;
                LOG(1, stderr, "(*top_node) = %p\n", (*top_node));
            }
            else
            {
                return 0;
            }
        }
        LOG(1, stderr, "read_brackets_in_expr counter = %d numb = %d nn = %s\n", counter, numb, nn);
    }
    return counter;
}
