import numpy as np
import math
EPSILON = 0.0001 #as instructed


# qr iteration algorithm
def modified_schmidt(U):
    ###initialization###
    #U = A.copy()
    n = np.shape(U)[0] #the dimension of the matrix
    R = np.zeros_like(U , np.float64)
    Q = np.zeros_like(U, np.float64)

    ###end of init###
    ###main loop###
    for i in range(n):

        R[i][i] = np.linalg.norm(U[:,i])
        Q[:, i] = np.true_divide(U[:,i],R[i][i])

        R[i,i+1:n] = Q[:,i] @ U[:,i+1:n]
        U[:,i+1:n] = U[:,i+1:n] -  Q[:,i].reshape(n,1) @ (R[i,i+1:n].reshape(1,n-i-1))

    return (Q,R)



#n is A's dimension

def qr_iteration(A):

    n = np.shape(A)[0]  # the dimension of the matrix
    A_bar = A
    Q_bar = np.eye(n)

    for i in range(n):
        Q,R = modified_schmidt(A_bar)
        A_bar= np.dot(R,Q)
        m = Q_bar @ Q
        Temp = np.absolute(np.absolute(Q_bar) - np.absolute(m))
        if( np.all(Temp<= EPSILON)):
            return A_bar,Q_bar

        Q_bar = m

    return A_bar,Q_bar


