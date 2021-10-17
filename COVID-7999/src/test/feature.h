#include <stdio.h>
#include "../../lib/json.hpp"

using json = nlohmann::json;

struct Feature{
    int numNode;
    int numEdge;
    double asp;
    double C;
    std::vector<std::pair<int,int> > degree;
    std::vector<int> nodeDegree;
    int coreness;
    std::vector<int> nodeCore;
    int maxConnSub;

    void toJson(json &j){
        j = json();
        j["numNode"]=numNode;
        j["numEdge"]=numEdge;
        j["asp"]=asp;
        j["C"]=C;
        j["degree"]=json::array();
        for(int i=0;i<degree.size();i++){
            j["degree"][i]["x"]=degree[i].first;
            j["degree"][i]["y"]=degree[i].second;
        }
        j["nodeDegree"] = nodeDegree;
        j["coreness"] = coreness;
        j["nodeCore"] = nodeCore;
        j["maxConnSub"] = maxConnSub;
    }
};
