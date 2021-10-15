#include "attackResult.h"

void AttackResult::show(){
    printf("============= Attack Result ==============\n");
    printf("Attacked node : ");
    for(auto it=nodes.begin();it!=nodes.end();it++){
        printf("%d ",*it);
    }
    printf("\n");
    printf("Average shortest path: %f\n",ASPLen);
    printf("Maximal Connected Subgraph node number is %d, Ratio is %f\n",maxCompNum,r);
}
