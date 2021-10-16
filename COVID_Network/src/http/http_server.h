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
    void Init(const std::string &url,const std::string &port);  //��ʼ��
    bool Start();   //����Httpserver
    bool Close();   //�ر�

    static std::string s_web_dir;
    static std::vector<Test> tests;
//    static mg_serve_http_opts s_server_option;
//    static std::unordered_map<std::string, ReqHandler> s_handler_map; //�ص�����ӳ���
//
//    void AddHandler(const std::string &url, ReqHandler req_handler);
//    void RemoveHandler(const std::string &url);

private:
    static void OnHttpEvent(mg_connection *connection,int event_type,void *event_data,void *fn_data);
    static void HandleHttpEvent(mg_connection *connection,mg_http_message *http_req);
    static void SendHttpRsp(mg_connection *connection,int code,json rsp);

    std::string m_url;
    std::string m_port; //�˿�
    mg_mgr m_mgr;   //���ӹ�����


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
