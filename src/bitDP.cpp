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
const ld INF = LDBL_MAX;

int N; // 頂点の数
const int MAX_N = 16;
pair<ld, string> dp[1 << MAX_N][MAX_N];
ld d[2500][2500];
string ans;

pair<ld, string> solve(int S, int v, string s){
  s += to_string(v);
  if (dp[S][v].first >= 0){
     dp[S][v].second = s;
    return dp[S][v];
  }

  if(S == (1 << N) - 1 && v == 0){
    // 全ての頂点を訪れて戻ってきた
    dp[S][v].first = 0;
    dp[S][v].second = s;
    return dp[S][v];
  }

  ld res = INF;
  rep(u, N){
    if(!(S >> u & 1)){
      // 次にuに移動する
      int tmp = solve(S| 1 << u, u, s).first + d[v][u];
      if (tmp < res){
        s += " " + to_string(u);
        res = tmp;
      }
    }
  }
  dp[S][v].first = res;
  dp[S][v].second = s;
  return dp[S][v];
}

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

int main() {
  int E;
  ifstream ifs("../input_0.csv");

  // テキストファイルから読み込み
  string line;
  vector<pair<ld, ld> > coodinate;
  while (getline(ifs, line)) {
      vector<string> strvec = split(line, ',');
      if(strvec[0] == "x") continue;
      if (strvec.size() == 2){
        coodinate.push_back(make_pair(stold(strvec[0]), stold(strvec[1])));
      }
  }

  // 各点の距離を求める
  N = coodinate.size();
  rep(i, N){
    d[i][i] = INF;
    for(int j = i + 1; j < N; j++){
      d[i][j] = pow(coodinate[i].first - coodinate[j].first , 2) + pow(coodinate[i].second - coodinate[j].second, 2);
      d[j][i] = d[i][j];
    }
  }


  rep(i, 2500){
    rep(j, 2500){
      dp[i][j] = make_pair(-1, "");
    }
  }
  print(solve(0, 0, "").second);
}
