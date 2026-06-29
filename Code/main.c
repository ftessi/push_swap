#include <stdio.h>
#include <stdlib.h>
#include "push_swap.h"

// Stack durumunu ekrana detaylı basan yardımcı test fonksiyonu
void print_stack_debug(t_stack_check *stack, char name)
{
	t_node *current;

	printf("=== STACK %c STATUS ===\n", name);
	if (!stack || !stack->head)
	{
		printf("Stack %c is EMPTY.\n\n", name);
		return ;
	}
	printf("Size: %d | Disorder: %d\n", stack->size, stack->disorder);
	printf("Min: %d | Max: %d\n", stack->min ? stack->min->value : 0, stack->max ? stack->max->value : 0);
	printf("Head: %d | Tail: %d\n", stack->head->value, stack->tail ? stack->tail->value : 0);
	printf("Elements (Forward) : ");
	current = stack->head;
	while (current)
	{
		printf("%d ", current->value);
		current = current->next;
	}
	printf("\nElements (Backward): ");
	current = stack->tail;
	while (current)
	{
		printf("%d ", current->value);
		current = current->prev;
	}
	printf("\n\n");
}

int main(void)
{
	t_stack_check	*stack_a;
	t_stack_check	*stack_b;
	t_node			*new_node;

	// 1. Stack Yapılarını Bellekte Başlatma
	stack_a = malloc(sizeof(t_stack_check));
	stack_b = malloc(sizeof(t_stack_check));
	if (!stack_a || !stack_b)
		return (1);
	
	// İçeriklerini sıfırlayalım
	stack_a->head = NULL; stack_a->tail = NULL; stack_a->max = NULL; stack_a->min = NULL; stack_a->size = 0; stack_a->disorder = 0;
	stack_b->head = NULL; stack_b->tail = NULL; stack_b->max = NULL; stack_b->min = NULL; stack_b->size = 0; stack_b->disorder = 0;

	printf("--- TEST 1: DÜĞÜM OLUŞTURMA VE BAĞLAMA ---\n");
	// Sırasıyla 24, -42 ve 5 sayılarını ekleyelim
	new_node = node_creator(24);
	node_connecter(stack_a, new_node);
	
	new_node = node_creator(-42);
	node_connecter(stack_a, new_node);
	
	new_node = node_creator(5);
	node_connecter(stack_a, new_node);

	// Stack değerlerini hesaplayan fonksiyonunuzu çağırın
	stack_o_meter(stack_a);
	print_stack_debug(stack_a, 'A');

	printf("--- TEST 2: SWAP (sa) HAMLESİ ---\n");
	printf("Executing sa...\n");
	sa(stack_a);
	stack_o_meter(stack_a); // Değişim sonrası metrikleri güncelle
	print_stack_debug(stack_a, 'A');

	printf("--- TEST 3: PUSH (pb) HAMLESİ ---\n");
	printf("Executing pb (Moving top of A to B)...\n");
	pb(stack_a, stack_b);
	stack_o_meter(stack_a);
	stack_o_meter(stack_b);
	print_stack_debug(stack_a, 'A');
	print_stack_debug(stack_b, 'B');

	printf("--- TEST 4: ROTATE (ra) HAMLESİ ---\n");
	printf("Executing ra...\n");
	ra(stack_a);
	stack_o_meter(stack_a);
	print_stack_debug(stack_a, 'A');

	printf("--- TEST 5: IS_SORTED KONTROLÜ ---\n");
	if (is_sorted(stack_a))
		printf("Stack A is fully sorted!\n");
	else
		printf("Stack A is NOT sorted yet.\n");

	// Bellek temizliği
	free_stack(stack_a);
	free_stack(stack_b);
	free(stack_a);
	free(stack_b);
	printf("\n--- Test Bitti, Bellek Temizlendi ---\n");
	return (0);
}
