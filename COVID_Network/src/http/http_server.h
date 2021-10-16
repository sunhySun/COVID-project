#include "../../lib/mongoose.h"
#include <string>
#include <unordered_map>
#include <stdio.h>
#include "../../lib/json.hpp"
#include "../test/Test.h"

using json = nlohmann::json;

class HttpServer{
public:
    HttpServer();
    ~HttpServer();
    void Init(const std::string &url,const std::string &port);  //初始化
    bool Start();   //启动Httpserver
    bool Close();   //关闭

    static std::string s_web_dir;
    static std::vector<Test> tests;
//    static mg_serve_http_opts s_server_option;
//    static std::unordered_map<std::string, ReqHandler> s_handler_map; //回调函数映射表
//
//    void AddHandler(const std::string &url, ReqHandler req_handler);
//    void RemoveHandler(const std::string &url);

private:
    static void OnHttpEvent(mg_connection *connection,int event_type,void *event_data,void *fn_data);
    static void HandleHttpEvent(mg_connection *connection,mg_http_message *http_req);
    static void SendHttpRsp(mg_connection *connection,int code,json rsp);

    std::string m_url;
    std::string m_port; //端口
    mg_mgr m_mgr;   //连接管理器


    static int GenerateTest(mg_connection *connection,int num=-1);
    static void getGraph(mg_connection *connection,int index);
    static void getFeature(mg_connection *connection,int index);
    static void getFeatureWithoutASP(mg_connection *connection,int index);
    static void getInfo(mg_connection *connection,int index,int id);
    static void testRandomAttack(mg_connection *connection,int index,int divide);
    static void testIntentialAttack(mg_connection *connection,int index,int divide);

    static void randomAttackOnce(mg_connection *connection,int index,double r);
    static void intentialAttackOnce(mg_connection *connection,int index,double r);

    static void sortEdgeByTime(mg_connection *connection,int index);
    static void countPaperByYear(mg_connection *connection,int index,bool byYear);

    static void getAllInfo(mg_connection *connection,int index);



    static int charToInt(char p[]);
    static double charToDouble(char p[]);

};
