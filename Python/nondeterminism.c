#include "mt64.h"
#include "Python.h"
#include <string.h>
#include <stdlib.h>
#include "nondeterminism.h"
/*
enum NonDexMode {
    OFF,
    FULL, 
    ONE
};*/

static enum NonDexMode mode = OFF;
static unsigned int seed = 1;

NonDexMode NonDex_Mode()
{
    return mode;
}

// Initializes seed for FULL mode
void NonDex_InitFull(Py_ssize_t init_seed)
{
    mode = FULL;
    seed = init_seed;
    init_genrand64(init_seed);
}

// Initializes seed for ONE mode
void NonDex_InitOne(Py_ssize_t init_seed)
{
    mode = ONE;
    seed = init_seed;
    init_genrand64(init_seed);
}
// Shuffles a Python List object in place
void NonDex_Shuffle_List(PyListObject *list)
{
    if (mode == OFF || !list) {
        return;
    }

    if (mode == ONE) {
        init_genrand64(seed);
    }
    
    Py_ssize_t n = PyList_GET_SIZE(list);
    
    if (n <= 1) return;
    
    for (Py_ssize_t pos = n - 1; pos >= 0; pos --) {
        Py_ssize_t swap_pos = genrand64_int64() % (pos + 1);
        PyObject *temp = PyList_GET_ITEM(list, swap_pos);
        PyList_SET_ITEM(list, swap_pos, PyList_GET_ITEM(list, pos));
        PyList_SET_ITEM(list, pos, temp);
    }
}
// Shuffles an array of Py_ssize_t
void NonDex_Shuffle_Py_ssize_t(Py_ssize_t *array, Py_ssize_t length)
{
    if (mode == OFF) {
        return;
    }

    if (mode == ONE) {
        init_genrand64(seed);
    }
    
    for (Py_ssize_t pos = length - 1; pos >= 0; pos--) {
        Py_ssize_t swap_pos = genrand64_int64() % (pos + 1);
        Py_ssize_t temp = array[pos];
        array[pos] = array[swap_pos];
        array[swap_pos] = temp;
    }
}
