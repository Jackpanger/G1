Greedy algorithm



Claim if optimized soluition uses k sets, greedy uses at most k ln N sets

Proof $s_{i1},s_{i2},\cdots,s_{ik}$ optimized solution. One of those sets has  size >= n/k

Step 1 if greedy chooses a set of size >=N/k 

End of setp 1, <=n(1-1/k) uncovered items

Step1:one of s_{i1},s_{i2},\cdots,s_{ik} covers >=n1/k uncovered items  so greedy covers at most n1/k items 

end of step2, # of uncovered.items <=n1(1-1/k)<=n(1-1/k)^2

Iterate at end of step2, at most n(1-1/k)^i uncovered items

(1-1/k)^i<=1/e

$(1-1/k)^{k\log n}<(1/e)^{\ln n}=1/n$

at end of step $k\ln n$ # uncovered items <= $n(1-1/n)^{k\ln n}<n\cdot1/n=1$

spinning tree: tree that joins all vertices in G

$wt(T)=\sum_{e\in T}wt(e)$

Minimum spanning tree(MST) - least wt spanning tree

 Cut property

$S\subseteq V $