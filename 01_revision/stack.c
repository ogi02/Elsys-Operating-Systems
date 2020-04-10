//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #2
// FILE PURPOSE:
// Implementation of stack in C with 6 mandatory functions.
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
    int *elements;
} stack;

void stack_init(stack *stack1){
	stack1->size = 0;
	stack1->capacity = 0;
	stack1->elements = malloc(stack1->capacity * sizeof(int));
}

void stack_destroy(stack *stack1){
	free(stack1->elements);
	stack1->elements = NULL;
}

int stack_empty(stack *stack1){
	return stack1->size == 0;
}

void stack_push(stack *stack1, int value){
	if(stack1->size == stack1->capacity){
		stack1->elements = realloc(stack1->elements, ((stack1->size + 1) * sizeof(int)));
		stack1->capacity += 2;
	}
	stack1->elements[stack1->size] = value;
	stack1->size ++;
}

int stack_top(stack *stack1){
	if(stack1->size == 0){
		return -1;
	}
	return stack1->elements[stack1->size - 1];
}

void stack_pop(stack *stack1){
	if(stack1->size == 0){
		return;
	}
	stack1->size --;
}

int main(){
	stack stack1;
	stack_init(&stack1);
	printf("%d\n", stack_empty(&stack1));
	stack_push(&stack1, 5);
	stack_push(&stack1, 8);
	printf("%d\n", stack_top(&stack1));
	stack_push(&stack1, 7);
	stack_push(&stack1, 6);
	//stack_print(&stack1);
	printf("%d\n", stack_empty(&stack1));
	printf("%d\n", stack_top(&stack1));
	stack_pop(&stack1);
	//stack_print(&stack1);
	printf("%d\n", stack_top(&stack1));
	printf("%d\n", stack_empty(&stack1));
	stack_pop(&stack1);
	stack_pop(&stack1);
	stack_pop(&stack1);
	printf("%d\n", stack_empty(&stack1));
	stack_destroy(&stack1);
	return 0;
}
