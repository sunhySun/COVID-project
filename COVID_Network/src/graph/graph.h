#include "graph_struct.h"
#include "../../lib/json.hpp"
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include "attackResult.h"

using json=nlohmann::json;


class Graph{
private:
    std::vector<Node> V;
    std::vector<Edge> E;
    std::vector<Info> infos;
//    std::vector<std::vector<Edge> > M; //邻接表

    int num_node;
    int num_edge;

    std::unordered_map<std::string,int> nodeNameMap;

    std::vector<std::unordered_set<int> > connect;      //不考虑重边，记录网络节点之间的边

    int maxCompNum; //最大联通子图的结点数

    //===============================================

    Info jsonToInfo(json j,int id);     //将json格式的数据转化为struct Info
    int getNodeId(std::string name); //获取node的id，如果不存在就添加

    std::vector<int> dijstra(int s);    //采用dijstra算法，计算以s为起点到其他节点的最短路

    void getConnect();

    int ConnectedCompDFS(int s,std::vector<bool> &vis);   //返回连通分量结点个数

    Graph attackGraph(std::unordered_set<int> nodeId);

//    void getM();    //根据E获得M

public:
    Graph();

    Graph(const Graph &graph);

    Graph& operator = (const Graph &graph);

    void ReadData(std::string path,int num=-1);    //将json中的数据读取到infos中
    void generateGraph();   //根据数据生成图

    std::vector<std::pair<int,int> > nodeDegreeDistribution(std::vector<int> &nodeDegree);      //计算degreedistribution（按度数从小到大返回）

    double averageSP();     //计算平均最短路，dist=num_node*2，说明两点不连通

    double clusteringCoefficient();     //计算聚集系数（不考虑重边的情况）

    int coreness(std::vector<int> &vec);     //计算核数

    int maximalConnectedSubgraph(); //求最大联通子图结点数

    AttackResult randomAttack(double r,Graph &attackedGraph);    //随机攻击0<r<1的结点

    AttackResult intentialAttack(double r,Graph &attackedGraph);

    void show();

    int getNumNode();

    int getNumEdge();

    void getGraph(json &j);

    bool getInfo(json &j,int id);

    void getAllInfo(json &j);

    std::vector<int> sortedEdgebyTime();

    void EdgeToJson(json &j,int index);

    void EdgeToJson(json &j,std::vector<int> indexes);

    void countPaperNum(json &j,bool byYear);
    //按照年或月为维度统计论文数量，结果返回在json中

    void graphShowSelect(int num,json &j);
    //从图中筛选出度数最高的点，返回name信息

    void getEdge(json &j);

};


