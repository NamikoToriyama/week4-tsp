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

vector<int> insertData(int a, vector<int> tour){

    double min_dist = INF; int min_num = 0;
    rep(j, n-1){ // 一番近いものを探す
            if(j==a || j+1==a) continue;
            if(dist[tour[j]][tour[a]] + dist[tour[j+1]][tour[a]] < min_dist){
                min_dist = dist[tour[j]][tour[a]] + dist[tour[j+1]][tour[a]];
                min_num = j;
            }
    }

    vector<int> moved;
    vector<bool> unvisited_cities(n, true); //訪れていない場所のセット
    int start = 0, current_city;
    rep(i, n) {
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
        // debug: totalの距離
        double t_t=0., m_t=0.0;
        rep(i, n-1){
            t_t += dist[tour[i]][tour[i+1]];
            m_t += dist[moved[i]][moved[i+1]];
        }
        t_t += dist[tour[n-1]][tour[0]];
        m_t += dist[moved[n-1]][moved[0]];
        if(t_t < m_t)
    }

    // 続きから貪欲
    int nextcity;
     while(start < n){
        double tmplen = DBL_MAX; //とりあえずの値なので大きめの値ならなんでも良い
        for (int j = 0; j < n; j++)
        {
            if (dist[current_city][j] < tmplen && dist[current_city][j] > 0 && unvisited_cities[j])
            {
                tmplen = dist[current_city][j];
                nextcity = j;
            }
        }
        moved.push_back(nextcity);
        current_city = nextcity;
        unvisited_cities[nextcity] = false;   
        start++;
    }

    // rep(i, n){
    //     cout << moved[i] << " ";
    // }
    // cout << endl;
    return moved;

}

vector<int> eliminateLongDist(vector<int> tour){
    
    rep(i, n-1){
        t_t += dist[tour[i]][tour[i+1]];
    }

    for(int i = n-1; i >= n-n/5; i--){
        // 各点同士の距離を求める
        vector<std::tuple<double, int, int> > lenData;
        rep(i, n-1){
            lenData.emplace_back(dist[tour[i]][tour[i+1]],i, i+1);
        }
        lenData.emplace_back(dist[tour[n-1]][tour[0]],n-1, 0);
        sort(begin( lenData ),  end( lenData ));
    
        int a = get<1>(lenData[i]);
        int b = get<2>(lenData[i]);

        tour = insertData(a, tour);
        tour = insertData(b, tour);
    }

    // debug: totalの距離
    // rep(i, n-1){
    //     m_t += dist[tour[i]][tour[i+1]];
    // }

    // cout << "tour:" << t_t << endl;
    // cout << "moved" << m_t << endl;
    
    return tour;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void tsp(string fname){    

    // 距離をゲットする
    auto t = read_file(fname);
    dist = t.first;
    n = t.second;

    vector<int> tour = greedy();
    tour = eliminateLongDist(tour);
    
    writefile(tour);
}

int main(int argc, char *argv[])
{
    string filename = argv[1];
    tsp(filename);
  return 0;
}
