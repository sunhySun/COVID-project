#ifndef graph_struct
#define graph_struct

#include <stdio.h>
#include <string>
#include <vector>

struct Node {
    int id;
    std::string name;
};

struct Info{
    int id;
    std::string title;
    std::string publicationTime;
    std::string articleId;
    int citeNum;
    std::vector<std::string> address;
    std::vector<int> relatedNode;
    std::vector<int> relatedEdge;

    //记录发布时间
    int year,month;

    Info(){
        year=-1;
        month=0;
    }

    void getTime(){
        if(year!=-1) return;
        std::vector<std::string> monthes;
        monthes.push_back("JAN");
        monthes.push_back("FEB");
        monthes.push_back("MAR");
        monthes.push_back("APR");
        monthes.push_back("MAY");
        monthes.push_back("JUN");
        monthes.push_back("JUL");
        monthes.push_back("AUG");
        monthes.push_back("SEP");
        monthes.push_back("OCT");
        monthes.push_back("NOV");
        monthes.push_back("DEC");

        std::vector<std::string> years;
        years.push_back("2018");
        years.push_back("2019");
        years.push_back("2020");
        years.push_back("2021");

        month=0;
        for(int i=0;i<12;i++){
            if(publicationTime.find(monthes[i]) != std::string::npos){
                month=i+1;
                break;
            }
        }

        year=0;
        for(int i=0;i<4;i++){
            if(publicationTime.find(years[i]) != std::string::npos){
                year = i+2018;
                break;
            }
        }

        if(year == 2021 && month > 10){
            printf("%s\n",publicationTime.c_str());
        }
    }
};


struct Edge{
    int id;
    int uid,vid;
    int infoId;
};

#endif // graph_struct
