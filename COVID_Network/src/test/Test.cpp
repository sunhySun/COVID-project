#include "Test.h"

Test::Test(std::string p,int num){
    path=p;
    graph.ReadData(p,num);
    graph.generateGraph();

    featureFlag=false;
}

Feature Test::testFeatures(){
    if(featureFlag) return feature;
    featureFlag = true;
    std::vector<int> vec;
    std::vector<int> nodeDegree;

    feature.numNode = graph.getNumNode();
    feature.numEdge = graph.getNumEdge();
    feature.asp = graph.averageSP();
    feature.C = graph.clusteringCoefficient();
    feature.degree = graph.nodeDegreeDistribution(nodeDegree);
    feature.nodeDegree = nodeDegree;
    feature.coreness = graph.coreness(vec);
    feature.nodeCore = vec;
    feature.maxConnSub = graph.maximalConnectedSubgraph();
    return feature;
}

Feature Test::testWithoutASP(){
    if(featureFlag) return feature;
    featureFlag = true;
    std::vector<int> vec;
    std::vector<int> nodeDegree;

    feature.numNode = graph.getNumNode();
    feature.numEdge = graph.getNumEdge();
//    feature.asp = graph.averageSP();
    feature.C = graph.clusteringCoefficient();
    feature.degree = graph.nodeDegreeDistribution(nodeDegree);
    feature.nodeDegree = nodeDegree;
    feature.coreness = graph.coreness(vec);
    feature.nodeCore = vec;
    feature.maxConnSub = graph.maximalConnectedSubgraph();
    return feature;
}

std::vector<int> Test::testIntentialAttack(int divide,std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs){
    ASPs.clear();
    Rs.clear();
    MCSRs.clear();
    double b = 1.0 / divide;
    std::vector<int> attackedGraphs;
    for(int i=0;i<=divide;i++){
        double r = (b * i >= 1) ? 0.99 : b * i;
        Graph attackedGraph;
        AttackResult result = graph.intentialAttack(r,attackedGraph);
        Rs.push_back(r);
        ASPs.push_back(result.ASPLen);
        MCSRs.push_back(result.r);
        int index = recordAttack(result,attackedGraph,true);
        attackedGraphs.push_back(index);
    }
    return attackedGraphs;
}

std::vector<int> Test::testRandomAttack(int divide,std::vector<double> &Rs,std::vector<double> &ASPs,std::vector<double> &MCSRs){
    ASPs.clear();
    Rs.clear();
    MCSRs.clear();
    double b = 1.0 / divide;
    std::vector<int> attackedGraphs;
    for(int i=0;i<=divide;i++){
        double r = (b * i >= 1) ? 0.99 : b * i;
        Graph attackedGraph;
        AttackResult result = graph.randomAttack(r,attackedGraph);
        Rs.push_back(r);
        ASPs.push_back(result.ASPLen);
        MCSRs.push_back(result.r);
        int index = recordAttack(result,attackedGraph,true);
        attackedGraphs.push_back(index);
        result.id=index;
        printf("%f %d",r,index);
    }
    return attackedGraphs;
}

int Test::recordAttack(AttackResult &result,Graph &graph,bool isRandom){
    int index;
    if(isRandom){
        index = attackResults.size();
        result.id=index;
        attackResults.push_back(result);
        attackedGraphs.push_back(graph);
        double r = result.r;
        auto it1 = attackMap.find(r);
        if(it1 == attackMap.end()){
            std::unordered_map<int,int> mp;
            std::pair<int,int> p(1,index);
            mp.insert(p);
            std::pair<double,std::unordered_map<int,int> > p2(r,mp);
            attackMap.insert(p2);
        }
        else{
            std::unordered_map<int,int> mp=it1->second;
            int randomIndex = 0;
            for(auto tmp = mp.begin();tmp!=mp.end();tmp++){
                int t = tmp->first;
                randomIndex = (randomIndex > t)? randomIndex : t;
            }
            randomIndex++;
            std::pair<int,int> p(randomIndex,index);
            (it1->second).insert(p);
        }
    }
    else{
        double r = result.r;
        index = isrecorded(r,0);
        if(index==-1){
            index = attackResults.size();
            result.id=index;
            attackResults.push_back(result);
            attackedGraphs.push_back(graph);
            auto it1 = attackMap.find(r);
            if(it1 == attackMap.end()){
                std::unordered_map<int,int> mp;
                std::pair<int,int> p(0,index);
                mp.insert(p);
                std::pair<double,std::unordered_map<int,int> > p2(r,mp);
                attackMap.insert(p2);
            }
            else{
                std::unordered_map<int,int> mp=it1->second;
                std::pair<int,int> p(0,index);
                (it1->second).insert(p);
            }

        }
    }
    return index;
}

void Test::testGraph(json &j){
    graph.getGraph(j);
}

int Test::isrecorded(double r,int isRandom){
    auto it1 = attackMap.find(r);
    if(it1 != attackMap.end()){
        std::unordered_map<int,int> mp = it1->second;
        auto it2 = mp.find(isRandom);
        if(it2!=mp.end()){
            return it2->second;
        }
        else    return -1;
    }
    else    return -1;
}

bool Test::testInfo(json &j,int id){
    graph.getInfo(j,id);
}

void Test::getAllInfo(json &j){
    graph.getAllInfo(j);
}

void Test::attackResultToJson(json &j, std::vector<int> ids){
    j = json::array();
    for(int i=0;i<ids.size();i++){
        json tmp;
        attackResults[ids[i]].toJson(tmp);
        j[i]=tmp;
    }
}

void Test::attackResultToJson(json &j, int id){
    attackResults[id].toJson(j);
}

int Test::randomAttackOnce(double r,bool flag){
    if(flag){
        int index = isrecorded(r,1);
        if(index != -1){
            return index;
        }
    }
    Graph attackedGraph;
    AttackResult result = graph.randomAttack(r,attackedGraph);
    int index = recordAttack(result,attackedGraph,true);
    return index;
}

int Test::intentialAttackOnce(double r){
    printf("Intential Attack Once:");
    int index = isrecorded(r,0);
    if(index !=-1)  return index;
    Graph attackedGraph;
    AttackResult result = graph.intentialAttack(r,attackedGraph);
    index = recordAttack(result,attackedGraph,false);
    printf("%d\n",index);
    return index;
}

void Test::sortEdgeByTime(json &j){
    std::vector<int> indexes = graph.sortedEdgebyTime();
    graph.EdgeToJson(j,indexes);
}

void Test::countPaperNum(json &j,bool byYear){
    graph.countPaperNum(j,byYear);
}

void Test::graphSelectNode(int num,json &j){
//    printf("test:selectNode");
    graph.graphShowSelect(num,j);
}

void Test::getEdge(json &j){
    graph.getEdge(j);
}

void Test::sortDegree(json &j){
    graph.sortDegree(j);
}

void Test::sortCoreness(json &j){
    graph.sortCoreness(j);
}
