import numpy as np
from sklearn.manifold import TSNE
import matplotlib.pyplot as plt
import sys
from sklearn.decomposition import PCA

def get_cmap(n, name='hsv'):
    '''Returns a function that maps each index in 0, 1, ..., n-1 to a distinct 
    RGB color; the keyword argument name must be a standard mpl colormap name.'''
    return plt.cm.get_cmap(name, n)

def main():
	np.random.seed(12)
	data_points_file = open(sys.argv[1],'r');
	cluter_file = open(sys.argv[2],'r');
	k = int(sys.argv[3])

	data_points = data_points_file.read().split('\n');
	clusters = cluter_file.read().split('\n');

	if clusters[len(clusters)-1] == '':
		clusters.pop()
	clusters = np.array(list(map(int,clusters)))
	cluter_file.close()



	all_points = [np.array(list(map(float,x.strip().split(' ')))) for x in data_points]
	all_points = np.array(all_points)
	data_points_file.close()

	print(clusters.shape)
	print(all_points.shape)

	# # tsne to change to 2 dimensions
	# dimensions = all_points.shape[1]
	# if dimensions > 2:
	# 	final_matrix = TSNE(n_components = 2, verbose = 1).fit_transform(all_points)
	# 	# final_matrix = PCA(n_components = 2).fit_transform(all_points)
	# else:
	# 	final_matrix = all_points

	
	colours = get_cmap(k+1)
	for i in range(k):
		points = all_points[np.where(clusters == i)]

		X = []
		Y = []
		for j in range(len(points)):
			X.append(points[j][0])
			Y.append(points[j][1])
		plt.scatter(X, Y, c = colours(i))

# outliers
	points = all_points[np.where(clusters == -1)]
	if len(points) > 0:
		X = []
		Y = []
		for j in range(len(points)):
			X.append(points[j][0])
			Y.append(points[j][1])
		plt.scatter(X, Y, c = 'black')

	plt.show()
	plt.close()

















if __name__ == '__main__':
	main()