#include "../graph/graph.h"
#include "feature.h"

class Test{
public:
    std::string path;
    Graph graph;

    bool featureFlag;
    Feature feature;

    /*记录计算过的攻击
        attackMap[i][j]:i-r，j=0,intential attack
                             j>0,random attack
                        表示在attackResults和Graph中的下标
    */
    std::vector<AttackResult> attackResults;
    std::vector<Graph> attackedGraphs;
    std::unordered_map<double,std::unordered_map<int,int> > attackMap;


    int recordAttack(AttackResult &result,Graph &graph,bool isRandom);

    int isrecorded(double r,int isRandom);

public:
    Test(std::string p,int num=-1);

    Feature testFeatures();

    Feature testWithoutASP();

    void testGraph(json &j);

    bool testInfo(json &j,int id);

    void getAllInfo(json &j);
    //测试图中相关特征：结点数量、边数、平均最短路径、聚集系数、度数分布、核数、最大联通图结点数

    /*测试攻击:
     divide：采样间隔，
     Rs：攻击的结点的比例，
     ASPs：平均最短路径
     MCSRs:最大连通图结点数的比例
     返回值：AttackResult 和 被攻击后的图*/
    std::vector<int> testIntentialAttack(int divide,std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs);

    std::vector<int> testRandomAttack(int divide,std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs);

    int randomAttackOnce(double r,bool flag);
    int intentialAttackOnce(double r);

    void attackResultToJson(json &j,std::vector<int> ids);
    void attackResultToJson(json &j,int id);

    //对网络进行一些初始的攻击
//    void initAttack(std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs);

    //数据统计部分
    void sortEdgeByTime(json &j);

    void countPaperNum(json &j,bool byYear);

    void graphSelectNode(int num,json &j);

    void getEdge(json &j);

    void sortDegree(json &j);
};
