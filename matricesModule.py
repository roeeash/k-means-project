import numpy as np

#The matrix W
def weighted_matrix(observs , n):

    W = np.zeros((n, n)) #the resulted matrix

    for i in range(n):
        temp = np.linalg.norm(observs[i+1:n] - observs[i] , axis = 1)
        W[i,i+1:n] = np.exp(np.multiply(temp,(-1/2)))
        W[i][i] = 0
        W[i+1:n,i] = W[i,i+1:n] #w_ij = w_ji

    return W

#The matrix D^(-1/2)
def diagonal_root (W):
    diag = np.sum(W,axis=1)   #sum of rows
    goods = (diag != 0)
    diag[goods] = np.power(diag[goods], (-1 / 2))
    return np.diag(diag)

#The Laplacian
def Laplacian(observs , n):
    I = np.eye(n)
    W = weighted_matrix(observs , n)
    D_root = diagonal_root (W)
    L = I - D_root @ W @ D_root
    return L

#determine k
def calculate_k(eigen , n):
  # get eigenvalues
  n = int(n/2)
  delta = eigen[1:n] - eigen[0:n-1]
  return np.argmax(delta) + 1