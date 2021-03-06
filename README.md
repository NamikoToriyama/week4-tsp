# Build@Mercari 2020 Week4 - Travelling Salesman PRoblem Challenges.

This is forked from [https://github.com/hayatoito/google-step-tsp-2016](https://github.com/hayatoito/google-step-tsp-2016).

## My Record
| N                          | Simulated Annealing| Greedy(C++)|Prim       | Prim + 2opt  |GreedyPlus | Greedy+2opt| GreedyPlus+2opt  | ConvexHull |ConvexHull+2opt|
| -------------------------: | -------------------| ---------- |-----------|--------------|-----------|-------------|-----------------|------------|---------------|
|                          5 | 3291.62            | 3418.1     | 3418.1    | 3418.1       |3291.62     |3418.1      |3291.62          |3291.62     |3291.62        |
|                          8 | 3778.72            | 3832.29    | 3832.29   | 3832.29      |3832.29     |3832.29     |3832.29          |3778.72     |3778.72        |
|                         16 | 4494.42            | 5449.44    | 5461.47   | 4670.27      |5307.58     |4994.89     |4994.89          |4494.42     |4494.42        |
|                         64 | 8887.73            | 10519.16   | 12568.47  | 8500.04      |9129.75     |8605.85     |8295.73          |8407.29     |8293.03        |
|                        128 | 15025.52           | 12684.06   | 22063.91  | 11223.05     |12353.27    |11248.13    |10980.47         |12412.73    |11160.3        |
|                        512 | 67670.17           | 25331.84   | 43229.61  | 21333.77     |23392.8     |21936.5     |21342.63         |25084.64    |21381.9        |
|                       2048 | 266817.35          | 49892.05   | 109171.5  | 43858.74     |47336.79    |42885.4     |42374.47         |49980.18    |43216          |

### Simulated Annealing
I used python library and referenced this [site](https://qiita.com/maskot1977/items/38e07042a9c5f3848877).
When N = 2048, it taken about 1,2hours.
When N was 64 or more, it started giving strange results

### Greedy
Normal Greedy method.

### Prim
Minimum Spanning tree is made by prim method. 
I wanted to implement Christofides, but I thought it was a difficult task, so I only implemented prim method.

### 2opt
交差している辺をほどくようなイメージ。変更前と変更後を比較して、変更後の方が値が小さければ点の位置を変更する。

### ConvexHull
凸包と最近挿入法を利用。実装が難しかったので、こちらの[サイト](https://kanchi0914.hatenablog.com/entry/2018/06/13/181330)をほぼほぼ引用した。

### GreedyPlus
何か名前があるのかもしれないが、思いついたアイディアを実装してみた。
各点の距離を計算し、距離でsortをする。
順番に見ていき、一番長かった辺を構成する点に着目する。
その点について距離が小さい点を見ていき、一番距離の短いところに挿入する。残りの部分については再び貪欲法をかける。
見ていく辺の数によるが計算量はO(n^2~n^3)くらい。

----
1. 問題
[巡回セールスマン問題](https://ja.wikipedia.org/wiki/%E5%B7%A1%E5%9B%9E%E3%82%BB%E3%83%BC%E3%83%AB%E3%82%B9%E3%83%9E%E3%83%B3%E5%95%8F%E9%A1%8C) を解くアルゴリズムを考えてください。

2. 課題

このrepositoryを自分のgithubにforkして使ってください。
N = 5 から N = 2048までの７つの課題があります。

| Challenge    | N (= the number of cities) | Input file  | Output (Solution) file |
| ------------ | -------------------------: | ----------- | ---------------------- |
| Challenge 0  |                          5 | input_0.csv | solution_yours_0.csv   |
| Challenge 1  |                          8 | input_1.csv | solution_yours_1.csv   |
| Challenge 2  |                         16 | input_2.csv | solution_yours_2.csv   |
| Challenge 3  |                         64 | input_3.csv | solution_yours_3.csv   |
| Challenge 4  |                        128 | input_4.csv | solution_yours_4.csv   |
| Challenge 5  |                        512 | input_5.csv | solution_yours_5.csv   |
| Challenge 6  |                       2048 | input_6.csv | solution_yours_6.csv   |

inputとoutputの形式については *3. Data Format Specification* を見てください。
### Your tasks

* 巡回セールスマン問題をとくアルゴリズムを考えて実装してください。
* `solution_yours_{0-6}.csv` をあなたのアルゴリズムでといた結果で上書きしてください。
* あなたの解法の*path length*を[scoreboard]に書いてください

[scoreboard]: https://docs.google.com/spreadsheets/d/1t4ScULZ7aZpDJL8i9AVFQfqL7sErjT5i3cmC1G5ecR8/edit?usp=sharing
### An optional task (Speed challenge)

What matters in this optional task is your program's *speed* (execution time). The path length does not matter as long as it meets the condition.
Your task is:

* Given `input_6.csv`, write a program which outputs a path shorter than `47,000`

Input your program's execution time in the [scoreboard]. Faster (smaller) is better.

You can measure the execution time by `time` command.

```shellsession
$ time yourprogram input_6.csv solution_yours_6.csv
2.96s user 0.07s system 97% cpu 3.116 total
```

In this case, your score is `3.116` (s).

### Visualizer

The demo page of the visualizer is:
https://mercari-build.github.io/week4-tsp/visualizer/.

The assignment includes a helper Web page, `visualizer/index.html`, which
visualizes your solutions. You need to run a HTTP server on your local machine
to access the visualizer. Any HTTP server is okay. If you are not sure how to
run a web server, use the following command to run the HTTP server included in
the assignment. Make sure that you are in the top directory of the assignment
before running the command.

``` shellsession
./nocache_server.py # For Python 3
./nocache_server.py2.py # If you don’t want to install Python3
```

Then, open a browser and navigate to the
[http://localhost:8000/visualizer/](http://localhost:8000/visualizer/). Note
that visualizer was only tested by Google Chrome.  Using the visualizer is
up-to you. You don’t have to use the visualizer to finish the assignment. The
visualizer is provided for the purpose of helping you understand the problem.

3. Data Format Specification
----

### Input Format

The input consists of `N + 1` lines. The first line is always `x,y`. It is followed by `N` lines, each line represents an i-th city’s location, point `xi,yi` where `xi`, `yi` is a floating point number.

```
x,y
x_0,y_0
x_1,y_1
…
x_N-1,y_N-1
```

### Output Format

Output has `N + 1` lines. The first line should be “index”. It is followed by `N` lines, each line is the index of city, which represents the visitation order.

```
index
v_0
v_1
v_2
…
v_N-1
```

### Example (Challenge 0, N = 5)

Input Example:

```
x,y
214.98279057984195,762.6903632435094
1222.0393903625825,229.56212316547953
792.6961393471055,404.5419583098643
1042.5487563564207,709.8510160219619
150.17533883877582,25.512728869805677
```

Output (Solution) Example:

```
index
0
2
3
1
4
```

These formats are requirements for the visualizer, which can take only properly formatted CSV files as input.

5. What’s included in the assignment
----

To help you understand the problem, there are some sample scripts / resources
in the assignment, including, but not limited to:

- `solver_random.py` - Sample stupid solver. You never lose to this stupid one.
- `solution_random_{0-6}.csv` - Sample solutions by solver_random.py.
- `solver_greedy.py` - Sample solver using the greedy algorithm. You should beat this definitely.
- `solution_greedy_{0-6}.csv` - Sample solutions by solver_greedy.py.
- `solution_sa_{0-6}.csv` - Yet another sample solutions. I expect all of you will beat this one too. The solver itself is not included intentionally.
- `solution_wakanapo_{0-6}.csv` - Sample solutions I solved when I was joined Google STEP 2016.
- `solution_yours_{0-6}.csv` - You should overwrite these files with your solution.
- `solution_verifier.py` - Try to validate your solution and print the path length.
- `input_generator.py` - Python script which was used to create input files, `input_{0-6}.csv`
- `visualizer/` - The directory for visualizer.

6. Acknowledgments
----
この課題は[私](https://github.com/wakanapo)がgoogle step 2016に参加したときにやったものです。問題のわかりやすさ、visualizerによるアルゴリズムのみやすさ、楽しさなどにおいてこれを上回る課題はないと思ったので、Build@Mercariでも採用することにしました。
