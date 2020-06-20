#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

int checkfile(char *);
int writefile(int, int *);
char *check_closefile(int);

//各点の構造体
struct data
{
    double x; //x座標
    double y; //y座標
};

struct kyori
{
    int shiten;
    int syuten;
    double len;
};
int remove_data(struct kyori *, int, int);
//距離を計算する関数
double Distance(struct data n1, struct data n2)
{
    return sqrt((n1.x - n2.x) * (n1.x - n2.x) + (n1.y - n2.y) * (n1.y - n2.y));
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char *fname = argv[1];
    double x, y;
    int file_size = 0;

    int i = 0, j = 0;

    if (argc != 2)
    {
        printf("Usage : %s <FileName>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //ファイル名のチェック
    file_size = checkfile(fname);
    printf("file_size : %d\n", file_size);

    printf("debugs\n");
    struct data d[file_size];
    int tour[file_size];
    //メモリの問題??で2048のときはsegになってしまった
    int array_size = file_size * file_size;
    struct kyori dist[array_size]; //若干無駄なメモリが存在

    printf("debugs\n");
    //ファイルを開く
    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("%s file not open\n", fname);
        exit(EXIT_FAILURE);
    }
    printf("debugs\n");
    //各行の読み込み
    fscanf(fp, "%*s,%*s"); //一行目を読み飛ばす
    while (fscanf(fp, "%lf,%lf", &x, &y) != EOF)
    {
        d[i].x = x; //構造体に代入
        d[i].y = y;
        i++;
    }
    printf("debugs\n");

    for (i = 0; i < file_size; i++)
    {
        for (j = i; j < file_size; j++)
        {
            //距離を配列に入れる
            dist[i * file_size + j].len = dist[i + file_size * j].len = Distance(d[i], d[j]);
            dist[i * file_size + j].shiten = dist[i + file_size * j].syuten = i;
            dist[i * file_size + j].syuten = dist[i + file_size * j].shiten = j;
        }
    }

    int current_city = 0;                //初期の場所変えると変化する
    int unvisited_cities[file_size - 1]; //訪れていない場所のセット
    int nextcity;
    for (i = 0; i < file_size - 1; i++)
    {
        unvisited_cities[i] = i + 1; //行ってないとこを入れていく
    }

    tour[0] = current_city; //初期値をツアーに入れる

    for (i = 1; i < file_size; i++)
    {
        double tmplen = DBL_MAX; //とりあえずの値なので大きめの値ならなんでも良い
        for (j = 0; j < file_size; j++)
        { //距離の比較を行う
            if (dist[current_city * file_size + j].len < tmplen && dist[current_city * file_size + j].len > 0)
            {
                tmplen = dist[current_city * file_size + j].len;
                nextcity = j;
            }
        }
        //本当は配列を減らしたかったが、静的配列なので使った値を０に変更
        remove_data(dist, current_city, file_size);
        tour[i] = nextcity; //次に移動した場所を配列に代入
        current_city = nextcity;
    }
    for (i = 0; i < file_size; i++)
    {
        printf("%d\n", tour[i]);
        fprintf(fp, "%d\n", tour[i]);
    }
    writefile(file_size, tour);
    fclose(fp);
    return 0;
}

int checkfile(char *file_name)
{
    int len = 0;
    if (!strcmp(file_name, "input_0.csv"))
    {
        len = 5;
    }
    else if (!strcmp(file_name, "input_1.csv"))
    {
        len = 8;
    }
    else if (!strcmp(file_name, "input_2.csv"))
    {
        len = 16;
    }
    else if (!strcmp(file_name, "input_3.csv"))
    {
        len = 64;
    }
    else if (!strcmp(file_name, "input_4.csv"))
    {
        len = 128;
    }
    else if (!strcmp(file_name, "input_5.csv"))
    {
        len = 512;
    }
    else if (!strcmp(file_name, "input_6.csv"))
    {
        len = 2048;
    }
    else
    {
        perror(file_name);
        exit(EXIT_FAILURE);
    }
    return len;
}
//移動したら、配列のデータを0にする
int remove_data(struct kyori *dist, int city, int file_size)
{
    int i = 0, j = 0;
    for (i = 0; i < file_size; i++)
    {
        for (j = i; j < file_size; j++)
        {
            if (i == city || j == city)
            {
                dist[i * file_size + j].len = dist[i + file_size * j].len = 0;
            }
        }
    }
    return 0;
}
//ファイルの書き込みをする関数
int writefile(int file_size, int *tour)
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
        closefile = "output_0.csv";
        break;
    case 8:
        closefile = "output_1.csv";
        break;
    case 16:
        closefile = "output_2.csv";
        break;
    case 64:
        closefile = "output_3.csv";
        break;
    case 128:
        closefile = "output_4.csv";
        break;
    case 512:
        closefile = "output_5.csv";
        break;
    case 2048:
        closefile = "output_6.csv";
        break;
    default:
        printf("error\n");
        exit(EXIT_FAILURE);
    }
    printf("closefile : %s\n", closefile);
    return closefile;
}