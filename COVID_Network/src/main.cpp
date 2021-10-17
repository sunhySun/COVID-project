#include <stdio.h>
#include <iostream>
#include <memory>
#include <fstream>
#include "http/http_server.h"
#include "../lib/json.hpp"

//mg_serve_http_opts HttpServer::s_server_option;
//std::string HttpServer::s_web_dir = "./web";
//std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;
//std::unordered_set<mg_connection *> HttpServer::s_websocket_session_set;

std::vector<Test> HttpServer::tests;

void httpConnect(){
    std::string url = "http://localhost";
	std::string port = "7999";
	auto http_server = std::shared_ptr<HttpServer>(new HttpServer);
	http_server->Init(url,port);
	http_server->Start();
}

int main(int argc, char *argv[])
{
//    std::string path="static/webofscience2021(1).json";
//    std::ofstream out;
//    out.open("result.json",std::ios::out);
//    Test test(path);
//    Feature feature = test.testFeatures();
//    json j;
//    feature.toJson(j);
//    out<<j;
//    out.close();
////    std::string path="static/new.json";
//    Graph graph;
//    graph.ReadData(path,-1);
//    graph.generateGraph();
//    graph.graphShowSelect(50);
//    graph.sortedEdgebyTime();
//    graph.show();
//    graph.averageSP();
//    graph.clusteringCoefficient();
//    graph.nodeDegreeDistribution();
//    graph.coreness();
////    std::vector<std::pair<int,int> > vec= graph.nodeDegreeDistribution();
////    for(int i=0;i<vec.size();i++){
////        printf("(%d,%d)\n",vec[i].first,vec[i].second);
////    }
////    printf("coreness:%d\n",graph.coreness());
//
//    int num=graph.maximalConnectedSubgraph();
//    printf("maximal Connected Subgraph:%d\n",num);
//
//    Graph attackedGraph1,attackedGraph2;
//
//    AttackResult result = graph.intentialAttack(0.2,attackedGraph1);
//    result.show();
//    printf("%d %d\n",attackedGraph1.getNumNode(),attackedGraph1.getNumEdge());

//    Test test("static/test.json",-1);
//    test.testFeatures();
//    std::vector<double> vec1,vec2,vec3;
//    std::vector<int> vec = test.testIntentialAttack(5,vec1,vec2,vec3);
//    std::vector<int> vec4 = test.testRandomAttack(5,vec1,vec2,vec3);
//    for(int i=0;i<vec.size();i++){
//        test.attackResults[vec[i]].show();
//    }
//    for(int i=0;i<vec4.size();i++){
//        test.attackResults[vec4[i]].show();
//    }

//    AttackResult result2 = graph.randomAttack(0.2,attackedGraph2);
//    result2.show();

    httpConnect();

	return 0;
}
