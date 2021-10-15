#ifndef attackResult
#define attackResult

#include <stdio.h>
#include <unordered_set>

#include "../../lib/json.hpp"
using json = nlohmann::json;

class AttackResult{
public:
    double ASPLen;    //Average Shortest Path
    int maxCompNum;    //�����������ͨ��ͼ�Ľ����
    double r;       //maxCompNum/origMaxCompNum
    std::unordered_set<int> nodes;  //�������Ľ���id
    int id;

    void show();

    bool operator < (const AttackResult &p) const{
        return r < p.r;
    }

    void toJson(json &j){
        j["ASPLen"]=ASPLen;
        j["maxCompNum"]=maxCompNum;
        j["r"]=r;
        j["id"]=id;
        j["attackedNode"]=json::array();
        int i=0;
        for(auto it = nodes.begin();it!=nodes.end();it++){
            j["attackedNode"][i]=*it;
            i++;
        }
    }
};

#endif // attackResult
