#include <stdio.h>
#include "attackResult.h"

class hashFunction{
public:
    size_t operator ()(const AttackResult &obj) const{
        return hash<int>()(obj.r);
    }
};
