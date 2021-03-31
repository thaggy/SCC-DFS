# SCC-DFS
Strongly Connected Components by DFS

This can be reads from a .txt file<br/>
the format of the .txt file should be as follows<br/>
VERTEX# VERTEX# <br/>
VERTEX# VERTEX#<br/>
etc.<br/>
The graphs given must be written as a directed graph so if for example I had <br/>
4 5<br/>
3 2<br/>
4 would connect to 5 but 5 would not connect to four and the same for 3 and 2<br/>

This program will find the strongly connected components using DFS in O(V+E) time which is the fastest efficiency possible for SCC
