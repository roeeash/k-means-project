#define PY_SSIZE_T_CLEAN
#include <stdio.h>
#include <Python.h>


static PyObject*  tofile(PyObject *self, PyObject *args);
static long* list_of_ints(PyObject* list, Py_ssize_t list_size);
static int lists_of_ints(PyObject* list , FILE* file);
static PyObject* jaccard(PyObject *self, PyObject *args);

static PyObject*  tofile(PyObject *self, PyObject *args){
    FILE  *fptr_out;
    PyObject *_listc, *_listo;
    int k ,e1,e2;
    fptr_out = fopen("clusters.txt" , "w");
    /*the observs from python*/
    if(!PyArg_ParseTuple(args, "OO", &_listo , &_listc)) {
        return NULL;
    }

    k = PyList_Size(_listc);
    fprintf(fptr_out , "%d\n" , k);
    e1 = lists_of_ints(_listo , fptr_out);
    e2 = lists_of_ints(_listc , fptr_out);
    if(e1 == 1 || e2 == 1){printf("problem in files");}
    fclose(fptr_out);
     Py_RETURN_NONE;

}


static PyObject* jaccard(PyObject *self, PyObject *args){
    PyObject *_listc, *_listp , *_listr;
    long *a1 , *a2 , *a3;
    int  i,j;
    double res_sp , res_pp , real_res_sp , real_res_pp;
    Py_ssize_t n;

    if(!PyArg_ParseTuple(args, "OOO", &_listc , &_listp , &_listr)) {
        return NULL;
    }
    n  = PyList_Size(_listc);
    a1 = list_of_ints(_listc , n);
    a2 = list_of_ints(_listp , n);
    a3 = list_of_ints(_listr , n);
    if(a1 == NULL || a2 == NULL || a3 == NULL){
        printf("problem with jaccard");
        Py_RETURN_NONE;
    }
    res_sp = 0;
    res_pp = 0;
    real_res_sp = 0;
    real_res_pp = 0;
    for(i=0; i<n; i++){
        for(j=i+1; j<n; j++){
             if(a3[i] == a3[j] && a1[i] == a1[j]){
                 real_res_sp++;
                 res_sp++;
                 } else if(a3[i] == a3[j] || a1[i] == a1[j]){
                        real_res_sp++;
                 }

                 if(a3[i] == a3[j] && a2[i] == a2[j]){
                 real_res_pp++;
                 res_pp++;
                 } else if(a3[i] == a3[j] || a2[i] == a2[j]){
                        real_res_pp++;
                 }
             }
        }
    return Py_BuildValue("{d:d}", res_sp/real_res_sp, res_pp/real_res_pp);

}





static long* list_of_ints(PyObject* list, Py_ssize_t list_size){
    Py_ssize_t i;
    PyObject *item;
    long *my_c_list;

    if (!PyList_Check(list))  /*check if we have a list*/
        return NULL;
    my_c_list = malloc(sizeof(long) * list_size);
    /* Check that we got the memory from OS. */
    if(my_c_list == NULL){return NULL;}
    for (i = 0; i < list_size; i++) { /*inserting the items to c list*/
        item = PyList_GetItem(list, i);
        my_c_list[i] =  PyLong_AsLong(item);
    }
    return my_c_list;

}


/*returns the pointer list of observs*/
static int lists_of_ints(PyObject* list , FILE* file){
    Py_ssize_t i,j, N ,s;
    PyObject  *item;
    long** observs;/*the pointer to be returned*/
    N = PyList_Size(list); //number of observations
    observs = calloc(N,sizeof(long*));
    /* Check that we got the memory from OS. */
    if( observs == NULL ){printf("problem with list of ints"); return 1;}
    for (i = 0; i < N; i++) {
        item = PyList_GetItem(list, i);
        if (!PyList_Check(item)){  /* We only use lists */
            continue;
        }
        s = PyList_Size(item);
        observs[i] = list_of_ints(item,s); /*PyList_Size(item)=d*/
        if( observs[i] == NULL ){printf("problem with list of ints"); return 1;}
        for(j=0 ; j<s-1 ; j++){
            fprintf(file,"%ld, ",observs[i][j]);
        }
        fprintf(file,"%ld\n",observs[i][s-1]);

    }
    return 0;
}



static PyMethodDef Methods[] = {
        {"tofile",                   /* the Python method name that will be used */
                (PyCFunction) tofile, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,           /* flags indicating parametersaccepted for this function */
                        PyDoc_STR("prints clusters")}, /*  The docstring for the function */
        {"jaccard",                   /* the Python method name that will be used */
                (PyCFunction) jaccard, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,           /* flags indicating parametersaccepted for this function */
                        PyDoc_STR("calc jaccard")}, /*  The docstring for the function */



        {NULL, NULL, 0, NULL}
};
/* This initiates the module using the above definitions. */
static struct PyModuleDef _moduledef = {
        PyModuleDef_HEAD_INIT,
        "files", /* name of module */
        NULL, /* module documentation */
        -1,
        Methods /* the PyMethodDef array from before containing the methods of the extension */
};
PyMODINIT_FUNC
PyInit_files(void)
{
    return PyModule_Create(&_moduledef);

}