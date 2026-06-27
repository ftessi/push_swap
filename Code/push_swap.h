/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:33:03 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/25 19:09:05 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifdef PUSH_SWAP_H
# define PUSH_SWAP_H 

# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

typedef struct s_stack_node
{
    int                 value;         // Sayının kendisi
    int                 index;         // Doğru sıralamadaki indeks değeri
    int                 pos;           // Yığındaki güncel pozisyonu
    int                 cost;          // Bu elemanı taşımak için toplam hamle maliyeti
    bool                above_median;  // Yığının ortasının (median) üstünde mi? (1: Üst, 0: Alt)
    struct s_stack_node *target_node;  // Karşı yığındaki hedef düğümün adresi
    struct s_stack_node *next;         // Sonraki düğüm
    struct s_stack_node *prev;         // Önceki düğüm
}   t_stack_node;