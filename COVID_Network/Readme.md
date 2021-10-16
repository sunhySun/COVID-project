### 使用说明

项目codeblocks构建，运行即可通过接口访问

接口地址：`http://localhost:7999/`

修改`src/http/http_server.cpp` 中的`GenerateTest`函数可以修改读取的文件（L161左右）

​																				`HandleHttpEvent`函数可以修改读取的数据条数（L75）

```
eg.
GenerateTest(connection,10);	//第二个参数表示读取数据的数量，参数缺省表示读取全部数据
```

**注**：在测试`getFeature`的时候需要将读取的数据条数改小一点，计算最短路时耗时可能较长（1000左右时间在几分钟）

### 相关接口1.0

1. url:`/generate`

   input:/

   output:

2. url:`/getFeature`

   intput:id

   output:{"C":0.3888888888888889,"asp":7.0,"coreness":2,"degree":[{"x":1,"y":3},{"x":2,"y":2},{"x":3,"y":1}],"maxConnSub":4,"numEdge":5,"numNode":6}

   含义：输入图的id，返回相关参数

   C:聚集系数，asp：平均最短路径，coreness：核数，

   degree：度数分布，x：度数，y：数量

   maxConnSub：最大连通子图的结点数量

   numEdge，numNode：边数、结点数

   url:`/getFeatureWithoutASP` 计算除最短路外的所有信息

   nodeDegree：每个点的度数

   nodeCore：每个点的核数

3. url:`/getGraph`

   input:id

   output:

   ```json
   {"edge":[{"id":0,"infoId":0,"u":0,"v":1},{"id":1,"infoId":0,"u":0,"v":2},{"id":2,"infoId":0,"u":1,"v":2},{"id":3,"infoId":1,"u":2,"v":3},{"id":4,"infoId":2,"u":4,"v":5}],"node":[{"id":0,"name":"a"},{"id":1,"name":"b"},{"id":2,"name":"c"},{"id":3,"name":"d"},{"id":4,"name":"e"},{"id":5,"name":"f"}]}
   ```

   含义，输入图的id，返回图的结构

   node:id——结点的唯一标识，name——结点名称（机构名称）

   edge:id——边的唯一标识，u,v——边的两个端点

4. url:`/getInfo`

   eg.getInfo?id=0&id2=1

   input: id:

   ​			id1:edge的infoId

   output

   ```
   {"address":["c","d"],"articleId":"WOS:000533411000039","citeNum":2011606336,"id":1,"publicationTime":"MAY 7 2020","relatedEdge":[3],"relatedNode":[2,3],"title":"Clinical Characteristics of Covid-19 in China"}
   ```

   含义输入图的id和infoId，返回相应的文章信息

   relatedEdge：与这条信息相关的边

   relatedNode：address对应结点的id

   

   5.url `/getRandomTest`

   `/getRandomTest?id=0&divide=2`

   input: id,divide

   output

   ```
   [{"ASPLen":7.0,"attackedNode":[],"id":0,"maxCompNum":4,"r":1.0,"ratio":0.0},{"ASPLen":11.266666666666667,"attackedNode":[1,2,0],"id":1,"maxCompNum":2,"r":0.5,"ratio":0.5},{"ASPLen":12.0,"attackedNode":[3,0,2,1,4],"id":2,"maxCompNum":1,"r":0.25,"ratio":0.99}]
   ```

   含义：包含divide+1次随机攻击

   每次随机攻击记录： ASPLen：平均最短路，attackNode：被攻击的结点，maxComNum：攻击后最大连通图结点数，r:maxComNum/未攻击最大联通图结点数，ratio：攻击比例

   

   6.url `/getIntentialTest`

   刻意攻击，参数与上一个相同





7. url`/randomAttackOnce`

   input:id,

   ​			r:被攻击节点的比例（double）

   output：被攻击的结果

   ```
   {"ASPLen":874.2765332640332,"attackedNode":[90,106,180,145,258,161,54,295,88,467,238,352,240,376,361,448,39,118,108,250,29,99,175,231,63,472,160,248,209,356,163,402,185,102,213,241,111,194,393,372,92,291,396,197,148,162,273,96,276,198,397,127,326,344,174,373,116,315,354,349,150,195,394,98,86,285,280,357,322,314,218,262,165,364,73,272,170,164,363,261,51,460,471,62,351,152,60,259,76,104,303,55,149,348,69,478,80,390,196,227,134,379,323,85,320,75,64,473,236,228,34,131,330,422,223,375,176,378,143,350,151,38,447,49,353,442,44,229,428,30,239,40,336,137,433,234,35,289,386,468,269,374,296,5,365,166,100,103,437,17,211,12,305,400,109,316,219,20,22,119,244,15,142,341,56,45,454,84,343,430,153,399,1,112,311,192,359,232,404,415,6,183,2,411,66,67,476,406,8,222,23,457,252,18,7,416,412,87,423,224,434,25,101,9,388,57,83,126,171,94,358,193,459,298,0,199,266,136,141,47,31,93,367,168,113,95,477,389,190,271,470,446,425,380,307,43,452,419,10,140],"id":0,"maxCompNum":143,"r":0.34210526315789475,"ratio":0.5}
   ```

   

8. url`/intentialAttackOnce`

   同上

9. url`/sortEdgeByTime`

   input:id

   output:按照publicationTime排序后的边

   ```
   [{"id":0,"infoId":0,"u":0,"v":1},{"id":1,"infoId":0,"u":0,"v":2},{"id":2,"infoId":0,"u":1,"v":2},{"id":3,"infoId":1,"u":2,"v":3},{"id":4,"infoId":2,"u":4,"v":5}]
   ```

10. url`/countPaperByYear`

    input:id

    ​			flag:=0,by year

    ​						=1,by month

    output

    ```
    http://localhost:7999/countPaperByYear?id=0&flag=0
    [{"x":2020,"y":3}]
    
    http://localhost:7999/countPaperByYear?id=0&flag=1
    [{"x":202003,"y":1},{"x":202005,"y":2}]
    ```

    按照年份或月统计论文数量
    
11. url `/getAllInfo`

    input:id

    output:

    ```
    [{"address":["Wuhan Pulm Hosp","Jinyintan Hosp","Capital Med Univ","Tsinghua Univ","China Japan Friendship Hosp","Chinese Acad Med Sci"],"articleId":"WOS:000522650100033","citeNum":21954560,"id":0,"publicationTime":"MAR 28 2020","relatedEdge":[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14],"relatedNode":[0,1,2,3,4,5],"title":"Clinical course and risk factors for mortality of adult inpatients with COVID-19 in Wuhan, China: a retrospective cohort study"},{"address":["Hosp Moinhos Vento Porto Alegre","Univ Fed Rio Grande Porto Alegre"],"articleId":"WOS:000533411000039","citeNum":21954560,"id":1,"publicationTime":"MAY 7 2020","relatedEdge":[15],"relatedNode":[6,7],"title":"Clinical Characteristics of Covid-19 in China"}]
    ```

    返回包含所有信息的json数组，数组中的元素格式参见4中info格式

