import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt

def main():
	X,y = datasets.make_circles(n_samples=30000, shuffle=True, noise=0.047, random_state=None, factor=0.5)
	# X1,y1 = datasets.make_blobs(n_samples=70000, centers=[(40,40), (75,75)], cluster_std=[3.0,7.0], random_state=None)
	X1,y1 = datasets.make_blobs(n_samples=70000, centers=[(50,50), (90,50)], cluster_std=[5.0,6.0], random_state=None)
	plt.scatter(X1[:,0], X1[:,1], c = 'blue')
	plt.scatter(30*X[:,0], 30*X[:,1], c = 'blue')
	plt.show()
	plt.close()

	with open('new_dataset.txt','w') as fw:
		for x in X:
			fw.write(str(30*x[0]) + ' ' + str(30*x[1]) + '\n')

		for count, x in enumerate(X1):
			fw.write(str(x[0]) + ' ' + str(x[1]))
			if count != len(X1)-1:
				fw.write('\n')




if __name__ == '__main__':
	main()