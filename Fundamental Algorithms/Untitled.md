![image-20221028172950622](/Users/jack/Library/Application Support/typora-user-images/image-20221028172950622.png)

![image-20221028173037489](/Users/jack/Library/Application Support/typora-user-images/image-20221028173037489.png)

![image-20221028173214144](/Users/jack/Library/Application Support/typora-user-images/image-20221028173214144.png)

![image-20221028173225020](/Users/jack/Library/Application Support/typora-user-images/image-20221028173225020.png)

![image-20221028173236454](/Users/jack/Library/Application Support/typora-user-images/image-20221028173236454.png)

![image-20221028173730562](/Users/jack/Library/Application Support/typora-user-images/image-20221028173730562.png)

![image-20221028173842728](/Users/jack/Library/Application Support/typora-user-images/image-20221028173842728.png)

![image-20221028174256233](/Users/jack/Library/Application Support/typora-user-images/image-20221028174256233.png)

![image-20221028174336398](/Users/jack/Library/Application Support/typora-user-images/image-20221028174336398.png)

![image-20221028174357733](/Users/jack/Library/Application Support/typora-user-images/image-20221028174357733.png)

![image-20221028174434538](/Users/jack/Library/Application Support/typora-user-images/image-20221028174434538.png)

![image-20221028174511031](/Users/jack/Library/Application Support/typora-user-images/image-20221028174511031.png)

![image-20221028174534142](/Users/jack/Library/Application Support/typora-user-images/image-20221028174534142.png)

![image-20221028174716415](/Users/jack/Library/Application Support/typora-user-images/image-20221028174716415.png)

![image-20221028174746423](/Users/jack/Library/Application Support/typora-user-images/image-20221028174746423.png)

![image-20221028174752300](/Users/jack/Library/Application Support/typora-user-images/image-20221028174752300.png)

![image-20221209170755081](/Users/jack/Library/Application Support/typora-user-images/image-20221209170755081.png)

```python
schedules = []
transactions = []
transactions.append(T1).append(T2).append(T3)

## this is all possible schedules by permutation
for permutation in permutations(transactions)
	schedules.append(permutation)
	
## use Algorithm 20.1 to check.
def determine_conflict_serializable(schedule): 
  graph = construct_graph(i)
  # here use BFS to find if the graph has a circle.
  # If no circle then it's conflict serializable 
  bool = check_path()
  return bool

# if the determine_conflict_serializable returns true, it is conflict serializable 
# then we list all equivalent serial schedules.
def equivalentSerialSchedules(schedule)
	# the connected nodes can have a undirected edge
  # for example, 3 nodes (v1,v2,v3), 2 directed edges(v1,v2),(v2,v3). v1 and v3 don't have edges, then they will have a undirected edge in the graph
	graph = construct_graph(i)
  # The method find all the paths covering all the nodes
  # Finding the start point can be achieved by seaching the nodes who do not have incoming edges
  equivalentSerialSchedules = findPathFromStartToEnd()
  return equivalentSerialSchedules
```

$Q: \pi_{Fname,Lname,Dname} (EMPLOYEE \bowtie_{Dno=Dnumber} DEPARTMENT)$
