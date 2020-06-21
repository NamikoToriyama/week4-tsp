#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#include <float.h>
#include <fstream>
#include <sstream>
#include <tuple>
#include <list>

using namespace std;
#define print(x) cout<<(x)<<endl
#define rep(i, n) for(int i = 0; i < (int)n; i++)
const double PI = acos(-1);
const double INF = DBL_MAX;

//各点の構造体
struct Data
{
    double x; //x座標
    double y; //y座標
};

vector<Data> d;
vector<vector<double> > dist;
int n;
int max_size=2048;

//距離を計算する関数
double Distance(struct Data n1, struct Data n2)
{
    return sqrt((n1.x - n2.x) * (n1.x - n2.x) + (n1.y - n2.y) * (n1.y - n2.y));
}

//　読み込み時に,で切る関数
vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

vector<vector<double> > calc_distance(int n){
  //距離を配列に入れる
  vector<vector<double> > distance(n, vector<double> (n));
  for (int i = 0; i < n; i++)
  {
      for (int j = i; j < n; j++)
      {
          
          distance[i][j] = distance[j][i] = Distance(d[i], d[j]);
      }
  }
  return distance;
}

pair<vector<vector<double> >, int> read_file(string fname){
   //ファイルを開く
    ifstream ifs(fname);
    d.resize(max_size);
    int filesize = 0;

    //各行の読み込み
    string line;
    int i = 0, j = 0;
    while (getline(ifs, line)) {
        vector<string> strvec = split(line, ',');
        if(strvec[0] == "x") continue;
        if (strvec.size() == 2){
          d[i].x = stod(strvec[0]); //構造体に代入
          d[i].y = stod(strvec[1]);
          i++;
          filesize++;
        }
    }
    return make_pair(calc_distance(filesize), filesize);
}

//ファイルの書き込みをする関数
int writefile(vector<int> tour)
{
    cout << "index\n";
    for (int i = 0; i < n; i++)
    {
        cout << tour[i] << "\n";
    }
    return 0;
}


vector<int> convex(vector<Data> data){
  int min = 0;
  // Search minimum y coordinate. If y coordinates are same, select larger x coodinate. 
  rep(i, n){
    if(data[min].y > data[i].y) min = i;
    else if (data[min].y == data[i].y && data[min].x < data[i].x) min = i;
  }

  // 反時計周りでの角度を調べる
  vector<pair<double, int> > angle(n);
  rep(i, n){
    if(i == min) angle[i] = make_pair(0, i);
    else {
      double theta = atan2(data[i].y - data[min].y, data[i].x - data[min].x);
      if (theta < 0){
        theta = (2 * PI) + theta;
      }
      angle[i] = make_pair(theta, i);
    }
  }

  // Sorted by angle
  sort(angle.begin(), angle.end());

  vector<int> cmp_x;
  cmp_x.push_back(angle[0].second);
  cmp_x.push_back(angle[1].second);
  cmp_x.push_back(angle[2].second);

  int cmp_top;
  double theta1, theta2;
  for(int i = 3; i < n; i++){
    cmp_top = cmp_x.size();
    while(true){
      theta1 = atan2(data[cmp_x[cmp_top - 1]].y - data[cmp_x[cmp_top - 2]].y, data[cmp_x[cmp_top - 1]].x - data[cmp_x[cmp_top - 2]].x);
      if (theta1 < 0) theta1 = 2 * PI + theta1;
      theta2 = atan2(data[angle[i].second].y - data[cmp_x[cmp_top - 1]].y, data[angle[i].second].x - data[cmp_x[cmp_top - 1]].x);
      if (theta2 <= 0) theta2 = 2 * PI + theta2;
      if (theta2 - theta1 < 0){
        // Delete last element.
        cmp_x.pop_back();
        cmp_top--;
      }
      else break;
    }
    cmp_x.push_back(angle[i].second);

  }
  return cmp_x;
}
    

// 追加コストを計算
double cal_cost(int i, int j, int k){
   return dist[i][k] + dist[k][j] - dist[i][j];
}

// コスト比を計算
double cal_cost_ratio(int i, int j, int k){
   return (dist[i][k] + dist[k][j]) / dist[i][j];
}
   

// 最近挿入法
vector<int> insertion(vector<int> root){

    vector<bool> ex_root(n, true);
    for(int i: root){
      ex_root[i] = false;
    }

    while(true){
      int min = 0, k;
      vector<double> costratio(root.size(), 0);
      vector<int> minNum(root.size(), 0);
      bool flag;

      rep(i, root.size()){
        flag = true;
        k = i - 1 < 0 ? root.size() - 1 : i - 1;
        rep(j, n){
          if(!ex_root[j]) continue;     
          if(flag || min > cal_cost(root[k], root[i], j)){
            min = cal_cost(root[k], root[i], j);
            minNum[i] = j;
            flag = false;
          }
        }
        costratio[i] = cal_cost_ratio(root[k],root[i], minNum[i]);
      }

      double ratiomin = INF;
      int ratiominNum = 0;
      rep(i, root.size()){
        if(ratiomin > costratio[i]){
          ratiomin = costratio[i];
          ratiominNum = i;
        }
      }
      root.insert(root.begin() + ratiominNum, minNum[ratiominNum]);
      ex_root[minNum[ratiominNum]] = false;

      if(root.size() == n) break;
      
    }
    return root;
}


vector<int> execute_2opt(vector<int> tour){
  // O(n^2*100)
  vector<vector<double> > distance = dist;
  bool flag = true;
  rep(k, 100){
    flag = true;
    // cout << k << endl;
    for(int A=0; A < n-2; A++){
      // cout << A << endl;
      for(int C=A+2; C < n; C++){
        //2opt法の計算, 関数に分けたかったが実行時間の関係で断念
        int B = (A+1)%n;
        int D = (C+1)%n;
        double d1 = distance[tour[A]][tour[C]];
        double d2 = distance[tour[B]][tour[D]];
        double d3 = distance[tour[A]][tour[B]];
        double d4 = distance[tour[C]][tour[D]];
        //頂点を交換した際のコストの変化量の計算
        if(d1 + d2 < d3 + d4){
            reverse(tour.begin() + B, tour.begin() + C);
            flag = false;
            //頂点番号の入れ替え
            int tmp=tour[B];
            tour[B]=tour[C];
            tour[C]=tmp;
        }
      }   
    }
    if(flag){
      break;
    }
       
    
  }
  return tour;
}


double get_total_distanace(vector<int> tour){
  double t_t=0.;
  rep(i, n-1){
      t_t += dist[tour[i]][tour[i+1]];
  }
  t_t += dist[tour[n-1]][tour[0]];
  return t_t;
}


vector<int> insertData(int a, vector<int> tour, vector<vector<double> > distance){
    double min_dist = INF; int min_num = 0;
    rep(j, n-1){ // 一番近いものを探す
            if(j==a || j+1==a) continue;
            if(distance[tour[j]][tour[a]] + distance[tour[j+1]][tour[a]] < min_dist){
                min_dist = distance[tour[j]][tour[a]] + distance[tour[j+1]][tour[a]];
                min_num = j;
            }
    }

    vector<int> moved;
    vector<bool> unvisited_cities(n, true); //訪れていない場所のセット
    int start = 0, current_city;
    rep(i, n) { // O(n)
        if(tour[i]==tour[a]) continue;
        unvisited_cities[tour[i]] = false;
        moved.push_back(tour[i]);
        if(i==min_num){
            moved.push_back(tour[a]);
            unvisited_cities[tour[a]] = false;
            start = i + 1;
            current_city = tour[a];
            if(i < n) {
                moved.push_back(tour[i+1]);
                unvisited_cities[tour[i+1]] = false;
                start = i + 2;
                current_city = tour[i+1];
            } 
            break;
        }
    }

    // 続きから貪欲
    int nextcity;
     while(start < n){// O(n^2)
        double tmplen = DBL_MAX; //とりあえずの値なので大きめの値ならなんでも良い
        for (int j = 0; j < n; j++)
        {
            if (distance[current_city][j] < tmplen && distance[current_city][j] > 0 && unvisited_cities[j])
            {
                tmplen = distance[current_city][j];
                nextcity = j;
            }
        }
        moved.push_back(nextcity);
        current_city = nextcity;
        unvisited_cities[nextcity] = false;   
        start++;
    }

    if(get_total_distanace(tour) < get_total_distanace(moved)) return tour;
    return moved;
}

vector<int> eliminateLongDist(vector<int> tour){
    vector<vector<double> > distance = dist;
    for(int i = n-1; i >= n-n/5; i--){//O(iの範囲次第)
        // 各点同士の距離を求める
        vector<std::tuple<double, int, int> > lenData;
        rep(i, n-1){
            lenData.emplace_back(distance[tour[i]][tour[i+1]],i, i+1);
        }
        lenData.emplace_back(distance[tour[n-1]][tour[0]],n-1, 0);
        sort(begin( lenData ),  end( lenData ));
    
        int a = get<1>(lenData[i]);
        int b = get<2>(lenData[i]);

        tour = insertData(a, tour, distance);
        tour = insertData(b, tour, distance);
    }
    return tour;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void tsp(string fname){    

    // 距離をゲットする
    auto t = read_file(fname);
    dist = t.first;
    n = t.second;
    vector<int> tour = convex(d); 
    tour = insertion(tour);
    tour = execute_2opt(tour);
    tour = eliminateLongDist(tour);
    tour = execute_2opt(tour);
    // tour = eliminateLongDist(tour);
    // tour = execute_2opt(tour);
    cout << get_total_distanace(tour) << endl;

    // writefile(tour);
}

int main(int argc, char *argv[])
{
    string filename = argv[1];
    tsp(filename);
  return 0;
}
