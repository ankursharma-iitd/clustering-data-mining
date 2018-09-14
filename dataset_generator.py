# param-1 : number of samples
# param-2 : number of dimensions
# param-3 : number of clusters

from sklearn.datasets.samples_generator import make_blobs
import sys

number_of_samples = int(sys.argv[1])
number_of_dimensions = int(sys.argv[2])
number_of_clusters = int(sys.argv[3])

X, y = make_blobs(
    n_samples=number_of_samples,
    centers=number_of_clusters,
    n_features=number_of_dimensions,
    random_state=0)

#creating the dataset for clustering
filename = 'clustering_dataset_' + str(number_of_samples) + '_' + str(number_of_dimensions) + '_' + str(number_of_clusters) + '.txt'
with open(filename, 'a+') as file:
    for i in range(X.shape[0]):
        row = ' '.join(X[i, :].astype(dtype='string'))
        file.write(row + '\n')
    file.close()

#creating the correct labels for these blobs
filename = 'clustering_labels_' + str(number_of_samples) + '_' + str(number_of_dimensions) + '_' + str(number_of_clusters) + '.txt'
with open(filename, 'a+') as file:
    for label in y:
        file.write(str(label) + '\n')
    file.close()

#flag when the dataset has been created
print('DATASET AND LABELS HAVE BEEN CREATED.')
