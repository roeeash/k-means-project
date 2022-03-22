from matplotlib import pyplot as plt
import numpy as np
from matplotlib.backends.backend_pdf import PdfPages
from files import jaccard

def visuallize_data(data,nsc_res, kmeans_res , initial_clusters ,n ,k , real_k):

 fig = plt.figure()

 pp = PdfPages('clusters.pdf')

 cluster_sp = np.full((n,) , 0 )  #c[i] means the cluster of observe i
 cluster_pp = np.full((n,), 0)

 #the colors that will be used
 colors = np.array(["black" , "purple" , "yellow" , "brown" , "orange" , "red" , "blue" , "pink" , "magenta" , "green", "lavender" , "salmon" , "khaki",
 "teal" , "thistle" , "olive" , "sienna" , "lightgreen" , "lightpink" , "orchid" , "indigo", "skyblue" , "aqua", "slategray" , "springgreen" , "mintcream",
 "limegreen" , "lightcoral" , "gold" , "goldenrod","mistyrose","tomato","darksalmon","sienna","seashell","chocolate","peru","linen","aliceblue","dodgerblue"])

 #creating the color categories
 for i in range(k):
     cluster_sp[nsc_res[i]] = i
     cluster_pp[kmeans_res[i]] = i


 dim = np.shape(data)[1]

 #plotting
 if (dim == 3):
     ax = fig.add_subplot(121, projection='3d')
     ax.scatter(data[:, 0], data[:, 1], data[:, 2] , c=colors[cluster_sp])
     ax.set_title(label = "Normalized Spectral Clustering")

     ax = fig.add_subplot(122, projection='3d')
     ax.scatter(data[:, 0], data[:, 1], data[:, 2] ,c=colors[cluster_pp])
     ax.set_title(label = "K-means")

 else:
     plt.subplot(121)
     plt.scatter(data[:, 0], data[:, 1], c=colors[cluster_sp])
     plt.title("Normalized Spectral Clustering")
     plt.subplot(122)
     plt.scatter(data[:, 0], data[:, 1], c=colors[cluster_pp])
     plt.title("K-means")

 plt.subplots_adjust( wspace=0.25)

 #calculating the jaccard measure
 jac = jaccard(cluster_sp.tolist() , cluster_pp.tolist() , initial_clusters.tolist())
 if(jac == None):
     return 1
 jac = list(jac.items())

 #adding the text
 plt.figtext(0.25, -0.05, "Data was generated from the values:" , fontsize = 12)
 plt.figtext(0.4 , -0.1 ,"n = " + str(n) + ", k = " + str(real_k) , fontsize = 12 )
 plt.figtext(0.2 , -0.15 , "The k that was used for both algorithms was " + str(k) , fontsize = 12)
 plt.figtext(0.18 , -0.2 , "The Jaccard measure of Spectral Clustering:" + str(jac[0][0])[0:4] , fontsize = 12)
 plt.figtext(0.25, -0.25, "The Jaccard measure of K-means:" + str(jac[0][1])[0:4] , fontsize = 12)

 pp.savefig(fig , bbox_inches='tight')
 pp.close()






