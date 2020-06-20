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

using namespace std;
#define print(x) cout<<(x)<<endl
#define rep(i, n) for(int i = 0; i < (int)n; i++)
const double INF = DBL_MAX;

//各点の構造体
struct Data
{
    double x; //x座標
    double y; //y座標
};

int checkfile(string);
int writefile(int, vector<int>);
char *check_closefile(int);
vector<vector<double> > read_file(string, int);

vector<Data> d;
vector<vector<double> > dist;
int max_size=2048;
int n;

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

vector<vector<double> > calc_distance(int file_size){
  //距離を配列に入れる
  vector<vector<double> > distance(file_size, vector<double> (file_size));
  for (int i = 0; i < file_size; i++)
  {
      for (int j = i; j < file_size; j++)
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
int writefile(int file_size, vector<int> tour)
{
    cout << "index\n";
    for (int i = 0; i < file_size; i++)
    {
        cout << tour[i] << "\n";
    }
    return 0;
}


vector<int> prim(){
  vector<double> mincost(n, INF);
  vector<bool> used(n, false);
  vector<int> tour;

  mincost[0] = 0;

  while(true){
    int v = -1;
    rep(u, n){
      if(!used[u]&&(v == -1 || mincost[u] < mincost[v])) v = u;
    }
    if(v == -1) break; // 全部usedになった
    used[v] = true;
    tour.push_back(v);

    rep(u, n){
      mincost[u] = min(mincost[u], dist[v][u]);
    }
  }
  return tour;

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void tsp(string fname){    

    // 距離をゲットする
    auto t = read_file(fname);
    dist = t.first;
    int file_size = t.second;
    n = file_size;
    vector<int> tour = prim();

    writefile(file_size, tour);
}

int main(int argc, char *argv[])
{
    string filename = argv[1];
    tsp(filename);
  return 0;
}
