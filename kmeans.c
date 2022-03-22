#define PY_SSIZE_T_CLEAN
#define epsilon 0.0001
#include <Python.h>



typedef struct
{
    double *sum_elements, *centroid;
    int num_elem;

}cluster;

static int calc_centroid(cluster*,Py_ssize_t,Py_ssize_t);
static double distance(double*, double*, Py_ssize_t);
static int min_dist(cluster* , double*, Py_ssize_t, Py_ssize_t);
static void add(double*, double*, Py_ssize_t);
static void init(cluster*, Py_ssize_t, Py_ssize_t);
static void free1(cluster*, double**,double**, Py_ssize_t ,Py_ssize_t);
static double* list_of_doubles(PyObject*, Py_ssize_t);
static double** lists_of_doubles(PyObject* );
static void reverse_copying(cluster* S,Py_ssize_t, Py_ssize_t , double**);
static PyObject* printobservs(double** observs , cluster* S , int k ,int d , int N);
static PyObject* kmeansf(PyObject *self, PyObject *args);
static PyObject* makelist(int* s, Py_ssize_t size);
static PyObject *makelists(int** s, Py_ssize_t k, int* index);

typedef  double   float64;

static PyObject* kmeansf(PyObject *self, PyObject *args){
    /*decleratio of variables*/
    Py_ssize_t k , N, d, clust_index, i, j;
    int max_iter;
    PyObject *_listc, *_listo , *finallist;
    double** observs;
    double** clusters;
    cluster *S ;
    int change_flag;
    change_flag = 0;

    /*the arguments clusters and observs from python*/
    if(!PyArg_ParseTuple(args, "OOi", &_listo , &_listc, &max_iter)) {
        return NULL;
    }
/*intialize the values*/
    k = PyList_Size(_listc);
    N = PyList_Size(_listo);
    d = PyList_Size(PyList_GetItem(_listo,0));

/*initialize c lists from python lists*/
    observs = lists_of_doubles(_listo);
    clusters = lists_of_doubles(_listc);
    if( observs == NULL || clusters == NULL){Py_RETURN_NONE;}
    /*allocate space for structs*/
    S = calloc(k,sizeof(cluster));
    if(S == NULL){Py_RETURN_NONE;}
    for(i=0;i<k;i++){   /*memory for clusters*/
        S[i].sum_elements = (double*)calloc(d,sizeof(double));
        if(S[i].sum_elements == NULL){Py_RETURN_NONE;}
        S[i].centroid = (double*)calloc(d,sizeof(double));
        assert(S[i].centroid != NULL);
        if(S[i].centroid == NULL){Py_RETURN_NONE;}
    }
/*copy the clusters to the struct that the program uses - S */
    reverse_copying(S,k,d,clusters);
/*the program*/
    i = 0;
    while(i < max_iter && change_flag == 0){ /*steps of algorithm*/
        init(S,d,k);
        for(j=0;j<N;j++){    /* running upon all the vectors*/
            clust_index = min_dist(S,observs[j],d,k);
            S[clust_index].num_elem++;
            add(S[clust_index].sum_elements, observs[j],d);
        }
        change_flag = calc_centroid(S,d,k);  /*changing centroids*/
        i++;
    }

   finallist = printobservs( observs ,  S , k , d , N);
    /*free memory*/
    free1(S,observs,clusters,k,N);
   return finallist;

}




static PyObject* printobservs(double** observs , cluster* S , int k ,int d , int N){

    int temp;
    int** array;
    Py_ssize_t i;
    int index[k];
    array = calloc(k,sizeof(int*));

    if(array == NULL){ Py_RETURN_NONE;}

    for(i=0;i<k;i++){
        array[i] = malloc(sizeof(int) * N);
        if(array[i] == NULL){Py_RETURN_NONE;}
        index[i] = 0;
    }

    for (i = 0; i < N; i++) {
        temp = min_dist(S,observs[i],d,k);
        array[temp][index[temp]] = i;
        index[temp]++;
    }



    return makelists(array , k , index);
}

static PyObject* makelist(int* s, Py_ssize_t size) {
    PyObject *l = PyList_New(size);
    Py_ssize_t i;
    for (i = 0; i < size; i++) {
        PyList_SetItem(l, i, Py_BuildValue("i", s[i]));
    }
    return l;
}

static PyObject *makelists(int** s, Py_ssize_t k, int* index){
    Py_ssize_t i;
    PyObject *l = PyList_New(k);
    for(i=0;i<k;i++)
        PyList_SetItem(l, i, makelist(s[i],index[i]));
    return l;

}

int calc_centroid(cluster* S, Py_ssize_t d , Py_ssize_t k ){ /*return 0 if there is a change*/
    int i,j;
    int change;
    double temp;
    change = 0;
    for(j=0;j<k;j++){ /*centroids*/
        if(S[j].num_elem != 0){
            for(i=0;i<d;i++){
                temp = *(S[j].centroid + i);
                *(S[j].centroid+i) = *(S[j].sum_elements + i)/(double)(S[j].num_elem);
                if((temp >= *(S[j].centroid+i)+epsilon) ||(temp <= *(S[j].centroid+i)-epsilon))
                    change++;
            }
        }}
    if(change>0)
        return 0;
    return 1;
}

static double distance(double* x, double* y, Py_ssize_t d){
    int j;
    double dist;
    dist = 0;
    for(j=0;j<d;j++)
        dist+= (*(x+j) - *(y+j))*(*(x+j) - *(y+j));
    return dist;
}

static int min_dist(cluster* S, double* vector, Py_ssize_t d, Py_ssize_t k){
    int j, min_index;
    double min_val, temp;
    min_index = 0;
    min_val = distance(vector,S[0].centroid,d);
    for(j=1;j<k;j++){ /*clusters*/
        temp = distance(vector,S[j].centroid,d);
        if(min_val > temp){
            min_val = temp;
            min_index = j;
        }

    }
    return min_index;

}

static void add(double* addto, double* addfrom, Py_ssize_t d){
    int j;
    for(j=0;j<d;j++)
        *(addto+j) += *(addfrom+j);
}


static void init(cluster* S, Py_ssize_t d ,Py_ssize_t k){
    int l,n;
    for(l=0;l<k;l++){   /*initialize clusters*/
        for(n=0;n<d;n++)
            S[l].sum_elements[n] = .0;
        S[l].num_elem = 0;
    }
}

static void reverse_copying(cluster* S,Py_ssize_t k, Py_ssize_t d , double** clusters){
    int i,j;
    for(i=0;i < k;i++){
        for(j=0; j<d;j++){
            S[i].centroid[j] = clusters[i][j];

        }
    }
}


static void free1(cluster* S, double** observs, double** clusters, Py_ssize_t k ,Py_ssize_t N){
    int i;
    for(i=0;i<k;i++){
        free(S[i].centroid);
        free(S[i].sum_elements);
        free(clusters[i]);
    }
    free(S);
    for(i=0;i<N;i++){
        free(observs[i]);
    }
    free(observs);
    free(clusters);
}




/*returns the pointer to clusters or to observs*/
static double* list_of_doubles(PyObject* list, Py_ssize_t list_size){
    Py_ssize_t i;
    PyObject *item;
    double *my_c_list;

    if (!PyList_Check(list))  /*check if we have a list*/
        return NULL;
    my_c_list = malloc(sizeof(double) * list_size);
    /* Check that we got the memory from OS. */
    if(my_c_list == NULL){return NULL;}
    for (i = 0; i < list_size; i++) { /*inserting the items to c list*/
        item = PyList_GetItem(list, i);
        my_c_list[i] = PyFloat_AsDouble(item);
    }
    return my_c_list;

}


/*returns the pointer list of observs*/
static double** lists_of_doubles(PyObject* list){
    Py_ssize_t i, N;
    PyObject  *item;
    double** observs;/*the pointer to be returned*/
    N = PyList_Size(list); //number of observations
    observs = calloc(N,sizeof(float*));
    /* Check that we got the memory from OS.*/
    if(observs == NULL){return NULL;}
    for (i = 0; i < N; i++) {
        item = PyList_GetItem(list, i);
        if (!PyList_Check(item)){  /* We only use lists */
            continue;
        }
        observs[i] = list_of_doubles(item,PyList_Size(item)); /*PyList_Size(item)=d*/
        if(observs[i] == NULL){return NULL;}
    }
    return observs;
}


/*capi part*/

static PyMethodDef Methods[] = {
        {"kmeans",                   /* the Python method name that will be used */
                (PyCFunction) kmeansf, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,           /* flags indicating parametersaccepted for this function */
                        PyDoc_STR("calculates clusters")}, /*  The docstring for the function */
        {NULL, NULL, 0, NULL}
};
/* This initiates the module using the above definitions. */
static struct PyModuleDef _moduledef = {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp", /* name of module */
        NULL, /* module documentation */
        -1,
        Methods /* the PyMethodDef array from before containing the methods of the extension */
};
PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
    return PyModule_Create(&_moduledef);

}
