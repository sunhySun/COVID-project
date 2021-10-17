#include "http_server.h"
#include <fstream>


HttpServer::HttpServer(){
}

HttpServer::~HttpServer(){
    Close();
}

void HttpServer::Init(const std::string &url,const std::string &port){
//    m_url=url;
    m_port = port;
    m_url=url;
//    s_server_option.enable_directory_listing = "yes";
//    s_server_option.document_root = s_web_dir.c_str();
}

bool HttpServer::Start(){
    mg_mgr_init(&m_mgr);
    std::string str = m_url+":"+m_port;
    mg_connection *connection = mg_http_listen(&m_mgr,str.c_str(),OnHttpEvent,NULL);
    if(connection == NULL)
        return false;

    printf("starting http server at prot %s\n",m_port.c_str());
    while(true){
        mg_mgr_poll(&m_mgr, 500);
    }
    return true;
}

bool HttpServer::Close(){
    mg_mgr_free(&m_mgr);
    return true;
}

void HttpServer::OnHttpEvent(mg_connection *connection,int event_type,void *event_data,void *fn_data){
    if(event_type == MG_EV_HTTP_MSG){
        struct mg_http_message *http_req = (struct mg_http_message *)event_data;
        HandleHttpEvent(connection, http_req);
    }
}

static bool route_check(mg_http_message *http_msg, char *route_prefix){
    if(mg_vcmp(&http_msg->uri, route_prefix)==0)
        return true;
    else
        return false;
}

void HttpServer::HandleHttpEvent(mg_connection *connection,mg_http_message *http_req){
    std::string req_str=std::string(http_req->message.ptr,http_req->message.len);
    printf("got request : %s\n",req_str.c_str());

    //ÆäËû
//    if(route_check(http_req,"/"))
//        return;
//    else if(route_check(http_req,"/api/hello")){
//        SendHttpRsp(connection,"welcome to httpserver");
//    }
//    else if(route_check(http_req,"/api/sum")){
//        char n1[100],n2[200];
//        double result;
//
//        mg_http_get_var(&http_req->body,"n1",n1,sizeof(n1));
//        mg_http_get_var(&http_req->body,"n2",n2,sizeof(n2));
//
//        result = strtod(n1,NULL) + strtod(n2,NULL);
//        json j;
//        j["result"]=result;
//        SendHttpRsp(connection,j);
//    }
    if(route_check(http_req,"/generate")){
        char num[10];
        mg_http_get_var(&http_req->query,"num",num,sizeof(num));
        int n = charToInt(num);
        GenerateTest(connection,n);
    }
    else if(route_check(http_req,"/getFeature")){
        char id[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        getFeature(connection,index);
    }
    else if(route_check(http_req,"/getFeatureWithoutASP")){
        char id[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        getFeatureWithoutASP(connection,index);
    }
    else if(route_check(http_req,"/getGraph")){
        char id[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        getGraph(connection,index);
    }
    else if(route_check(http_req,"/getInfo")){
        char id[10]="",id2[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        mg_http_get_var(&http_req->query,"id2",id2,sizeof(id2));
        int index = charToInt(id);
        int index2 = charToInt(id2);
        printf("%d %d\n",index,index2);
        getInfo(connection,index,index2);
    }
    else if(route_check(http_req,"/getAllInfo")){
        char id[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        getAllInfo(connection,index);
    }
    else if(route_check(http_req,"/getRandomTest")){
        char id[10],divide[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        mg_http_get_var(&http_req->query,"divide",divide,sizeof(divide));
        int index2 = charToInt(divide);
        testRandomAttack(connection,index,index2);
    }
    else if(route_check(http_req,"/getIntentialTest")){
        char id[10],divide[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        mg_http_get_var(&http_req->query,"divide",divide,sizeof(divide));
        int index2 = charToInt(divide);
        testIntentialAttack(connection,index,index2);
    }
    else if(route_check(http_req,"/randomAttackOnce")){
        char id[10],r[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        mg_http_get_var(&http_req->query,"r",r,sizeof(id));
        double rat = charToDouble(r);
        randomAttackOnce(connection,index,rat);
    }
    else if(route_check(http_req,"/intentialAttackOnce")){
        char id[10],r[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        mg_http_get_var(&http_req->query,"r",r,sizeof(id));
        double rat = charToDouble(r);
        intentialAttackOnce(connection,index,rat);
    }
    else if(route_check(http_req,"/sortEdgeByTime")){
        char id[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        sortEdgeByTime(connection,index);
    }
    else if(route_check(http_req,"/countPaperByYear")){
        char id[10],flag[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        mg_http_get_var(&http_req->query,"flag",flag,sizeof(flag));
        int index = charToInt(id);
        int t = charToInt(flag);
        bool f;
        if(t==0) f=true;
        else    f=false;
        countPaperByYear(connection,index,f);
    }
    else if(route_check(http_req,"/graphSelectNode")){
        char id[10],num[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        mg_http_get_var(&http_req->query,"num",num,sizeof(num));
        int index = charToInt(id);
        int n = charToInt(num);
//        printf("http:selctNode\n");
        graphSelectNode(connection,index,n);
    }
    else if(route_check(http_req,"/getEdge")){
        char id[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        getEdge(connection,index);
    }
     else if(route_check(http_req,"/sortDegree")){
        char id[10];
        mg_http_get_var(&http_req->query,"id",id,sizeof(id));
        int index = charToInt(id);
        sortDegree(connection,index);
    }
}

void HttpServer::SendHttpRsp(mg_connection *connection,int code,json rsp){
    mg_http_reply(connection,code,"Access-Control-Allow-Origin: * \r\nContent-Type: application/json\r\n",rsp.dump().c_str());
}

//=============================================================

int HttpServer::GenerateTest(mg_connection *connection,int num){
//    std::string path = "static/test.json";
//    std::string path="static/webofscience2021(1).json";
//    Test test(path,num);
//    int index = tests.size();
//    tests.push_back(test);
//
    json j;
//    j["id"]=index;
    SendHttpRsp(connection,200,j);
//
//    return index;
}

void HttpServer::getFeature(mg_connection *connection,int index){
    std::string path = "data/feature.json";
    std::ifstream in(path);
    if(!in.is_open()){
        printf("未成功打开文件！");
    }
    else{
        json j;
        in >> j;
        SendHttpRsp(connection,200,j);
    }

}

void HttpServer::getFeatureWithoutASP(mg_connection *connection,int index){
    if(index >= tests.size()){
        printf("getFeature：参数有误！\n");
        json j;
        j["error"] = std::string("getFeature：参数有误！\n");
        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    Feature feature = test.testWithoutASP();
    json j;
    feature.toJson(j);
    SendHttpRsp(connection,200,j);
}

void HttpServer::getGraph(mg_connection *connection,int index){
    if(index >= tests.size()){
        printf("getGraph：参数有误！\n");
        json j;
        j["error"] = std::string("getGraph：参数有误！\n");
        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    test.testGraph(j);
    SendHttpRsp(connection,200,j);
}

void HttpServer::getInfo(mg_connection *connection,int index,int id){
    if(index >= tests.size()){
        printf("getInfo：参数有误！\n");
        json j;
        j["error"] = std::string("getInfo：参数有误！\n");
        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    if(test.testInfo(j,id))
        SendHttpRsp(connection,200,j);
    else{
        printf("getInfo：Edge参数有误！\n");
        json j;
        j["error"] = std::string("getInfo：Edge参数有误！\n");
        SendHttpRsp(connection,500,j);
    }
}

void HttpServer::testRandomAttack(mg_connection *connection,int index,int divide){
    std::string path = "data/randomAttack.json";
    std::ifstream in(path);
    if(!in.is_open()){
        printf("未成功打开文件！");
    }
    else{
        json j;
        in >> j;
        SendHttpRsp(connection,200,j);
    }
}

void HttpServer::testIntentialAttack(mg_connection *connection,int index,int divide){
   std::string path = "data/intentialAttack.json";
    std::ifstream in(path);
    if(!in.is_open()){
        printf("未成功打开文件！");
    }
    else{
        json j;
        in >> j;
        SendHttpRsp(connection,200,j);
    }
}

void HttpServer::randomAttackOnce(mg_connection *connection,int index,double r){
    if(index >= tests.size()){
        printf("randomAttackOnce：参数有误！\n");
        json j;
        j["error"] = std::string("randomAttackOnce：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    int i=test.randomAttackOnce(r,true);
    json j;
    test.attackResultToJson(j,i);
    j["ratio"]=r;
    SendHttpRsp(connection,200,j);
}

void HttpServer::intentialAttackOnce(mg_connection *connection,int index,double r){
    if(index >= tests.size()){
        printf("intentialAttackOnce：参数有误！\n");
        json j;
        j["error"] = std::string("intentialAttackOnce：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    int i=test.intentialAttackOnce(r);
    json j;
    test.attackResultToJson(j,i);
    j["ratio"]=r;
    SendHttpRsp(connection,200,j);
}

void HttpServer::sortEdgeByTime(mg_connection *connection,int index){
    if(index >= tests.size()){
        printf("sortEdgeByTime：参数有误！\n");
        json j;
        j["error"] = std::string("sortEdgeByTime：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    test.sortEdgeByTime(j);
    SendHttpRsp(connection,200,j);
}

void HttpServer::countPaperByYear(mg_connection *connection,int index,bool byYear){
    if(index >= tests.size()){
        printf("countPaperByYear：参数有误！\n");
        json j;
        j["error"] = std::string("countPaperByYear：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    test.countPaperNum(j,byYear);
    SendHttpRsp(connection,200,j);
}

void HttpServer::getAllInfo(mg_connection *connection,int index){
    if(index >= tests.size()){
        printf("countPaperByYear：参数有误！\n");
        json j;
        j["error"] = std::string("countPaperByYear：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    test.getAllInfo(j);
    SendHttpRsp(connection,200,j);
}

void HttpServer::graphSelectNode(mg_connection *connection,int index,int num){
    if(index >= tests.size()){
        printf("countPaperByYear：参数有误！\n");
        json j;
        j["error"] = std::string("countPaperByYear：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    test.graphSelectNode(num,j);
    SendHttpRsp(connection,200,j);
}

void HttpServer::getEdge(mg_connection *connection,int index){
    if(index >= tests.size()){
        printf("countPaperByYear：参数有误！\n");
        json j;
        j["error"] = std::string("countPaperByYear：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    test.getEdge(j);
    SendHttpRsp(connection,200,j);
}

void HttpServer::sortDegree(mg_connection *connection,int index){
    if(index >= tests.size()){
        printf("countPaperByYear：参数有误！\n");
        json j;
        j["error"] = std::string("countPaperByYear：参数有误！\n");

        SendHttpRsp(connection,500,j);
    }
    Test test = tests[index];
    json j;
    test.sortDegree(j);
    SendHttpRsp(connection,200,j);
}

int HttpServer::charToInt(char p[]){
    int num=0;
    for(int i=0;p[i]!='\0';i++){
        num = num*10+p[i]-'0';
    }
    return num;
}

double HttpServer::charToDouble(char p[]){
    std::string str(p);
    std::string::size_type sizet;
    return std::stod(str,&sizet);
}

