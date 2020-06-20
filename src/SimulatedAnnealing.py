from simanneal import Annealer
class TravellingSalesmanProblem(Annealer):

    """Test annealer with a travelling salesman problem.
    """
    # pass extra data (the distance matrix) into the constructor
    def __init__(self, state, distance_matrix):
        self.distance_matrix = distance_matrix
        super(TravellingSalesmanProblem, self).__init__(state)  # important!

    def move(self):
        """Swaps two cities in the route."""
        # no efficiency gain, just proof of concept
        # demonstrates returning the delta energy (optional)
        initial_energy = self.energy()

        a = random.randint(0, len(self.state) - 1)
        b = random.randint(0, len(self.state) - 1)
        self.state[a], self.state[b] = self.state[b], self.state[a]

        return self.energy() - initial_energy

    def energy(self):
        """Calculates the length of the route."""
        e = 0
        for i in range(len(self.state)):
            e += self.distance_matrix[self.state[i-1]][self.state[i]]
        return e

import pandas as pd
filename = '../input_4.csv'
xyData = pd.read_csv(filename)
N = len(xyData)

import numpy as np
from scipy.spatial import distance

mat = xyData[['x', 'y']].values
dist_mat = distance.cdist(mat, mat, metric='euclidean') # ユークリッド距離

index = [str(i) for i in range(N)]
distance_matrix = {}
for i, town in enumerate(index):
    if town not in distance_matrix.keys():
        distance_matrix[town] = {}
    for j, town2 in enumerate(index):
        distance_matrix[town][town2] = dist_mat[i][j]

import random
random.shuffle(index)
tsp = TravellingSalesmanProblem(index, distance_matrix)

tsp.set_schedule(tsp.auto(minutes=1.5))
tsp.copy_strategy = "slice"
state, e = tsp.anneal()

print(state)
outfile = '../solution_simulated_namiko_' + filename[-5:]
import csv
with open(outfile, 'w') as f:
    writer = csv.writer(f)
    writer.writerow("index")
    for s in state:
      writer.writerow(s)
