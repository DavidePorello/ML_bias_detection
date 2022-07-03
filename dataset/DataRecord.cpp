#include <sstream>
#include "DataRecord.h"

bool DataRecord::isPrunable() {
    for(string& str : this->raw_data)
        if(str == "Not in universe")
            return true;
    return false;
}
