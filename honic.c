#include <stdio.h>
#include <stdlib.h>

typedef struct stackList
{
    int num;
    struct stackList *next;
} stack;
stack *A = NULL, *B = NULL, *C = NULL;
int  setps = 0;
stack *CreateStack(int n)
{
    stack *newStack = (stack *)malloc(sizeof(stack));
    newStack->num = n;
    newStack->next = NULL;
    return newStack;
}

void pushNewStack(stack **topStack, int n)
{
    stack *newStack = CreateStack(n);
    newStack->next = *topStack;
    *topStack = newStack;
}

void pushDisk(stack **topStack, stack *newDisk)
{
    newDisk->next = *topStack;
    *topStack = newDisk;
}

stack *removeDisk(stack **topStack)
{
    stack *removedDisk = *topStack;
    *topStack = (*topStack)->next;
    removedDisk->next = NULL;
    return removedDisk;
}

void displayStack(stack *s)
{
    stack *assiste = s;
    while (assiste != NULL)
    {
        printf("\t %d \t ", assiste->num);
        assiste = assiste->next;
    }
    printf("\n");
}
void displayAllStacks(){
    printf("peg A ");
    displayStack(A);
    printf("peg B ");
    displayStack(B);
    printf("peg C ");
    displayStack(C);
    printf("\n");
}

int move(stack **source, stack **destination)
{
    if (*source == NULL)
    {
        return 0;
    }
    if (*destination == NULL || (*source)->num < (*destination)->num)
    {
        stack *disk = removeDisk(source);
        pushDisk(destination, disk);
        return 1;
    }
    return 0;
}

void HANOI(int n, stack **source, stack **auxiliary, stack **destination)
{
    if (n == 0)
        return;
    HANOI(n - 1, source, destination, auxiliary);
    if (*source != NULL)
    {
        if (move(source, destination))
        {
            setps++;
            printf("step %d \n",setps);
            displayAllStacks();
        }
    }
    HANOI(n - 1, auxiliary, source, destination); 
}
 
int main()
{

    int n;
    printf("enter number of disks \n");
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("write a valid value (0 < n)");
        return 0;
    }
    

    for (int i = n; i >= 1; i--)
        pushNewStack(&A, i);

    displayStack(A);
    
    HANOI(n,&A,&B,&C);
    printf("total staps : %d ",setps);
    return 0;
}
