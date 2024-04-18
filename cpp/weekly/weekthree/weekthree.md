# Week 3 4/15

## Leetcode Study:
- Review Weekly Contest 393 & Biweekly Contest 128 

- Review Monotonic Stack qs:
1. https://leetcode.com/problems/maximal-rectangle
2. https://leetcode.com/problems/largest-rectangle-in-histogram/description/
3. 1-3 more qs

- Review ~ 3-5 Djikstra questions until comfortable with manual impl

- Review Subarray questions (trace it out on paper etc until we understand): 
1. 442. Find All Duplicates in an Array
2. 41. First Missing Positive
3. 713. Subarray Product Less Than K
4. 2958. Length of Longest Subarray With at Most K Frequency
5. 2962. Count Subarrays Where Max Element Appears at Least K Times
6. 992. Subarrays with K Different Integers
7. 2444. Count Subarrays With Fixed Bounds

## OSTEP:
- Homework/Problems for Chapters 5-7

## Monotonic Stack

## Djikstra
### 2146. K Highest Ranked Items Within a Price Range
```
2146. K Highest Ranked Items Within a Price Range
Solved
Medium
Topics
Companies
Hint

You are given a 0-indexed 2D integer array grid of size m x n that represents a map of the items in a shop. The integers in the grid represent the following:

    0 represents a wall that you cannot pass through.
    1 represents an empty cell that you can freely move to and from.
    All other positive integers represent the price of an item in that cell. You may also freely move to and from these item cells.

It takes 1 step to travel between adjacent grid cells.

You are also given integer arrays pricing and start where pricing = [low, high] and start = [row, col] indicates that you start at the position (row, col) and are interested only in items with a price in the range of [low, high] (inclusive). You are further given an integer k.

You are interested in the positions of the k highest-ranked items whose prices are within the given price range. The rank is determined by the first of these criteria that is different:

    Distance, defined as the length of the shortest path from the start (shorter distance has a higher rank).
    Price (lower price has a higher rank, but it must be in the price range).
    The row number (smaller row number has a higher rank).
    The column number (smaller column number has a higher rank).

Return the k highest-ranked items within the price range sorted by their rank (highest to lowest). If there are fewer than k reachable items within the price range, return all of them.
```
```py
class Solution:
    def highestRankedKItems(self, grid: List[List[int]], pricing: List[int], start: List[int], k: int) -> List[List[int]]:
        q = [(0, grid[start[0]][start[1]], start[0], start[1])]
        highest = []
        directions = [(1,0),(-1,0),(0,-1),(0,1)]
        visited = set()
        visited.add((start[0], start[1]))
        rows, cols = len(grid), len(grid[0])
        while q and len(highest) < k:
            d, p, r, c = heappop(q)
            if pricing[0] <= p <= pricing[1]:
                highest.append([r, c])
            for x, y in directions:
                new_r, new_c = r + x, c + y
                if 0 <= new_r < rows and 0 <= new_c < cols and grid[new_r][new_c] != 0 and (new_r, new_c) not in visited:                    
                    heappush(q, (d + 1, grid[new_r][new_c], new_r, new_c))
                    visited.add((new_r, new_c))
        return highest
```
```
1928. Minimum Cost to Reach Destination in Time
Solved
Hard
Topics
Companies
Hint

There is a country of n cities numbered from 0 to n - 1 where all the cities are connected by bi-directional roads. The roads are represented as a 2D integer array edges where edges[i] = [xi, yi, timei] denotes a road between cities xi and yi that takes timei minutes to travel. There may be multiple roads of differing travel times connecting the same two cities, but no road connects a city to itself.

Each time you pass through a city, you must pay a passing fee. This is represented as a 0-indexed integer array passingFees of length n where passingFees[j] is the amount of dollars you must pay when you pass through city j.

In the beginning, you are at city 0 and want to reach city n - 1 in maxTime minutes or less. The cost of your journey is the summation of passing fees for each city that you passed through at some moment of your journey (including the source and destination cities).

Given maxTime, edges, and passingFees, return the minimum cost to complete your journey, or -1 if you cannot complete it within maxTime minutes.

 

Example 1:

Input: maxTime = 30, edges = [[0,1,10],[1,2,10],[2,5,10],[0,3,1],[3,4,10],[4,5,15]], passingFees = [5,1,2,20,20,3]
Output: 11
Explanation: The path to take is 0 -> 1 -> 2 -> 5, which takes 30 minutes and has $11 worth of passing fees.

```
```python
class Solution:
    def minCost(self, maxTime: int, edges: List[List[int]], passingFees: List[int]) -> int:
        """
            visit our graph in sorted order of time using djikstra
            maintain a list storing the min value we reached ith node with
        """

        graph = defaultdict(list)
        for start, end, time in edges:
            graph[start].append((end, time))
            graph[end].append((start, time))
        heap = [(0, passingFees[0], 0)] # time, passing fee, node
        fees = [float("inf") for i in range(len(passingFees))]

        while heap:
            cur_time, cur_fee, cur_node = heappop(heap)
            if cur_fee >= fees[cur_node]:
                continue
            fees[cur_node] = cur_fee
            for nei, nei_time in graph[cur_node]:
                if nei_time + cur_time <= maxTime:
                    heappush(heap, (nei_time + cur_time, cur_fee + passingFees[nei], nei))
        return fees[-1] if fees[-1] != float("inf") else -1
```
In this problem I learned that some djikstra problems can be viewed instead as what order do we wish to view our graph in? do we want to view in sorted time? how do we use the problems criteria with djikstra? 
in this case I needed to view it in sorted time to ensure we stay under maxTime 100% of the time. The other criteria is finding the min cost we visited that node at which can be accomplished by using a list that stores the fee a node was reached with

```
2247. Maximum Cost of Trip With K Highways
Solved
Hard
Topics
Hint

A series of highways connect n cities numbered from 0 to n - 1. You are given a 2D integer array highways where highways[i] = [city1i, city2i, tolli] indicates that there is a highway that connects city1i and city2i, allowing a car to go from city1i to city2i and vice versa for a cost of tolli.

You are also given an integer k. You are going on a trip that crosses exactly k highways. You may start at any city, but you may only visit each city at most once during your trip.

Return the maximum cost of your trip. If there is no trip that meets the requirements, return -1.

 

Example 1:

Input: n = 5, highways = [[0,1,4],[2,1,3],[1,4,11],[3,2,3],[3,4,2]], k = 3
Output: 17
Explanation:
One possible trip is to go from 0 -> 1 -> 4 -> 3. The cost of this trip is 4 + 11 + 2 = 17.
Another possible trip is to go from 4 -> 1 -> 2 -> 3. The cost of this trip is 11 + 3 + 3 = 17.
It can be proven that 17 is the maximum possible cost of any valid trip.

Note that the trip 4 -> 1 -> 0 -> 1 is not allowed because you visit the city 1 twice.

Example 2:
```
```python
class Solution:
    def maximumCost(self, n: int, highways: List[List[int]], k: int) -> int:
        graph = defaultdict(list)
        max_cost = -1 
        for city1, city2, toll in highways:
            graph[city1].append((city2, toll))
            graph[city2].append((city1, toll))

        def traverse(node, k):
            visited = set()
            heap = [(0, node)]
            max_val = -1
            while heap:
                cur_toll, cur_node = heappop(heap)
                cur_toll = -cur_toll
                if cur_node in visited:
                    continue
                if k == 0:
                    max_val = max(max_val, cur_toll)
                    return max_val
                visited.add(cur_node)
                for nei, nei_toll in graph[cur_node]:
                    if nei not in visited:
                        heappush(heap, (-(cur_toll + nei_toll), nei))
                k -= 1
            return -1
        for i in range(n):
            max_cost = max(max_cost, traverse(i, k))
        return max_cost
```
Little tricky with the negations and the maxes but the main idea is trying starting from every city and trying to find the max trip possible while staying under k visits

```
743. Network Delay Time
Solved
Medium
Topics
Companies
Hint

You are given a network of n nodes, labeled from 1 to n. You are also given times, a list of travel times as directed edges times[i] = (ui, vi, wi), where ui is the source node, vi is the target node, and wi is the time it takes for a signal to travel from source to target.

We will send a signal from a given node k. Return the minimum time it takes for all the n nodes to receive the signal. If it is impossible for all the n nodes to receive the signal, return -1.

 
```

```PY
class Solution:
    def networkDelayTime(self, times: List[List[int]], n: int, k: int) -> int:
        
        graph = defaultdict(list)
        for u, v, t in times:
            graph[u].append((v, t))
        
        heap = [(0, k)]
        visited = set()
        time = 0
        while heap:
            cur_time, cur_node = heappop(heap)
            if cur_node in visited:
                continue
            visited.add(cur_node)
            time = cur_time
            for nei, nei_time in graph[cur_node]:
                if nei not in visited:
                    heappush(heap, (cur_time + nei_time, nei))
        return time if len(visited) == n else -1
        
```
traverse via shortest times first
## Subarray