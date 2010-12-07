#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <google/cmockery.h>

#include "cstl/cstl_def.h"
#include "cstl/cstl_iterator.h"
#include "cstl/cstl_vector_iterator.h"
#include "cstl/cvector.h"
#include "cstl_vector_aux.h"

#include "ut_def.h"
#include "ut_cstl_vector.h"

UT_SUIT_DEFINATION(cstl_vector, vector_init)

/*
 * test vector_init
 */
UT_CASE_DEFINATION(vector_init)
void test_vector_init__null_vector_container(void** state)
{
    expect_assert_failure(vector_init(NULL));
}

void test_vector_init__non_created(void** state)
{
    vector_t vec;
    vec._pc_start = (char*)0x887;
    expect_assert_failure(vector_init(&vec));
}

void test_vector_init__successfully_c_builtin(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_true(vector_size(pvec) == 0);
    assert_true(vector_capacity(pvec) == 0);

    vector_destroy(pvec);
}

void test_vector_init__successfully_cstl_builtin(void** state)
{
    vector_t* pvec = create_vector(vector_t<int>);
    vector_init(pvec);

    assert_true(vector_size(pvec) == 0);
    assert_true(vector_capacity(pvec) == 0);

    vector_destroy(pvec);
}

void test_vector_init__successfully_cstr(void** state)
{
    vector_t* pvec = create_vector(char*);
    vector_init(pvec);

    assert_true(vector_size(pvec) == 0);
    assert_true(vector_capacity(pvec) == 0);

    vector_destroy(pvec);
}

void test_vector_init__successfully_user_defined(void** state)
{
    typedef struct _tagtest_vector_init__successfully_user_defined
    {
        int n_elem;
    }_test_vector_init__successfully_user_defined_t;
    vector_t* pvec = NULL;

    type_register(_test_vector_init__successfully_user_defined_t, NULL, NULL, NULL, NULL);

    pvec = create_vector(_test_vector_init__successfully_user_defined_t);
    vector_init(pvec);

    assert_true(vector_size(pvec) == 0);
    assert_true(vector_capacity(pvec) == 0);

    vector_destroy(pvec);
}

/*
 * test vector_init_n
 */
UT_CASE_DEFINATION(vector_init_n)
void test_vector_init_n__null_vector_container(void** state)
{
    expect_assert_failure(vector_init_n(NULL, 10));
}

void test_vector_init_n__non_created(void** state)
{
    vector_t vec;
    vec._pc_finish = (char*)0x234;
    expect_assert_failure(vector_init_n(&vec, 10));
}

void test_vector_init_n__successfully_0(void** state)
{
    size_t i = 0;
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 0);
    assert_true(vector_size(pvec) == 0);
    assert_true(vector_capacity(pvec) == 0);

    vector_destroy(pvec);
}

void test_vector_init_n__successfully_c_builtin(void** state)
{
    size_t i = 0;
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 10);
    assert_true(vector_size(pvec) == 10);
    assert_true(vector_capacity(pvec) == 26);
    for(i = 0; i < vector_size(pvec); ++i)
    {
        assert_true(*(int*)vector_at(pvec, i) == 0);
    }

    vector_destroy(pvec);
}

void test_vector_init_n__successfully_cstr(void** state)
{
    size_t i = 0;
    vector_t* pvec = create_vector(char*);

    vector_init_n(pvec, 20);
    assert_true(vector_size(pvec) == 20);
    assert_true(vector_capacity(pvec) == 36);
    for(i = 0; i < vector_size(pvec); ++i)
    {
        assert_true(strcmp((char*)vector_at(pvec, i), "") == 0);
    }

    vector_destroy(pvec);
}

void test_vector_init_n__successfully_cstl_builtin(void** state)
{
    size_t i = 0;
    vector_t* pvec = create_vector(vector_t<int>);

    vector_init_n(pvec, 40);
    assert_true(vector_size(pvec) == 40);
    assert_true(vector_capacity(pvec) == 60);
    for(i = 0; i < vector_size(pvec); ++i)
    {
        assert_true(_vector_is_inited((vector_t*)vector_at(pvec, i)));
    }

    vector_destroy(pvec);
}

void test_vector_init_n__successfully_user_defined(void** state)
{
    typedef struct _tag_test_vector_init_n__successfully_user_defined
    {
        int n_elem;
    }_test_vector_init_n__successfully_user_defined_t;

    _test_vector_init_n__successfully_user_defined_t t_elem;
    size_t i = 0;
    vector_t* pvec = NULL;

    type_register(_test_vector_init_n__successfully_user_defined_t, NULL, NULL, NULL, NULL);
    pvec = create_vector(_test_vector_init_n__successfully_user_defined_t);

    vector_init_n(pvec, 160);
    assert_true(vector_size(pvec) == 160);
    assert_true(vector_capacity(pvec) == 240);
    memset(&t_elem, 0x00, sizeof(t_elem));
    for(i = 0; i < vector_size(pvec); ++i)
    {
        assert_true(memcmp((_test_vector_init_n__successfully_user_defined_t*)vector_at(pvec, i), &t_elem, sizeof(t_elem)) == 0);
    }

    vector_destroy(pvec);
}

/*
 * test vector_init_copy
 */
UT_CASE_DEFINATION(vector_init_copy)
void test_vector_init_copy__null_dest(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_init_copy(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_init_copy__null_src(void** state)
{
    vector_t* pvec = create_vector(int);

    expect_assert_failure(vector_init_copy(pvec, NULL));

    vector_destroy(pvec);
}

void test_vector_init_copy__non_create_dest(void** state)
{
    vector_t vec;
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    vec._t_typeinfo._pt_type = NULL;
    expect_assert_failure(vector_init_copy(&vec, pvec));

    vector_destroy(pvec);
}

void test_vector_init_copy__non_inited_src(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);

    pvec_src->_pc_finish = (char*)0x732;
    expect_assert_failure(vector_init_copy(pvec_dest, pvec_src));

    pvec_src->_pc_finish = NULL;
    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__not_same_type(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(double);
    vector_init(pvec_src);

    expect_assert_failure(vector_init_copy(pvec_dest, pvec_src));

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_c_builtin_empty(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_init(pvec_src);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 0);

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_c_builtin_little(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_init_elem(pvec_src, 10, 2010);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 26);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(*(int*)vector_at(pvec_dest, i) == 2010);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_c_builtin_huge(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_init_elem(pvec_src, 1000, 2010);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 1500);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(*(int*)vector_at(pvec_dest, i) == 2010);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_cstr_empty(void** state)
{
    vector_t* pvec_dest = create_vector(char*);
    vector_t* pvec_src = create_vector(char*);
    vector_init(pvec_src);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 0);

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_cstr_little(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(char*);
    vector_t* pvec_src = create_vector(char*);
    vector_init_elem(pvec_src, 19, "c-string");

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 35);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(strcmp((char*)vector_at(pvec_dest, i), "c-string") == 0);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_cstr_huge(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(char*);
    vector_t* pvec_src = create_vector(char*);
    vector_init_elem(pvec_src, 19000, "c-string");

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 28500);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(strcmp((char*)vector_at(pvec_dest, i), "c-string") == 0);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_cstl_builtin_empty(void** state)
{
    vector_t* pvec_dest = create_vector(vector_t<int>);
    vector_t* pvec_src = create_vector(vector_t<int>);
    vector_t* pvec_elem = create_vector(int);
    vector_init_elem(pvec_elem, 23, 21234);
    vector_init(pvec_src);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 0);

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
    vector_destroy(pvec_elem);
}

void test_vector_init_copy__successfully_cstl_builtin_little(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(vector_t<int>);
    vector_t* pvec_src = create_vector(vector_t<int>);
    vector_t* pvec_elem = create_vector(int);
    vector_init_elem(pvec_elem, 23, 21234);
    vector_init_elem(pvec_src, 32, pvec_elem);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 48);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(vector_equal((vector_t*)vector_at(pvec_dest, i), pvec_elem));
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
    vector_destroy(pvec_elem);
}

void test_vector_init_copy__successfully_cstl_builtin_huge(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(vector_t<int>);
    vector_t* pvec_src = create_vector(vector_t<int>);
    vector_t* pvec_elem = create_vector(int);
    vector_init_elem(pvec_elem, 23, 21234);
    vector_init_elem(pvec_src, 3702, pvec_elem);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 5553);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(vector_equal((vector_t*)vector_at(pvec_dest, i), pvec_elem));
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
    vector_destroy(pvec_elem);
}

typedef struct _tag_test_vector_init_copy__successfully_user_define
{
    int n_elem;
}_test_vector_init_copy__successfully_user_define_t;
void test_vector_init_copy__successfully_user_define_empty(void** state)
{
    vector_t* pvec_dest = NULL;
    vector_t* pvec_src = NULL;
    type_register(_test_vector_init_copy__successfully_user_define_t, NULL, NULL, NULL, NULL);
    pvec_dest = create_vector(_test_vector_init_copy__successfully_user_define_t);
    pvec_src = create_vector(_test_vector_init_copy__successfully_user_define_t);
    vector_init(pvec_src);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 0);

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_user_define_little(void** state)
{
    size_t i = 0;
    _test_vector_init_copy__successfully_user_define_t t_user;
    vector_t* pvec_dest = create_vector(_test_vector_init_copy__successfully_user_define_t);
    vector_t* pvec_src = create_vector(_test_vector_init_copy__successfully_user_define_t);
    t_user.n_elem = 100;
    vector_init_elem(pvec_src, 17, &t_user);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 33);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(memcmp(
            (_test_vector_init_copy__successfully_user_define_t*)vector_at(pvec_dest, i), &t_user, sizeof(t_user)) == 0);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy__successfully_user_define_huge(void** state)
{
    size_t i = 0;
    _test_vector_init_copy__successfully_user_define_t t_user;
    vector_t* pvec_dest = create_vector(_test_vector_init_copy__successfully_user_define_t);
    vector_t* pvec_src = create_vector(_test_vector_init_copy__successfully_user_define_t);
    t_user.n_elem = 100;
    vector_init_elem(pvec_src, 11117, &t_user);

    vector_init_copy(pvec_dest, pvec_src);
    assert_true(vector_size(pvec_dest) == vector_size(pvec_src));
    assert_true(vector_capacity(pvec_dest) == 16675);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(memcmp(
            (_test_vector_init_copy__successfully_user_define_t*)vector_at(pvec_dest, i), &t_user, sizeof(t_user)) == 0);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

/*
 * test vector_init_copy_range
 */
UT_CASE_DEFINATION(vector_init_copy_range)
void test_vector_init_copy_range__null_dest(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init(pvec);
    it_begin = vector_begin(pvec);
    it_end = vector_end(pvec);

    expect_assert_failure(vector_init_copy_range(NULL, it_begin, it_end));

    vector_destroy(pvec);
}

void test_vector_init_copy_range__invalid_begin_iterator(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init(pvec_src);
    it_begin = vector_begin(pvec_src);
    it_end = vector_end(pvec_src);

    it_begin._t_iteratortype = 23445;
    expect_assert_failure(vector_init_copy_range(pvec_dest, it_begin, it_end));

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy_range__invalid_end_iterator(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init(pvec_src);
    it_begin = vector_begin(pvec_src);
    it_end = vector_end(pvec_src);

    it_end._t_containertype = 9999;
    expect_assert_failure(vector_init_copy_range(pvec_dest, it_begin, it_end));

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy_range__invalid_range(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init_elem(pvec_src, 19, 323);
    it_begin = vector_begin(pvec_src);
    it_end = vector_end(pvec_src);

    expect_assert_failure(vector_init_copy_range(pvec_dest, it_end, it_begin));

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy_range__not_same_type(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(double);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init_elem(pvec_src, 19, 323);
    it_begin = vector_begin(pvec_src);
    it_end = vector_end(pvec_src);

    expect_assert_failure(vector_init_copy_range(pvec_dest, it_begin, it_end));

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy_range__successfully_empty_range(void** state)
{
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init_elem(pvec_src, 19, 323);
    it_begin = vector_begin(pvec_src);
    it_end = vector_end(pvec_src);

    vector_init_copy_range(pvec_dest, it_begin, it_begin);
    assert_true(vector_size(pvec_dest) == 0);
    assert_true(vector_capacity(pvec_dest) == 0);

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy_range__successfully_c_builtin(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(int);
    vector_t* pvec_src = create_vector(int);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init(pvec_src);
    for(i = 0; i < 10; ++i)
    {
        vector_push_back(pvec_src, i);
    }
    it_begin = iterator_next_n(vector_begin(pvec_src), 3);
    it_end = iterator_next_n(it_begin, 5);

    vector_init_copy_range(pvec_dest, it_begin, it_end);
    assert_true(vector_size(pvec_dest) == iterator_distance(it_begin, it_end));
    assert_true(vector_capacity(pvec_dest) == 21);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(*(int*)vector_at(pvec_dest, i) == i + 3);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy_range__successfully_cstr(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(char*);
    vector_t* pvec_src = create_vector(char*);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init_elem(pvec_src, 100, "abcdefg");
    it_begin = vector_begin(pvec_src);
    it_end = iterator_next_n(it_begin, 10);

    vector_init_copy_range(pvec_dest, it_begin, it_end);
    assert_true(vector_size(pvec_dest) == iterator_distance(it_begin, it_end));
    assert_true(vector_capacity(pvec_dest) == 26);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(strcmp((char*)vector_at(pvec_dest, i), "abcdefg") == 0);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

void test_vector_init_copy_range__successfully_cstl_builtin(void** state)
{
    size_t i = 0;
    vector_t* pvec_dest = create_vector(vector_t<int>);
    vector_t* pvec_src = create_vector(vector_t<int>);
    vector_t* pvec_elem = create_vector(int);
    vector_iterator_t it_begin;
    vector_iterator_t it_end;

    vector_init_elem(pvec_elem, 18, 23);
    vector_init_elem(pvec_src, 1004, pvec_elem);
    it_begin = iterator_next_n(vector_begin(pvec_src), 100);
    it_end = vector_end(pvec_src);

    vector_init_copy_range(pvec_dest, it_begin, it_end);
    assert_true(vector_size(pvec_dest) == iterator_distance(it_begin, it_end));
    assert_true(vector_capacity(pvec_dest) == 1356);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(vector_equal((vector_t*)vector_at(pvec_dest, i), pvec_elem));
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
    vector_destroy(pvec_elem);
}

void test_vector_init_copy_range__successfully_user_define(void** state)
{
    typedef struct _tag_test_vector_init_copy_range__successfully_user_define
    {
        int n_elem;
    }_test_vector_init_copy_range__successfully_user_define_t;

    size_t i = 0;
    vector_t* pvec_dest = NULL;
    vector_t* pvec_src = NULL;
    vector_iterator_t it_begin;
    vector_iterator_t it_end;
    _test_vector_init_copy_range__successfully_user_define_t t_user;

    type_register(_test_vector_init_copy_range__successfully_user_define_t, NULL, NULL, NULL, NULL);

    pvec_dest = create_vector(_test_vector_init_copy_range__successfully_user_define_t);
    pvec_src = create_vector(_test_vector_init_copy_range__successfully_user_define_t);

    t_user.n_elem = 28743;
    vector_init_elem(pvec_src, 10000, &t_user);
    it_begin = vector_begin(pvec_src);
    it_end = vector_end(pvec_src);

    vector_init_copy_range(pvec_dest, it_begin, it_end);
    assert_true(vector_size(pvec_dest) == iterator_distance(it_begin, it_end));
    assert_true(vector_capacity(pvec_dest) == 15000);
    for(i = 0; i < vector_size(pvec_dest); ++i)
    {
        assert_true(memcmp((_test_vector_init_copy_range__successfully_user_define_t*)vector_at(pvec_dest, i),
            &t_user, sizeof(t_user)) == 0);
    }

    vector_destroy(pvec_dest);
    vector_destroy(pvec_src);
}

/*
 * test vector_destroy
 */
UT_CASE_DEFINATION(vector_destroy)
void test_vector_destroy__null_vector_container(void** state)
{
    expect_assert_failure(vector_destroy(NULL));
}

void test_vector_destroy__non_created(void** state)
{
    vector_t vec;
    vec._pc_start = (char*)0x37;

    expect_assert_failure(vector_destroy(&vec));
}

void test_vector_destroy__created_non_inited(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_destroy(pvec);
    assert_true(pvec->_pc_start == NULL);
    assert_true(pvec->_pc_finish == NULL);
    assert_true(pvec->_pc_endofstorage == NULL);
}

void test_vector_destroy__inited_empty(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    vector_destroy(pvec);
    assert_true(pvec->_pc_start == NULL);
    assert_true(pvec->_pc_finish == NULL);
    assert_true(pvec->_pc_endofstorage == NULL);
}

void test_vector_destroy__inited_non_empty(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init_n(pvec, 199);

    vector_destroy(pvec);
    assert_true(pvec->_pc_start == NULL);
    assert_true(pvec->_pc_finish == NULL);
    assert_true(pvec->_pc_endofstorage == NULL);
}

/*
 * test vector_size
 */
UT_CASE_DEFINATION(vector_size)
void test_vector_size__null_vector_container(void** state)
{
    expect_assert_failure(vector_size(NULL));
}

void test_vector_size__non_inited(void** state)
{
    vector_t* pvec = create_vector(int);

    pvec->_pc_start = (char*)0x887;
    expect_assert_failure(vector_size(pvec));

    pvec->_pc_start = NULL;
    vector_destroy(pvec);
}

void test_vector_size__successfully_empty(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_true(vector_size(pvec) == 0);

    vector_destroy(pvec);
}

void test_vector_size__successfully_non_empty(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init_n(pvec, 45);

    assert_true(vector_size(pvec) == 45);

    vector_destroy(pvec);
}

/*
 * test vector_empty
 */
UT_CASE_DEFINATION(vector_empty)
void test_vector_empty__null_vector_container(void** state)
{
    expect_assert_failure(vector_empty(NULL));
}

void test_vector_empty__non_inited(void** state)
{
    vector_t* pvec = create_vector(int);

    pvec->_pc_finish = (char*)0x34;
    expect_assert_failure(vector_empty(pvec));

    pvec->_pc_finish = NULL;
    vector_destroy(pvec);
}

void test_vector_empty__successfully_empty(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init(pvec);
    assert_true(vector_empty(pvec));

    vector_destroy(pvec);
}

void test_vector_empty__successfully_non_empty(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 239);
    assert_false(vector_empty(pvec));

    vector_destroy(pvec);
}

/*
 * test vector_max_size
 */
UT_CASE_DEFINATION(vector_max_size)
void test_vector_max_size__null_vector_container(void** state)
{
    expect_assert_failure(vector_max_size(NULL));
}

void test_vector_max_size__non_inited(void** state)
{
    vector_t* pvec = create_vector(int);

    pvec->_pc_finish = (char*)0x783;
    expect_assert_failure(vector_max_size(pvec));

    pvec->_pc_finish = NULL;
    vector_destroy(pvec);
}

void test_vector_max_size__successfully(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init(pvec);
    assert_true(vector_max_size(pvec) != 0);

    vector_destroy(pvec);
}

/*
 * test vector_capacity
 */
UT_CASE_DEFINATION(vector_capacity)
void test_vector_capacity__null_vector_container(void** state)
{
    expect_assert_failure(vector_capacity(NULL));
}

void test_vector_capacity__non_inited(void** state)
{
    vector_t* pvec = create_vector(int);

    pvec->_pc_endofstorage = (char*)0x623;
    expect_assert_failure(vector_capacity(pvec));

    pvec->_pc_endofstorage = NULL;
    vector_destroy(pvec);
}

void test_vector_capacity__successfully_empty(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init(pvec);
    assert_true(vector_capacity(pvec) == 0);

    vector_destroy(pvec);
}

void test_vector_capacity__successfully_little(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 3);
    assert_true(vector_capacity(pvec) == 19);

    vector_destroy(pvec);
}

void test_vector_capacity__successfully_huge(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 3983);
    assert_true(vector_capacity(pvec) == 5974);

    vector_destroy(pvec);
}

/*
 * test vector_reserve
 */
UT_CASE_DEFINATION(vector_reserve)
void test_vector_reserve__null_vector_container(void** state)
{
    expect_assert_failure(vector_reserve(NULL, 100));
}

void test_vector_reserve__non_inited(void** state)
{
    vector_t* pvec = create_vector(int);

    pvec->_pc_finish = (char*)0x34;
    expect_assert_failure(vector_reserve(pvec, 100));    

    pvec->_pc_finish = NULL;
    vector_destroy(pvec);
}

void test_vector_reserve__shrink(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 100);
    assert_true(vector_capacity(pvec) == 150);
    vector_reserve(pvec, 0);
    assert_true(vector_capacity(pvec) == 150);

    vector_destroy(pvec);
}

void test_vector_reserve__not_change(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 100);
    assert_true(vector_capacity(pvec) == 150);
    vector_reserve(pvec, 150);
    assert_true(vector_capacity(pvec) == 150);

    vector_destroy(pvec);
}

void test_vector_reserve__expand(void** state)
{
    vector_t* pvec = create_vector(int);

    vector_init_n(pvec, 100);
    assert_true(vector_capacity(pvec) == 150);
    vector_reserve(pvec, 850);
    assert_true(vector_capacity(pvec) == 850);

    vector_destroy(pvec);
}

/*
 * test vector_equal
 */
UT_CASE_DEFINATION(vector_equal)
void test_vector_equal__null_first(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_equal(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_equal__null_second(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_equal(pvec, NULL));

    vector_destroy(pvec);
}

void test_vector_equal__non_inited_first(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_second);
    pvec_first->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_equal(pvec_first, pvec_second));

    pvec_first->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__non_inited_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    pvec_second->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_equal(pvec_first, pvec_second));

    pvec_second->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__not_same_type(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(double);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_false(vector_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__same_vector(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_true(vector_equal(pvec, pvec));

    vector_destroy(pvec);
}

void test_vector_equal__size_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init_n(pvec_second, 48);
    assert_false(vector_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__size_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_n(pvec_first, 233);
    vector_init_n(pvec_second, 48);
    assert_false(vector_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__size_equal_0(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_true(vector_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__size_equal_elem_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 4545);
    assert_false(vector_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__size_equal_elem_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 45);
    assert_false(vector_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_equal__size_equal_elem_equal(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 100);
    assert_true(vector_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

/*
 * test vector_not_equal
 */
UT_CASE_DEFINATION(vector_not_equal)
void test_vector_not_equal__null_first(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_not_equal(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_not_equal__null_second(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_not_equal(pvec, NULL));

    vector_destroy(pvec);
}

void test_vector_not_equal__non_inited_first(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_second);
    pvec_first->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_not_equal(pvec_first, pvec_second));

    pvec_first->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__non_inited_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    pvec_second->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_not_equal(pvec_first, pvec_second));

    pvec_second->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__not_same_type(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(double);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_true(vector_not_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__same_vector(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_false(vector_not_equal(pvec, pvec));

    vector_destroy(pvec);
}

void test_vector_not_equal__size_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init_n(pvec_second, 48);
    assert_true(vector_not_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__size_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_n(pvec_first, 233);
    vector_init_n(pvec_second, 48);
    assert_true(vector_not_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__size_equal_0(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_false(vector_not_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__size_equal_elem_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 4545);
    assert_true(vector_not_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__size_equal_elem_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 45);
    assert_true(vector_not_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_not_equal__size_equal_elem_equal(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 100);
    assert_false(vector_not_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

/*
 * test vector_less
 */
UT_CASE_DEFINATION(vector_less)
void test_vector_less__null_first(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_less(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_less__null_second(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_less(pvec, NULL));

    vector_destroy(pvec);
}

void test_vector_less__non_inited_first(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_second);
    pvec_first->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_less(pvec_first, pvec_second));

    pvec_first->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__non_inited_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    pvec_second->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_less(pvec_first, pvec_second));

    pvec_second->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__not_same_type(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(double);

    vector_init(pvec_first);
    vector_init(pvec_second);
    expect_assert_failure(vector_less(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__same_vector(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_false(vector_less(pvec, pvec));

    vector_destroy(pvec);
}

void test_vector_less__size_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init_n(pvec_second, 48);
    assert_true(vector_less(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__size_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_n(pvec_first, 233);
    vector_init_n(pvec_second, 48);
    assert_false(vector_less(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__size_equal_0(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_false(vector_less(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__size_equal_elem_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 4545);
    assert_true(vector_less(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__size_equal_elem_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 45);
    assert_false(vector_less(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less__size_equal_elem_equal(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 100);
    assert_false(vector_less(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

/*
 * test vector_less_equal
 */
UT_CASE_DEFINATION(vector_less_equal)
void test_vector_less_equal__null_first(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_less_equal(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_less_equal__null_second(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_less_equal(pvec, NULL));

    vector_destroy(pvec);
}

void test_vector_less_equal__non_inited_first(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_second);
    pvec_first->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_less_equal(pvec_first, pvec_second));

    pvec_first->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__non_inited_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    pvec_second->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_less_equal(pvec_first, pvec_second));

    pvec_second->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__not_same_type(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(double);

    vector_init(pvec_first);
    vector_init(pvec_second);
    expect_assert_failure(vector_less_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__same_vector(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_true(vector_less_equal(pvec, pvec));

    vector_destroy(pvec);
}

void test_vector_less_equal__size_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init_n(pvec_second, 48);
    assert_true(vector_less_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__size_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_n(pvec_first, 233);
    vector_init_n(pvec_second, 48);
    assert_false(vector_less_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__size_equal_0(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_true(vector_less_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__size_equal_elem_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 4545);
    assert_true(vector_less_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__size_equal_elem_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 45);
    assert_false(vector_less_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_less_equal__size_equal_elem_equal(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 100);
    assert_true(vector_less_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

/*
 * test vector_greater
 */
UT_CASE_DEFINATION(vector_greater)
void test_vector_greater__null_first(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_greater(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_greater__null_second(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_greater(pvec, NULL));

    vector_destroy(pvec);
}

void test_vector_greater__non_inited_first(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_second);
    pvec_first->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_greater(pvec_first, pvec_second));

    pvec_first->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__non_inited_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    pvec_second->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_greater(pvec_first, pvec_second));

    pvec_second->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__not_same_type(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(double);

    vector_init(pvec_first);
    vector_init(pvec_second);
    expect_assert_failure(vector_greater(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__same_vector(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_false(vector_greater(pvec, pvec));

    vector_destroy(pvec);
}

void test_vector_greater__size_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init_n(pvec_second, 48);
    assert_false(vector_greater(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__size_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_n(pvec_first, 233);
    vector_init_n(pvec_second, 48);
    assert_true(vector_greater(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__size_equal_0(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_false(vector_greater(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__size_equal_elem_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 4545);
    assert_false(vector_greater(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__size_equal_elem_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 45);
    assert_true(vector_greater(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater__size_equal_elem_equal(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 100);
    assert_false(vector_greater(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

/*
 * test vector_greater_equal
 */
UT_CASE_DEFINATION(vector_greater_equal)
void test_vector_greater_equal__null_first(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_greater_equal(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_greater_equal__null_second(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_greater_equal(pvec, NULL));

    vector_destroy(pvec);
}

void test_vector_greater_equal__non_inited_first(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_second);
    pvec_first->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_greater_equal(pvec_first, pvec_second));

    pvec_first->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__non_inited_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    pvec_second->_pc_finish = (char*)0x73;
    expect_assert_failure(vector_greater_equal(pvec_first, pvec_second));

    pvec_second->_pc_finish = NULL;
    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__not_same_type(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(double);

    vector_init(pvec_first);
    vector_init(pvec_second);
    expect_assert_failure(vector_greater_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__same_vector(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    assert_true(vector_greater_equal(pvec, pvec));

    vector_destroy(pvec);
}

void test_vector_greater_equal__size_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init_n(pvec_second, 48);
    assert_false(vector_greater_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__size_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_n(pvec_first, 233);
    vector_init_n(pvec_second, 48);
    assert_true(vector_greater_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__size_equal_0(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init(pvec_first);
    vector_init(pvec_second);
    assert_true(vector_greater_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__size_equal_elem_first_less_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 4545);
    assert_false(vector_greater_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__size_equal_elem_first_greater_than_second(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 45);
    assert_true(vector_greater_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

void test_vector_greater_equal__size_equal_elem_equal(void** state)
{
    vector_t* pvec_first = create_vector(int);
    vector_t* pvec_second = create_vector(int);

    vector_init_elem(pvec_first, 10, 100);
    vector_init_elem(pvec_second, 10, 100);
    assert_true(vector_greater_equal(pvec_first, pvec_second));

    vector_destroy(pvec_first);
    vector_destroy(pvec_second);
}

/*
 * test vector_assign
 */
UT_CASE_DEFINATION(vector_assign)
void test_vector_assign__null_dest(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_assign(NULL, pvec));

    vector_destroy(pvec);
}

void test_vector_assign__null_src(void** state)
{
    vector_t* pvec = create_vector(int);
    vector_init(pvec);

    expect_assert_failure(vector_assign(pvec, NULL));

    vector_destroy(pvec);
}
