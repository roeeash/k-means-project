import numpy as np
import random
from sklearn import datasets
from spectral import NSC
from kmeanspp import kmeanspp
from files import tofile
import argparse
from distutils import util
from  visuallize_data import visuallize_data
#max n is 440 and max k is 20
def create_observs(n, k, rand):
  MAX = 300 #iterations
  MAX_n = 440 #for both dimensions
  MAX_k = 20


  if(rand):

     n =  random.randint(MAX_n/2,MAX_n)
     k =  random.randint(MAX_k/2,MAX_k)
     

  obs , clust = datasets.make_blobs(n_samples=n,n_features=np.random.choice(a=np.array([2,3]),p=np.array([1/2,1/2])),
                                         centers=k,random_state=None, return_centers=False)


  real_k = k #the k we generated
  # creating data.txt
  np.savetxt("data.txt",obs,delimiter=",")


  #spectral clustering
  res_spec , calc_k = NSC(obs , rand,k)

  if(rand):
      k = calc_k

  #calc ++ clustering
  res_pp = kmeanspp(k , n , np.shape(obs)[1] , MAX , obs)
  if( res_pp == None):
      print("problem with k-means")
      exit(0)

  # creating clusters.txt
  tofile(res_spec,res_pp)
 #creating clusters.pdf
  if(visuallize_data(obs,res_spec, res_pp , clust ,n , k , real_k) == 1):
      exit(0)





### user input - adding the arguments.
parser = argparse.ArgumentParser()

parser.add_argument("k", type = int)
parser.add_argument("N", type = int)
parser.add_argument("rand")
args = parser.parse_args()

if(len(vars(args)) != 3) :
    print("Not all the argsuments provided")
    exit(0)

k,n,rand = args.k,args.N,args.rand
rand = bool(util.strtobool(args.rand))

if(rand == False and (n<=k or k<=0)):
     print("wrong input")
     exit(0)

#### end of user input
create_observs(n , k , rand)