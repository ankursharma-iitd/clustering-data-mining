import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt
import random
from shapely.geometry.polygon import Polygon
from shapely.geometry.point import Point


def get_random_point_in_polygon(poly, np):
    random.seed(12)
    (minx, miny, maxx, maxy) = poly.bounds
    num_points = 0
    all_points = []
    while True:   
        p = Point(random.uniform(minx, maxx), random.uniform(miny, maxy))
        if poly.contains(p) and (p.coords not in all_points):
            num_points += 1
            all_points.append(p.coords)
        if num_points == np:
            break
    return all_points


def main():
	X,y = datasets.make_circles(n_samples=40000, shuffle=True, noise=0.04, random_state=50, factor=0.5)
	X = X*30

	p1 = Polygon([(40, 40), (70, 70), (100, 40), (70, 10)])
	p2 = Polygon([(100, 40), (130, 70), (160, 40), (130, 10)])

	point_in_poly1 = get_random_point_in_polygon(p1,40000)
	point_in_poly2 = get_random_point_in_polygon(p2,20000)

	all_points1 = []
	for point in point_in_poly1:
	    all_points1.append(np.array([point[0][0],point[0][1]]))
	all_points1 = np.array(all_points1)


	all_points2 = []
	for point in point_in_poly2:
	    all_points2.append(np.array([point[0][0],point[0][1]]))
	all_points2 = np.array(all_points2)

	plt.scatter(X[:,0], X[:,1], c = 'blue')
	plt.scatter(all_points1[:,0], all_points1[:,1], c = 'blue')
	plt.scatter(all_points2[:,0], all_points2[:,1], c = 'blue')
	plt.show()

	with open('new_dataset.txt','w') as fw:
		for x in X:
			fw.write(str(x[0]) + ' ' + str(x[1]) + '\n')

		for x in all_points1:
			fw.write(str(x[0]) + ' ' + str(x[1]) + '\n')

		for count, x in enumerate(all_points2):
			fw.write(str(x[0]) + ' ' + str(x[1]))
			if count != len(all_points2)-1:
				fw.write('\n')










if __name__ == '__main__':
	main()