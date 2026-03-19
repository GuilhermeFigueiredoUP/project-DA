#pragma once

#include <string>

enum DataType {
    Submission,
    Reviewer
};

typedef struct DataNode {
    DataType type;
    int id;
    int primaryDomain;
    int secondaryDomain;
    std::string email;
    std::string nameTitle;
    std::string authors;
}