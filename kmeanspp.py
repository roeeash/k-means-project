import numpy as np
import mykmeanssp as km


def kmeanspp(K,n,D,MAX, observations):
    ### centroid initialization and seed randomness
    np.random.seed(0)
    if (K <= 0 or n < K):
        print("wrong n,k-kmeans_pp")
        return  None
    if (D <= 0):
        print("negative d-kmeans_pp")
        return None

    centroids = np.empty((0, D), dtype=np.float64)
    index_list = np.empty((0, K), dtype=int)  # indexes of the chosen centroids
    ###choosing first centroid randomly###
    rand = np.random.choice(n, 1)[0]
    index_list = np.append(index_list, rand)
    new_centroid = observations[rand]
    centroids = np.append(centroids, np.array([new_centroid]), axis=0)
    min_dist = np.power((observations - new_centroid), 2).sum(axis=1)
    #### end of centroid initialization


    ### main loop

    for j in range(2, K + 1):
        sum_of_di = np.sum(min_dist, dtype=np.float64)
        ###updating the probabilities###
        prob_vector = np.true_divide(min_dist, sum_of_di)
        ###choosing the new centroid according to distribution p###
        rand_index = np.random.choice(n, 1, p=prob_vector)[0]
        index_list = np.append(index_list, rand_index)
        new_centroid = observations[rand_index]
        centroids = np.append(centroids, np.array([new_centroid]), axis=0)
        if (j < K):
            new_dist = np.power((observations - new_centroid), 2).sum(axis=1)
            min_dist = np.minimum(min_dist, new_dist)

    return km.kmeans(observations.tolist(), centroids.tolist(),MAX)
