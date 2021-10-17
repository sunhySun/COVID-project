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
//    std::vector<std::vector<Edge> > M; //�ڽӱ�

    int num_node;
    int num_edge;

    std::unordered_map<std::string,int> nodeNameMap;

    std::vector<std::unordered_set<int> > connect;      //�������رߣ���¼����ڵ�֮��ı�

    int maxCompNum; //�����ͨ��ͼ�Ľ����

    //===============================================

    Info jsonToInfo(json j,int id);     //��json��ʽ������ת��Ϊstruct Info
    int getNodeId(std::string name); //��ȡnode��id����������ھ����

    std::vector<int> dijstra(int s);    //����dijstra�㷨��������sΪ��㵽�����ڵ�����·

    void getConnect();

    int ConnectedCompDFS(int s,std::vector<bool> &vis);   //������ͨ����������

    Graph attackGraph(std::unordered_set<int> nodeId);

//    void getM();    //����E���M

public:
    Graph();

    Graph(const Graph &graph);

    Graph& operator = (const Graph &graph);

    void ReadData(std::string path,int num=-1);    //��json�е����ݶ�ȡ��infos��
    void generateGraph();   //������������ͼ

    std::vector<std::pair<int,int> > nodeDegreeDistribution(std::vector<int> &nodeDegree);      //����degreedistribution����������С���󷵻أ�

    double averageSP();     //����ƽ�����·��dist=num_node*2��˵�����㲻��ͨ

    double clusteringCoefficient();     //����ۼ�ϵ�����������رߵ������

    int coreness(std::vector<int> &vec);     //�������

    int maximalConnectedSubgraph(); //�������ͨ��ͼ�����

    AttackResult randomAttack(double r,Graph &attackedGraph);    //�������0<r<1�Ľ��

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
    //���������Ϊά��ͳ���������������������json��

    void graphShowSelect(int num,json &j);
    //��ͼ��ɸѡ��������ߵĵ㣬����name��Ϣ

    void getEdge(json &j);

};


