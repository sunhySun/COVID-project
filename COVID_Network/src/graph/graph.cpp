#include "graph.h"
#include <fstream>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <queue>

Graph::Graph(){
    num_node=0;
    num_edge=0;
    maxCompNum=0;
}

Graph::Graph(const Graph &graph){
    V=graph.V;
    E=graph.E;
    infos=graph.infos;
//    M=graph.M;
    num_node=graph.num_node;
    num_edge=graph.num_edge;
    nodeNameMap=graph.nodeNameMap;
    connect =graph.connect;
    maxCompNum=graph.maxCompNum;
}

Graph& Graph::operator = (const Graph &graph){
    if(this!=&graph){
        V=graph.V;
        E=graph.E;
        infos=graph.infos;
//        M=graph.M;
        num_node=graph.num_node;
        num_edge=graph.num_edge;
        nodeNameMap=graph.nodeNameMap;
        connect =graph.connect;
        maxCompNum=graph.maxCompNum;
    }
    return *this;
}

Info Graph::jsonToInfo(json j,int id){
    Info info;
    info.id = id;
    j["address"].get_to(info.address);
    j["article_id"].get_to(info.articleId);
    j["title"].get_to(info.title);
    j["publication_time"].get_to(info.publicationTime);
    return info;
}

void Graph::ReadData(std::string path,int num){
    std::ifstream in(path);
    if(!in.is_open()){
        printf("未成功打开文件！");
    }
    else{
        json j;
        in>>j;
        int cnt=0;
        for(json::iterator it = j.begin();it!=j.end();it++){
            json tmp=*it;
            Info info = jsonToInfo(tmp,infos.size());
            infos.push_back(info);
            cnt++;
            if(num>0){
                if(cnt == num)  break;
            }
        }
        printf("成功读取 %d 条记录\n",infos.size());
    }
    in.close();
}

int Graph::getNodeId(std::string name){
    auto it = nodeNameMap.find(name);
    if(it != nodeNameMap.end())
        return it->second;
    else{
        int id=V.size();
        Node node;
        node.id=id;
        node.name=name;
        V.push_back(node);
        std::pair<std::string,int> pairNode(name,id);
        nodeNameMap.insert(pairNode);
        return id;
    }

}

void Graph::generateGraph(){
    clock_t start = clock();
    num_edge = 0;
    std::vector<Info> :: iterator it;
    int cnt=0;
    for(it=infos.begin();it!=infos.end();it++,cnt++){
        std::vector<std::string> address = it->address;
        int len = address.size();
        for(int i=0;i<len;i++){
            int id = getNodeId(address[i]);
            it->relatedNode.push_back(id);
        }
        //创建边
        for(int i=0;i<len;i++){
            for(int j=i+1;j<len;j++){
                int uid= it->relatedNode[i];
                int vid= it->relatedNode[j];
                Edge e1;
                e1.id=num_edge++;
                e1.uid = uid;
                e1.vid =vid;
                e1.infoId=cnt;
                E.push_back(e1);
                it->relatedEdge.push_back(e1.id);
//                printf("push_back:%d %d\n",it->id,e1.id);
            }
        }
    }
//    getM();
    num_node = V.size();
    clock_t finish = clock();
    double time = (double)(finish-start)/1000;
    printf("网络构建完成，共有 %d 个节点， %d 条边，耗时 %f 秒\n",num_node,num_edge,time);
}

void Graph::show(){
    for(int i=0;i<(int)V.size();i++){
        printf("id: %d,name:%s\n",V[i].id,V[i].name.c_str());
    }
    for(int i=0;i<(int)E.size();i++){
        printf("uid:%d,vid:%d,info:%s\n",E[i].uid,E[i].vid,infos[E[i].infoId].title.c_str());
    }
//    for(int i=0;i<M.size();i++){
//        printf("%d: ",i);
//        for(int j=0;j<M[i].size();j++){
//            printf("(%d,%d), ",M[i][j]->uid,M[i][j]->vid);
//        }
//        printf("\n");
//    }
}

bool cmp(const std::pair<int,int> &p1,const std::pair<int,int> &p2){
    return p1.first<p2.first;
}

std::vector<std::pair<int,int> > Graph::nodeDegreeDistribution(std::vector<int> &nodeDegree){
    clock_t start=clock();
    std::unordered_map<int,int>  dist;
    getConnect();
    nodeDegree.clear();
    for(int i=0;i<num_node;i++){
        int degree = connect[i].size();
        nodeDegree.push_back(degree);
        std::unordered_map<int,int> ::iterator it = dist.find(degree);
        if(it!=dist.end()){
            it->second++;
        }
        else{
            std::pair<int,int> p(degree,1);
            dist.insert(p);
        }
    }
    std::vector<std::pair<int,int> > result;
    for(auto it = dist.begin();it!=dist.end();it++){
        std::pair<int,int> p(it->first,it->second);
        result.push_back(p);
    }
    sort(result.begin(),result.end(),cmp);
    clock_t finish = clock();
    double time = (double)(finish-start)/1000;
    printf("\n计算degree 分布 耗时 %f 秒！\n",time);
    return result;
}

struct dijNode{
    int id;
    int dis;
    bool operator < (const dijNode &p)const{
        return dis < p.dis;
    }
};
std::priority_queue<dijNode> dijQ;

std::vector<int> Graph::dijstra(int s){
    int inf = num_node *2;
    std::vector<bool> flag(num_node,false);
    std::vector<int> dis(num_node,inf);
    dijNode node;
    dis[s]=0;
    node.id=s;node.dis=0;
    dijQ.push(node);
    while(!dijQ.empty()){
        node = dijQ.top();
        dijQ.pop();
        int u=node.id;
        int mi=node.dis;
        flag[u]=true;
        for(auto it=connect[u].begin();it!=connect[u].end();it++){
//            printf("%d,%d\n",u,*it);
            int v = *it;
            if(flag[v]) continue;
            if(mi+1 < dis[v]){
                dis[v]=mi+1;
                node.dis = dis[v];
                node.id = v;
                dijQ.push(node);
            }

        }
    }

//    for(int i=0;i<num_node-1;i++){
//        int u=-1,mi=inf;
//        for(int j=0;j<num_node;j++){
//            if(flag[j]) continue;
//            if(dist[j]<mi){
//                u=j;
//                mi=dist[j];
//            }
//        }
////        printf("%d\n",u);
//        if(u==-1)    break;
//        flag[u]=true;
//        for(auto it=connect[u].begin();it!=connect[u].end();it++){
////            printf("%d,%d\n",u,*it);
//            int v = *it;
//            if(flag[v]) continue;
//            if(mi+1 < dist[v])
//                dist[v]=mi+1;
//        }
//    }
    return dis;
}


double Graph::averageSP(){
    printf("开始计算平均最短路");
    clock_t start=clock();
    double sum=0;
    getConnect();
    int b =num_node/10;
    if(b==0)    b=1;
    for(int i=0;i<num_node;i++){
        std::vector<int> dist = dijstra(i);
        for(int j=0;j<num_node;j++){
            sum+=dist[j];
//            printf("(%d,%d,%d)\n",i,j,dist[j]);
        }
        if(i%b==0)
            printf(".");
    }
    clock_t finish = clock();
    double time = (double)(finish-start)/1000;
    double result=sum/num_node/(num_node-1);
    FILE *f = fopen("time.txt","w");
    fprintf(f,"\n平均最短路为 %f ,耗时 %f 秒！\n",result,time);
    return result;
}

void Graph::getConnect(){
    if(connect.size()>0)    return ;
    for(int i=0;i<num_node;i++){
        std::unordered_set<int> st;
        connect.push_back(st);
    }
    for(int i=0;i<num_edge;i++){
        int uid = E[i].uid;
        int vid = E[i].vid;
        connect[uid].insert(vid);
        connect[vid].insert(uid);
    }
}

double Graph::clusteringCoefficient(){
    printf("开始计算聚集系数");
    clock_t start=clock();
    getConnect();
    double C=0;
    int b =num_node/10;
    if(b==0)    b=1;
    for(int i=0;i<num_node;i++){
        std::unordered_set<int> st=connect[i];
        int e=0;
        int k=0;
        for(auto it=st.begin();it!=st.end();it++){
            for(auto it1=it;it1!=st.end();it1++){
                if(*it == *it1) continue;
                int u=*it,v=*it1;
                if(connect[u].find(v)!=connect[u].end()){
                    e++;
                }
            }
            k++;
        }
        if(k<=1)    continue;
        double c=2.0 * e / (k*(k-1));
        C+=c;
        if(i%b==0)  printf(".");
    }
    C/=num_node;
    clock_t finish = clock();
    double time = (double)(finish-start)/1000;
    printf("\n聚集系数为 %f ,耗时 %f 秒！\n",C,time);
    return C;
}

int Graph::coreness(std::vector<int> &vec){
    printf("开始计算核数");
    clock_t start=clock();
    getConnect();
    std::vector<std::unordered_set<int> > tmpConnect=connect;

    std::vector<int> degree;
    for(int i=0;i<num_node;i++){
        degree.push_back(connect[i].size());
        vec.push_back(-1);
    }
    int b =num_node/10;
    if(b==0)    b=1;
    int num=num_node;
    int core=0;
    while(num>0){
        bool flag = true;  //记录有无节点被删除
        while(flag){
            flag=false;
            for(int i=0;i<num_node;i++){
                if(degree[i] ==-1)  continue;
                if(degree[i]<=core){
                    flag=true;
                    for(auto it=tmpConnect[i].begin();it!=tmpConnect[i].end();it++){
                        int u=*it;
                        degree[u]--;
                        tmpConnect[u].erase(i);
                    }
                    tmpConnect[i].clear();
                    degree[i]=-1;
                    num--;
//                    printf("%d %d %d\n",core,i,num);
//                    char c;
//                    scanf("%c",&c);
                    vec[i]=core;
                }
            }
        }
        core++;
        if(num%b==0)    printf(".");
    }
    for(int i=0;i<num_node;i++){
        if(vec[i]==-1)
            vec[i]=core-1;
    }
    clock_t finish = clock();
    double time = (double)(finish-start)/1000;
    printf("\n核数（coreness）为 %d ,耗时 %f 秒！\n",core-1,time);
    return core-1;
}

int Graph::maximalConnectedSubgraph(){
    clock_t start = clock();
    printf("开始计算最大联通子图");
    int b =num_node/10;
    if(b==0)    b=1;
    if(maxCompNum!=0)   return maxCompNum;
    std::vector<bool> vis(num_node,false);
    getConnect();
    int ma=0;
    for(int i=0;i<num_node;i++){
        if(!vis[i]){
            int tmp = ConnectedCompDFS(i,vis);
            ma=(tmp>ma)?tmp:ma;
        }
        if(i%b==0)  printf(".");
    }
    clock_t finish = clock();
    double time = (double)(finish-start)/1000;
    printf("\n最大连通子图为 %d ,耗时 %f 秒！\n",ma,time);
    return ma;
}

int Graph::ConnectedCompDFS(int s,std::vector<bool> &vis){
//    printf("%d\n",s);
    vis[s]=1;
    int ans=1;
    for(auto it=connect[s].begin();it!=connect[s].end();it++){
        int u = *it;
        if(!vis[u])
            ans+=ConnectedCompDFS(u,vis);
    }
    return ans;
}

AttackResult Graph::randomAttack(double r,Graph &attackedGraph){
    AttackResult result;
    if(r>=1 || r<0){
        printf("随机攻击参数有误\n");
        return result;
    }
    int num = num_node * r;
    std::unordered_set<int> nodeId;
    srand(time(NULL));
    for(int i=0;i<num;i++){
        int t = rand()%num_node;
        while(nodeId.find(t)!=nodeId.end()){
            t=rand()%num_node;
        }
        nodeId.insert(t);
    }
    if(nodeId.size()==0)    attackedGraph = *this;
    else
        attackedGraph = attackGraph(nodeId);

    result.ASPLen = attackedGraph.averageSP();
    result.maxCompNum = attackedGraph.maximalConnectedSubgraph();
    result.r = 1.0 * result.maxCompNum / (maximalConnectedSubgraph());
    result.nodes = nodeId;
    return result;
}

Graph Graph::attackGraph(std::unordered_set<int> nodeId){
    Graph graph;
    graph.V=V;
    graph.num_node = num_node;
    std::vector<Edge> attackE;
    for(auto it=E.begin();it!=E.end();it++){
        Edge e =*it;
        if((nodeId.find(e.uid) != nodeId.end())||(nodeId.find(e.vid) != nodeId.end()))
            continue;
        attackE.push_back(*it);
    }
    graph.E = attackE;
    graph.num_edge = attackE.size();
//    graph.getM();
    return graph;
}

//void Graph::getM(){
//    for(auto it=E.begin();it!=E.end();it++){
//        Edge e=*it;
//        int uid=e.uid,vid=e.vid;
//        while(((int)M.size()<=uid)||((int)M.size()<=vid)){
//            std::vector<Edge*> e1;
//            M.push_back(e1);
//        }
//        M[uid].push_back(*it);
//        M[vid].push_back(*it);
//    }
//}

AttackResult Graph::intentialAttack(double r,Graph &attackedGraph){
    AttackResult result;
    if(r>=1 || r<0){
        printf("刻意攻击参数有误\n");
        return result;
    }
    std::vector<int> degree(num_node,0);
    std::unordered_set<int> nodeId;
    getConnect();
    for(int i=0;i<num_node;i++){
        int t=connect[i].size();
        degree[i]=t;
//        printf("%d\n",connect[i].size());
    }
    int num=r*num_node;
    printf("%d\n",num);

    for(int i=0;i<num;i++){
        int u=-1,ma=-1;
        for(int j=0;j<num_node;j++){
            if(degree[j]==-1)   continue;
//            printf("%d %d\n",j,degree[j]);
            if(degree[j]>ma){
                ma=degree[j];
                u=j;
            }
        }
//        printf("aaa::%d %d\n",u,degree[u]);
        nodeId.insert(u);
        degree[u]=-1;
        for(auto it=connect[u].begin();it!=connect[u].end();it++){
            int v=*it;
//            printf("v:%d\n",v);
            if(degree[v]!=-1)   degree[v]--;
        }
//        printf("%d\n",u);
    }
    if(nodeId.size()==0)    attackedGraph = *this;
    else
        attackedGraph = attackGraph(nodeId);

    result.ASPLen = attackedGraph.averageSP();
    result.maxCompNum = attackedGraph.maximalConnectedSubgraph();

    result.r = 1.0 * result.maxCompNum / (maximalConnectedSubgraph());

    result.nodes = nodeId;
    return result;

}


int Graph::getNumNode(){
    return num_node;
}

int Graph::getNumEdge(){
    return num_edge;
}

void Graph::getGraph(json &j){
    j["node"]=json::array();
    for(int i=0;i<V.size();i++){
        j["node"][i]["id"]=V[i].id;
        j["node"][i]["name"]=V[i].name;
    }
    j["edge"]=json::array();
    for(int i=0;i<E.size();i++){
        j["edge"][i]["u"]=E[i].uid;
        j["edge"][i]["v"]=E[i].vid;
        j["edge"][i]["id"]=E[i].id;
        j["edge"][i]["infoId"]=E[i].infoId;
    }
}

bool Graph::getInfo(json &j,int id){
//    printf("%d %d\n",id,infos.size());
    if(id>=infos.size())    return false;
    Info info = infos[id];
//    printf("info:%d\n",info.id);
    j["id"]=info.id;
    j["title"]=info.title;
    j["publicationTime"]=info.publicationTime;
    j["articleId"]=info.articleId;
    j["citeNum"]=info.citeNum;
    j["address"]=json::array();
    for(int i=0;i<info.address.size();i++){
        j["address"][i]=info.address[i];
    }
    j["relatedNode"]=json::array();
    for(int i=0;i<info.relatedNode.size();i++){
        j["relatedNode"][i]=info.relatedNode[i];
    }
    j["relatedEdge"]=json::array();
    for(int i=0;i<info.relatedEdge.size();i++){
        j["relatedEdge"][i]=info.relatedEdge[i];
    }
    return true;
}

void Graph::getAllInfo(json &j){
    j=json::array();
    for(int i=0;i<infos.size();i++){
        json tmp;
        getInfo(tmp,i);
        j[i]=tmp;
    }
}

bool cmp1(const Info &a,const Info &b){
    if(a.year == b.year)
        return a.month <b.month;
    else
        return a.year < b.year;
}

std::vector<int> Graph::sortedEdgebyTime(){
    std::vector<Info> infoArr;
    for(int i=0;i<infos.size();i++){
        infos[i].getTime();
        infoArr.push_back(infos[i]);
    }
    sort(infoArr.begin(),infoArr.end(),cmp1);

    std::vector<int> result;
    for(int i=0;i<infoArr.size();i++){
        std::vector<int> edges = infoArr[i].relatedEdge;
        for(int j=0;j<edges.size();j++){
            result.push_back(edges[j]);
        }
    }

//    for(int i=0;i<10;i++){
//        int index = E[result[i]].infoId;
//        printf("%s\n",infos[index].publicationTime.c_str());
//    }
    return result;
}

void Graph::EdgeToJson(json &j,int i){
    j["u"]=E[i].uid;
    j["v"]=E[i].vid;
    j["id"]=E[i].id;
    j["infoId"]=E[i].infoId;
}

void Graph::EdgeToJson(json &j,std::vector<int> indexes){
    j=json::array();
    for(int i=0;i<indexes.size();i++){
        json tmp;
        int t=indexes[i];
        EdgeToJson(tmp,t);
        j[i]=tmp;
    }
}

void Graph::countPaperNum(json &j,bool byYear){
    std::unordered_map<int,int> cnt;
    for(int i=0;i<infos.size();i++){
        infos[i].getTime();
        int date=0;
        if(byYear)  date = infos[i].year;
        else
            date=infos[i].year*100+infos[i].month;
        auto it = cnt.find(date);
        if(it != cnt.end()){
            it->second++;
        }
        else{
            std::pair<int,int> p(date,1);
            cnt.insert(p);
        }
    }

    std::vector<std::pair<int,int> > vec;
    for(auto it = cnt.begin();it!=cnt.end();it++){
        std::pair<int,int> p(it->first,it->second);
        vec.push_back(p);
    }
    sort(vec.begin(),vec.end(),cmp);

    j=json::array();
    for(int i=0;i<vec.size();i++){
        j[i]["x"]=vec[i].first;
        j[i]["y"]=vec[i].second;
    }
}

struct bfsnode{
    int id;
    int degree;
    bool operator < (const  bfsnode &p) const{
        return degree < p.degree;
    }
};

void Graph::graphShowSelect(int num,json &j){
//    printf("graph:selectNode");
    getConnect();
    std::vector<bool> vis(num_node,false);
    std::priority_queue<bfsnode> q;
    int ma=0,u=0;
    for(int i=0;i<connect.size();i++){
        int t= connect[i].size();
        if(t > ma){
            ma = t;
            u = i;
        }
    }
    bfsnode node;
    node.id = u;
    node.degree = ma;
    q.push(node);
    std::unordered_set<int> mp;
    while(!q.empty() && num>0){
        node = q.top();
        q.pop();
        int u= node.id;
        mp.insert(u);
        if(vis[u])  continue;
        vis[u]=true;
        num--;
        std::unordered_set<int> connectu = connect[u];
        for(auto it = connectu.begin();it!=connectu.end();it++){
            if(vis[*it])    continue;
            node.id=*it;
            node.degree = connect[node.id].size();
            q.push(node);
        }
    }

    j=json::array();
    int cnt=0;
//    for(int i=0;i<num_edge;i++){
//        Edge e = E[i];
//        int u = e.uid,v=e.vid;
//        if((mp.find(u)!=mp.end())&&(mp.find(v)!=mp.end())){
//            j[cnt][0]=V[u].name;
//            j[cnt][1]=V[v].name;
//            cnt++;
//        }
//    }
    for(auto it1 = mp.begin();it1!=mp.end();it1++){
        for(auto it2=it1;it2!=mp.end();it2++){
            if(it1 == it2)  continue;
            if(connect[*it1].find(*it2)!=connect[*it1].end()){
                j[cnt][0]=V[*it1].name;
                j[cnt][1]=V[*it2].name;
                cnt++;
            }
        }
    }
//    printf("graph return\n");
//    printf("%d\n",cnt);


//    std::ofstream out;
//    out.open("graph-50.json",std::ios::out);
//    out<<j<<std::endl;
}

void Graph::getEdge(json &j){
    j=json::array();
    int cnt=0;
    getConnect();
    for(int i=0;i<connect.size();i++){
        for(auto it = connect[i].begin();it!=connect[i].end();it++){
            int u=*it;
            if(u<i){
                j[cnt][0]=V[i].name;
                j[cnt][1]=V[u].name;
                cnt++;
            }
        }
    }
}

bool cmpSortDegree(const std::pair<int,int> &p1,const std::pair<int,int> &p2) {
    if(p1.second == p2.second)
        return p1.first < p2.first;
    return p1.second > p2.second;
}

void Graph::sortDegree(json &j){
    getConnect();
    std::vector<std::pair<int,int> > vec;
    for(int i=0;i<num_node;i++){
        std::pair<int,int> p(i,connect[i].size());
        vec.push_back(p);
    }
    sort(vec.begin(),vec.end(),cmpSortDegree);
    j=json::array();
    for(int i=0;i<vec.size();i++){
        j[i]["id"] = vec[i].first;
        j[i]["degree"] = vec[i].second;
        j[i]["name"] = V[vec[i].first].name;
    }
}

