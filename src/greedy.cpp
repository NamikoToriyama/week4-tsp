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
#define ll long long
#define ld long double

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

vector<int> greedy(int file_size){
  vector<int> tour(file_size, 0);    
  int current_city = 0;                //初期の場所変えると変化する
  int nextcity;
  vector<bool> unvisited_cities(file_size, true); //訪れていない場所のセット
  unvisited_cities[current_city] = false;
  tour[0] = current_city; //初期値をツアーに入れる
  int cnt = 1;
  //距離の比較を行う
    while(cnt < file_size){
        double tmplen = DBL_MAX; //とりあえずの値なので大きめの値ならなんでも良い
        for (int j = 0; j < file_size; j++)
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void tsp(string fname){    

    // 距離をゲットする
    auto t = read_file(fname);
    dist = t.first;
    int file_size = t.second;

    vector<int> tour = greedy(file_size);
    writefile(file_size, tour);
}

int main(int argc, char *argv[])
{
    string filename = argv[1];
    tsp(filename);
  return 0;
}
