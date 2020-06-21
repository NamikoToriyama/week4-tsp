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
#define ld long double
const ld INF = LDBL_MAX;


//各点の構造体
struct Data
{
    double x; //x座標
    double y; //y座標
};

vector<Data> xy_data;
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
          
          distance[i][j] = distance[j][i] = Distance(xy_data[i], xy_data[j]);
      }
  }
  return distance;
}

pair<vector<vector<double> >, int> read_file(string fname){
   //ファイルを開く
    ifstream ifs(fname);
    xy_data.resize(max_size);
    int filesize = 0;

    //各行の読み込み
    string line;
    int i = 0, j = 0;
    while (getline(ifs, line)) {
        vector<string> strvec = split(line, ',');
        if(strvec[0] == "x") continue;
        if (strvec.size() == 2){
          xy_data[i].x = stod(strvec[0]); //構造体に代入
          xy_data[i].y = stod(strvec[1]);
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

vector<int> greedy(){
  vector<int> tour(n, 0);    
  int current_city = 0;                //初期の場所変えると変化する
  int nextcity;
  vector<bool> unvisited_cities(n, true); //訪れていない場所のセット
  unvisited_cities[current_city] = false;
  tour[0] = current_city; //初期値をツアーに入れる
  int cnt = 1;
  //距離の比較を行う
    while(cnt < n){
        double tmplen = DBL_MAX; //とりあえずの値なので大きめの値ならなんでも良い
        for (int j = 0; j < n; j++)
        {
            if (dist[current_city][j] < tmplen && dist[current_city][j] > 0 && unvisited_cities[j])
            {
                tmplen = dist[current_city][j];
                nextcity = j;
            }
        }
        tour[cnt-1] = nextcity; //次に移動した場所を配列に代入
        current_city = nextcity;
        unvisited_cities[nextcity] = false;   
        cnt++;
    }
    return tour;
}


vector<int> execute_2opt(vector<int> tour){
  // O(n^2*100)
  vector<vector<double> > distance = dist;
  bool flag = true;
  rep(k, 1000){
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
    if(flag) {
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

    vector<int> tour = greedy(); 
    // cout << get_total_distanace(tour) << endl;
    tour = eliminateLongDist(tour);
    tour = execute_2opt(tour);
    
    // cout << get_total_distanace(tour) << endl;

    writefile(tour);
}

int main(int argc, char *argv[])
{
    string filename = argv[1];
    tsp(filename);
  return 0;
}
