Your code output is : 
Process  AT      BT      CT      TAT     WT      RT
P1       0       5       8       8       3       0
P2       1       3       13      12      9       3
P3       2       1       7       5       4       4
P4       3       2       10      7       5       5
P5       4       3       14      10      7       6

Average Turnaround Time: 8.40
Average Waiting Time: 5.60
Average Response Time: 3.60
Total Idle Time: 0

Gantt Chart:
| P1  | P1  | P2  | P3  | P1  | P4  | P5  | P2  | P5  |
0     2     4     6     7     8     10     12     13     14




Correct Output is : 
Process  AT      BT      CT      TAT     WT      RT
P1       0       5       13      13       8       0
P2       1       3       12      11       8       2
P3       2       1        5       3       2       4
P4       3       2        9       6       4       7
P5       4       3       14      10      10       9



Gantt Chart:
| P1  | P2  | P3  | P1  | P4  | P5  | P2  | P1  | P5  |
0     2     4     6     7     8    10    12    13    14
