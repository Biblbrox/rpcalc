/* test_ll.c - test driver for ll
 *
 * Copyright (C) 2014 Charles Lehner
 * This file is part of ll.
 *
 * ll is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * ll is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ll.h"
#include "mock_malloc.h"

#include <assert.h>

/* some strings to use */
static char
    *str1 = "apple",
    *str2 = "pie",
    *str3 = "sauce";

/* point struct for reduce function testing */
struct point {
    int x, y;
};

void test_strings()
{
    char **strs = NULL;
    char **strs_read;

    malloc_calls = 0;
    free_calls = 0;

    /* appending strings to list */
    strs = ll_new(strs);
    assert(strs);
    *strs = str1;
    strs = ll_new(strs);
    assert(strs);
    *strs = str2;
    strs = ll_new(strs);
    assert(strs);
    *strs = str3;

    assert(malloc_calls == 3);

    /* nexting at strings in list */
    strs_read = strs;
    strs_read = ll_next(strs_read);
    assert(strs_read && *strs_read == str2);
    strs_read = ll_next(strs_read);
    assert(strs_read && *strs_read == str1);
    strs_read = ll_next(strs_read);
    assert(strs_read == NULL);

    assert(free_calls == 0);

    /* popping strings off list */
    strs = ll_pop(strs);
    assert(strs && *strs == str2);
    strs = ll_pop(strs);
    assert(strs && *strs == str1);
    strs = ll_pop(strs);
    assert(strs == NULL);

    assert(free_calls == 3);
}

void test_ints()
{
    int *ints = NULL;

    /* appending ints to list */
    ints = ll_new(ints);
    assert(ints);
    *ints = 100;
    ints = ll_new(ints);
    assert(ints);
    *ints = 200;
    ints = ll_new(ints);
    assert(ints);
    *ints = 300;

    /* popping ints off list */
    ints = ll_pop(ints);
    assert(ints && *ints == 200);
    ints = ll_pop(ints);
    assert(ints && *ints == 100);
    ints = ll_pop(ints);
    assert(!ints);
}

void test_structs()
{
    struct {
        int bar;
        char *str;
    } *foo = NULL;

    /* append items */
    foo = ll_new(foo);
    assert(foo);
    foo->bar = 11;
    foo->str = str1;

    foo = ll_new(foo);
    assert(foo);
    foo->bar = 7;
    foo->str = str2;

    foo = ll_new(foo);
    assert(foo);
    foo->bar = 5;
    foo->str = str3;

    /* pop items */
    foo = ll_pop(foo);
    assert(foo && foo->bar == 7 && foo->str == str2);
    foo = ll_pop(foo);
    assert(foo && foo->bar == 11 && foo->str == str1);
    foo = ll_pop(foo);
    assert(!foo);
}

void test_iteration()
{
    int i, *num, *nums = NULL;

    /* put numbers in list */
    for (i = 0; i < 20; i += 2) {
        nums = ll_new(nums);
        assert(nums);
        *nums = i;
    }

    /* read back the numbers */
    for (num = nums; num; num = ll_pop(num)) {
        i -= 2;
        assert(*num == i);
    }
}

void test_free()
{
    int i, *nums = NULL;
    free_calls = 0;

    /* make a list */
    for (i = 0; i < 10; i++) {
        *(nums = ll_new(nums)) = i;
    }

    /* free the list */
    ll_free(nums);
    assert(free_calls == 10);
}

void test_empty()
{
    /* try next and pop on empty list */
    assert(ll_next(NULL) == NULL);
    assert(ll_pop(NULL) == NULL);
}

int point_sum LL_REDUCE_FN(struct point, *a, *b)
    a->x += b->x;
    a->y += b->y;
    return 0;
}

void test_reduce()
{
    struct point *point = NULL, sum = {0, 0};
    int i, j = 3, k = 12;

    /* make some points */
    for (i = j; i < k; i++) {
        point = ll_new(point);
        point->x = i;
        point->y = 1 << i;
    }

    /* sum the points */
    ll_reduce(point, point_sum, &sum);
    assert(sum.x == (k - j) * (k + j - 1) / 2);
    assert(sum.y == (1 << k) - (1 << j));
}

void test_reduce_empty()
{
    struct point sum = {0, 0};

    /* sum empty list of points */
    ll_reduce(NULL, point_sum, &sum);
    assert(sum.x == 0 && sum.y == 0);
}

int sum_lt5 LL_REDUCE_FN(int, *a, *b)
    int sum = *a + *b;

    /* stop if the sum gets too big */
    if (sum < 5) {
        *a = sum;
        return 0;
    }
    return 1;
}

void test_reduce_stop()
{
    int *num1, *num2, *num3, *num4, *nums_end, *nums = NULL;
    int sum = 0;
    free_calls = 0;

    *(nums = num1 = ll_new(nums)) = 2;
    *(nums = num2 = ll_new(nums)) = 2;
    *(nums = num3 = ll_new(nums)) = 2;
    *(nums = num4 = ll_new(nums)) = 2;

    nums_end = ll_reduce(nums, sum_lt5, &sum);

    /* the first two numbers get summed, and then the function stops the
     * reduce and the rest of the list is returned */
    assert(free_calls == 2);
    assert(sum == 4);
    assert(nums_end == num2);
}

void test_foreach()
{
    struct {
        int value;
    } *items = NULL;

    (items = ll_new(items))->value = 1;
    (items = ll_new(items))->value = 2;

    ll_foreach(items, item) {
        assert(item);
        assert(item->value == 1 || item->value == 2);
    }
}

int main()
{
    test_strings();
    test_ints();
    test_structs();
    test_iteration();
    test_free();
    test_empty();
    test_reduce();
    test_reduce_empty();
    test_reduce_stop();
    test_foreach();

    return 0;
}
