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

vector<vector<double> > read_file(string fname, int file_size){
   //ファイルを開く
    ifstream ifs(fname);
    d.resize(file_size);

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
        }
    }
    return calc_distance(file_size);
}


int checkfile(string file_name)
{
    int len = 0;
    if (file_name == "input_0.csv")
    {
        len = 5;
    }
    else if (file_name == "input_1.csv")
    {
        len = 8;
    }
    else if (file_name == "input_2.csv")
    {
        len = 16;
    }
    else if (file_name == "input_3.csv")
    {
        len = 64;
    }
    else if (file_name == "input_4.csv")
    {
        len = 128;
    }
    else if (file_name == "input_5.csv")
    {
        len = 512;
    }
    else if (file_name == "input_6.csv")
    {
        len = 2048;
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    return len;
}

//ファイルの書き込みをする関数
int writefile(int file_size, vector<int> tour)
{
    FILE *fp;
    //ファイル名のチェック
    char *fname = check_closefile(file_size);
    printf("file : %s\n", fname);

    //ファイルを開く
    fp = fopen(fname, "w");
    if (fp == NULL)
    {
        printf("%s file not open\n", fname);
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "index\n");
    for (int i = 0; i < file_size; i++)
    {
        // printf("num : %d %d\n",i,tour[i]);
        fprintf(fp, "%d\n", tour[i]);
    }

    fclose(fp);
    return 0;
}

//書き込むファイルを決める関数
char *check_closefile(int file_size)
{
    char *closefile;
    switch (file_size)
    {
    case 5:
        closefile = "solution_yours_0.csv";
        break;
    case 8:
        closefile = "solution_yours_1.csv";
        break;
    case 16:
        closefile = "solution_yours_2.csv";
        break;
    case 64:
        closefile = "solution_yours_3.csv";
        break;
    case 128:
        closefile = "solution_yours_4.csv";
        break;
    case 512:
        closefile = "solution_yours_5.csv";
        break;
    case 2048:
        closefile = "solution_yours_6.csv";
        break;
    default:
        printf("error\n");
        exit(EXIT_FAILURE);
    }
    printf("closefile : %s\n", closefile);
    return closefile;
}

bool judgeIntersected(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy){
  double ta = (cx - dx) * (ay - cy) + (cy - dy) * (cx - ax);
  double tb = (cx - dx) * (by - cy) + (cy - dy) * (cx - bx);
  double tc = (ax - bx) * (cy - ay) + (ay - by) * (ax - cx);
  double td = (ax - bx) * (dy - ay) + (ay - by) * (ax - dx);

  return tc * td < 0 && ta * tb < 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void tsp(string fname){
   //ファイル名のチェック
    int file_size = checkfile(fname);
    printf("file_size : %d\n", file_size);

    // 距離をゲットする
    dist = read_file(fname, file_size);

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

    vector<int> moved(file_size);
    rep(i, file_size){
      moved[i] = tour[i];
    }
    Data A, B, C, D; int tmp;

    // MAX100回くらい回す
    cnt = 0; bool flag = false;
    while(cnt < 100) {
      flag = true;
      for(int i = 0; i < file_size - 1; i++){
        A = d[moved[i]];
        B = d[moved[i+1]];
        for(int j = i + 2; j < file_size - 1; j++){ 
          C = d[moved[j]];
          D = d[moved[j+1]];
          if(judgeIntersected(A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y)){
            flag = false;
            if(Distance(A, C) + Distance(B, D) < Distance(A, D) + Distance(B, C)){
              swap(moved[i+1], moved[j]);
            } else {
              swap(moved[i+1], moved[j+1]);
            }
          }
        }
      }
      if(flag) break;
      cnt++;
    }
    
    writefile(file_size, moved);
}

int main()
{
  rep(i, 7){
    string filename = "input_" + to_string(i) + ".csv";
    tsp(filename);
  }
  return 0;
}
