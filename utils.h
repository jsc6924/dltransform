#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <unordered_map>
#include <QString>

using std::vector;
using std::unordered_map;

class utils
{
public:
    utils();

    static vector<QString> loadFilesFromDirectory(const QString &dirPath, bool recursively);
};

#endif // UTILS_H
