#define K 10000000


int stack[K];

int stack_len = 0;

int pop_stack()
{
    if (!stack)
    {
        return -1;
    }
    stack_len--;
    return stack[stack_len];
}

void push_stack(int num)
{
    stack[stack_len] = num;
    stack_len++;
}

int stack_size()
{
    return stack_len;
}