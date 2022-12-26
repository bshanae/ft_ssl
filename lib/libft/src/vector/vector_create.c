/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshanae <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 16:07:29 by bshanae           #+#    #+#             */
/*   Updated: 2022/12/26 11:20:48 by v.belchenko      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_vector.h"
#include <stdlib.h>

t_vector					*vector_create(size_t element_size)
{
	t_vector				*vector;

	vector = malloc(sizeof(t_vector));
	vector->storage = NULL;
	vector->element_size = 0;
	vector->capacity = 0;
	vector->length = 0;
	vector->storage = malloc(element_size * VECTOR_ALLOC_INIT);
	if (!vector->storage)
		return (vector);
	vector->element_size = element_size;
	vector->capacity = VECTOR_ALLOC_INIT;
	return (vector);
}
