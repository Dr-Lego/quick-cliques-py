# **Quick Cliques**: Quickly compute all maximal cliques in graphs

The original intent of the C++ implementation was to provide exact reproducibility of experimental results from two papers. This is a fork providing an additional easy-to-use Python interface.

*Listing All Maximal Cliques in Large Sparse Real-World Graphs in Near-Optimal Time*,
**D. Eppstein, M. Löffler, and D. Strash**,
Journal of Experimental Algorithmics, 18 (3): 3.1, 2013,
[doi:10.1145/2543629](https://doi.org/10.1145/2543629)

and

*Listing All Maximal Cliques in Large Sparse Real-World Graphs*,
**D. Eppstein and D. Strash**
Proceedings of the 10th International Conference on Experimental Algorithms (SEA 2011), LNCS vol. 6630, pp. 403-414.
[doi:10.1007/978-3-642-20662-7_31](https://doi.org/10.1007/978-3-642-20662-7_31)
[arXiv:1103.0318](https://arxiv.org/abs/1103.0318)

## Installation

### Python Package

```bash
# Clone the repository
git clone https://github.com/dr-lego/quick-cliques-py.git
cd quick-cliques

pip install -r requirements.txt
pip install .
```

### C++ Build

```sh
$ make
```

## Python Usage

```python
import numpy as np
from quick_cliques import find_cliques_tomita, find_cliques_degeneracy

# Create a simple graph as an adjacency list
adj_list = [
    [1, 2],     # Neighbors of vertex 0
    [0, 2],     # Neighbors of vertex 1
    [0, 1, 3, 4],  # Neighbors of vertex 2
    [2, 4, 5],  # Neighbors of vertex 3
    [2, 3, 5],  # Neighbors of vertex 4
    [3, 4]      # Neighbors of vertex 5
]

# Find all maximal cliques using the Tomita algorithm
cliques = find_cliques(adj_list, "tomita")
print("Maximal cliques:", cliques)

# Or use an adjacency matrix
adj_matrix = np.array([
    [0, 1, 1, 0, 0, 0],
    [1, 0, 1, 0, 0, 0],
    [1, 1, 0, 1, 1, 0],
    [0, 0, 1, 0, 1, 1],
    [0, 0, 1, 1, 0, 1],
    [0, 0, 0, 1, 1, 0]
], dtype=bool)

# Find all maximal cliques using the degeneracy algorithm
cliques = find_cliques(adj_matrix, "degeneracy")
print("Maximal cliques:", cliques)
```

For another working example, see [`examples/example.py`](examples/example.py).

### Available Algorithms

The module provides four different algorithms for finding maximal cliques:

1. `tomita`: Uses the Tomita algorithm, efficient for small to medium-sized graphs.
2. `adjlist`: Uses an adjacency list-based algorithm.
3. `degeneracy`: Uses a degeneracy-based algorithm, often fastest for large sparse graphs.
4. `hybrid`: Uses a hybrid algorithm combining strengths of the other approaches.

## C++ Usage

```sh
$ ./bin/qc --input-file= --algorithm=
```

or

```sh
$ ./test.sh
```

to run all algorithms on all data sets in ./data directory.

## Graph Format

The C++ implementation supports two formats:

 - **The unweighted METIS format**: Which consists of

   `<# vertices> <# edges> 1`

   followed by `<# vertices>` lines of space-separated vertices,  where the `i`-th line consists of
   all neighbors of `i`. All vertices range from `1` to `<# vertices>`

 - **A custom format**: Which consists of

   `<# vertices> 2*<# edges>`

   followed by `2*<# edges>` lines of comma-separated vertices of the form

   `u,v`

   where `u` and `v` are vertices represented by integers from `0` to `<# vertices>-1`. If `u,v` is in the list, then `v,u` must be in the list also.

## Printing Cliques

Cliques can be printed in two formats:

1. One clique per line, as space separated integers.
2. The recursion tree format used by [Tomita et al. (2006)](http://dx.doi.org/10.1016/j.tcs.2006.06.015).

These can be activated by defines in the `makefile`.

## Version
2.0beta

## License

This code is released under the GNU Public License (GPL) 3.0.

## Contact

**Darren Strash** (first name DOT last name AT gmail DOT com)
**Raphael Engel** (Python Wrapper) (hello AT alpharee DOT de)
