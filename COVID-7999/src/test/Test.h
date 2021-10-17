#include "../graph/graph.h"
#include "feature.h"

class Test{
public:
    std::string path;
    Graph graph;

    bool featureFlag;
    Feature feature;

    /*��¼������Ĺ���
        attackMap[i][j]:i-r��j=0,intential attack
                             j>0,random attack
                        ��ʾ��attackResults��Graph�е��±�
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
    //����ͼ��������������������������ƽ�����·�����ۼ�ϵ���������ֲ��������������ͨͼ�����

    /*���Թ���:
     divide�����������
     Rs�������Ľ��ı�����
     ASPs��ƽ�����·��
     MCSRs:�����ͨͼ������ı���
     ����ֵ��AttackResult �� ���������ͼ*/
    std::vector<int> testIntentialAttack(int divide,std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs);

    std::vector<int> testRandomAttack(int divide,std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs);

    int randomAttackOnce(double r,bool flag);
    int intentialAttackOnce(double r);

    void attackResultToJson(json &j,std::vector<int> ids);
    void attackResultToJson(json &j,int id);

    //���������һЩ��ʼ�Ĺ���
//    void initAttack(std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs);

    //����ͳ�Ʋ���
    void sortEdgeByTime(json &j);

    void countPaperNum(json &j,bool byYear);

    void graphSelectNode(int num,json &j);

    void getEdge(json &j);

    void sortDegree(json &j);
};
