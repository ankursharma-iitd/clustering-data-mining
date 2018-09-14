# command line arguments
# param-1 : actual label file name
# param-2 : predicted label file name

from sklearn.metrics.cluster import v_measure_score
from sklearn.metrics.cluster import completeness_score
from sklearn.metrics.cluster import homogeneity_score
import numpy as np
import sys

actual_labels = str(sys.argv[1])
my_labels = str(sys.argv[2])

true_labels = np.loadtxt(actual_labels)
predicted_labels = np.loadtxt(my_labels)

print('COMPLETENESS SCORE : ' + str(completeness_score(true_labels, predicted_labels)))
print('HOMOGENEITY SCORE : ' + str(homogeneity_score(true_labels, predicted_labels)))
print('V_MEASURE SCORE : ' + str(v_measure_score(true_labels, predicted_labels)))