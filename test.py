


import random
import string
from subprocess import run, PIPE
import os
import subprocess

def generate_test_string():
    vertices = random.randint(1, 100)
    edges = random.randint(1, 100)
    input_string = f"{vertices}\n{edges}\n"
    
    # Generate edges
    for _ in range(edges):
        vertex1 = random.randint(0, vertices-1)
        vertex2 = random.randint(0, vertices - 1)
        input_string += f"{vertex1}\n{vertex2}\n"
    
    return input_string

from collections import defaultdict, deque


def bfs_shortest_paths(num_vertices, edges):
    # Create an adjacency list representation of the graph
    graph = defaultdict(list)
    for edge in edges:
        u, v = edge
        graph[u].append(v)
        graph[v].append(u)  # Uncomment this line for undirected graphs

    # Initialize distances matrix with all values as infinity
    distances = [[-1] * num_vertices for _ in range(num_vertices)]

    # BFS traversal from every vertex
    for source in range(num_vertices):
        queue = deque()
        visited = [False] * num_vertices

        queue.append(source)
        visited[source] = True
        distances[source][source] = 0

        while queue:
            current_vertex = queue.popleft()
            for neighbor in graph[current_vertex]:
                if not visited[neighbor]:
                    distances[source][neighbor] = distances[source][current_vertex] + 1
                    queue.append(neighbor)
                    visited[neighbor] = True

    return distances


def run(print_output):
    try:
        test_input = generate_test_string()
        read, write = os.pipe()
        os.write(write, test_input.encode('utf-8'))
        os.close(write)

        output = subprocess.check_output(['./part_III.out'], stdin=read)
        os.close(read)
        output = output.decode('utf-8')
        input_lines = test_input.split("\n")
        num_vertices = int(input_lines[0])
        num_edges = int(input_lines[1])
        edges = [(int(input_lines[i]), int(input_lines[i+1])) for i in range(2, 2 + 2*num_edges, 2)]
        output = output.strip().split("\n")
        output = output[1:]
        output = [list(map(int, line.split())) for line in output]
        # Compute shortest paths
        shortest_paths = bfs_shortest_paths(num_vertices, edges)
        if print_output:
            print("Input:", test_input)
            print("Expected:", shortest_paths)
            print("Output:", output)
        # Compare the output
        for i in range(num_vertices):
            for j in range(num_vertices):
                if output[i][j] != shortest_paths[i][j]:
                    print("Test failed")
                    print("Input:", test_input)
                    print("Expected:", shortest_paths)
                    print("Output:", output)
                    print("The error is in the distance from vertex", i, "to vertex", j)
                    print("The expected value is", shortest_paths[i][j], "but the output is", output[i][j])
                    exit(1)

    except subprocess.CalledProcessError as e:
        # Handle errors
        print("Error:", e)


# get input for number of tests
def get_number_of_tests():
    # Get the input from the user
    print("Enter the number of tests")
    return int(input())

def want_output():
    print("Do you want to see the output (if no, only errors will be printed)? (Y/N)")
    return input().upper() == "Y"


number_of_tests = get_number_of_tests()
output = want_output()

for i in range(number_of_tests):
    run(output)
