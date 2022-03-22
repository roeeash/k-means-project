from matricesModule import Laplacian , calculate_k
from kmeanspp import kmeanspp
import numpy as np
from gram_schmidt_module import qr_iteration

#this function runs the spectral clustering
MAX = 300

def NSC (observs , random , k = None):

    #calculates the needed matrices and k
    n = np.shape(observs)[0]
    L = Laplacian(observs ,n)
    A,Q = qr_iteration(L)
    eiges = A.diagonal()
    index = np.argsort(eiges)
    if(random):
       k = calculate_k(eiges[index] ,n)

    U = Q[:,index[0:k]]

    #normalaizing U
    norm = np.sum(np.power(U,2),axis=1)
    goods = ( norm != 0)
    norm[goods] = np.power(norm[goods],-0.5)  # boolean indexing
    T = np.diag(norm) @ U
    #runs kmeans
    res = kmeanspp(k, np.shape(T)[0], np.shape(T)[1] , MAX , T)
    return res , k





