## Bloxorz

Implementation of a popular [online game](https://www.miniclip.com/games/bloxorz/en) designed as a console application.

The goal of the game is to get the block (##) from the start field (S) to the target field (T) without falling off the edges. To move the block around the map, use WASD keys.

The game is modeled with a connected graph, where each node of the graph represents a field in a game map. Maps are loaded from .txt files, and they can be modified during runtime by adding or removing fields from the map (i.e. adding/removing nodes and relationships in the graph).

A modified version of the Breadth-First Search (BFS) algorithm is also implemented as a part of the finding a solution feature (i.e. finding a series of right moves that lead to successful game ending). If the solution for a given map can indeed be found, a simulation can be played:
<p align="center">
	<img src="https://user-images.githubusercontent.com/61201104/126904902-2f0e89e2-756a-441d-baf4-95e5011decb3.gif" width="80%">
</p>