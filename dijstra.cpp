#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<math.h>
#include <chrono>
#include <time.h>
using namespace std;
typedef struct Node {
    int id;
    double x;
    double y;
    int POINum;
    int sing;
}Node;
typedef struct SL {
    int id;//节点的ID
    vector<int> index;//该节点的父亲节点所在的位置
    double dist;//父亲节点出发按照时间的前进到该点动态距离
    double sum_dist;
}SL;
#define KIND 10
vector<Node> Graph;
vector<vector<int>> RSP;
vector<SL> arr;
void Init() {
    fstream fp("D:\\编程文件\\Visual Studio\\基于层次的最短路径\\dijstra算法\\map\\one_hundred8.node");
    int id;
    double x, y;
    int POINum;
    for (int i = 0; i <= KIND; ++i) {
        vector<int> tmp;
        RSP.push_back(tmp);
    }
    while (fp >> id >> x >> y >> POINum) {
        Node node{ id,x,y,POINum,-1 };
        Graph.push_back(node);
        RSP[POINum].push_back(id);
    }
}
SL function(vector<int> seq) {
    double min = 0, tem_min = 100000;
    int i = 0, arr_len = arr.size(), delete_arr_node = -1;
    while (1) {//无限循环
        arr[i].dist = arr[i].dist - min;
        if (arr[i].dist < tem_min && arr[i].dist>0)
            tem_min = arr[i].dist;
        if (arr[i].dist == 0) {
            delete_arr_node = arr[i].id;
            Graph[arr[i].id].sing = arr[i].index.size();
            if (arr[i].index.size() == seq.size())
                return arr[i];//循环的出口
            for (int n = 0; n < RSP[seq[arr[i].index.size()]].size(); ++n) {
                if (Graph[RSP[seq[arr[i].index.size()]][n]].sing == -1 ||
                    Graph[RSP[seq[arr[i].index.size()]][n]].sing != arr[i].index.size()) {
                    SL tem;
                    tem.id = RSP[seq[arr[i].index.size()]][n];
                    tem.index = arr[i].index;
                    tem.index.push_back(arr[i].id);
                    tem.dist = sqrt(pow(Graph[RSP[seq[arr[i].index.size()]][n]].x - Graph[arr[i].id].x, 2)
                        + pow(Graph[RSP[seq[arr[i].index.size()]][n]].y - Graph[arr[i].id].y, 2));
                    tem.sum_dist = arr[i].sum_dist + tem.dist;
                    arr.push_back(tem);
                    if (tem.dist < tem_min)
                        tem_min = tem.dist;
                }
            }
        }
        if (delete_arr_node == arr[i].id) {
            arr.erase(arr.begin() + i);
            --i;
            --arr_len;
        }
        if (++i >= arr_len) {
            i = 0;
            min = tem_min;
            tem_min = 10000;
            arr_len = arr.size();
        }
    }
}

int main() {
    vector<int> data1 = { 4,2,5,6,7,3,1 };
    for (int n = 3; n <= data1.size(); n++) {
        vector<int> seq;
        for (int h = 0; h < n; h++) {
            seq.push_back(data1[h]);
        }
        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        std::chrono::duration<double> time_span{};
        t1 = std::chrono::high_resolution_clock::now();
        Init();
        Node a = { Graph.size(),1.1,2.5,0,-1 };//起点位置
        Graph.push_back(a);
        RSP[a.POINum].push_back(a.id);
        SL ori_node;
        ori_node.dist = ori_node.sum_dist = 0;
        ori_node.id = a.id;
        arr.push_back(ori_node);
        //for (int i = 0; i < RSP.size(); i++) {
        //    for (int n = 0; n < RSP[i].size(); n++) {
        //        cout << RSP[i][n] << " ";
        //    }
        //    cout << endl;
        //}
        SL result = function(seq);
        for (int i = 0; i < result.index.size(); i++) {
            cout << result.index[i] << "->";
        }
        cout << result.id << endl;

        cout << "权重为" << result.sum_dist << endl;
        t2 = std::chrono::high_resolution_clock::now();
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        cout << to_string(time_span.count()) << endl;
    }
    return 0;
}