#include "utils.h"

#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include <QDir>


utils::utils() {}

vector<QString> utils::loadFilesFromDirectory(const QString &dirPath, bool recursively = false) {
    return utils::loadFilesFromDirectory(QDir(dirPath), recursively);
}
vector<QString> utils::loadFilesFromDirectory(const QDir &dirPath, bool recursively = false)
{
    QDir directory(dirPath);

    // Check if the directory exists
    if (!directory.exists()) {
        qWarning() << "Directory does not exist!";
        return {};
    }

    // Set filters to include files and all directories (for recursion)
    QStringList filters;
    filters << "*.txt";  // Filter for text files only

    // Get all files (including those in subdirectories)
    QFileInfoList files = directory.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

    vector<QString> results;

    // Iterate over files and print file names
    for (const QFileInfo &fileInfo : std::as_const(files)) {
        results.emplace_back(fileInfo.absoluteFilePath());
    }

    if (recursively) {
        // Recursively search for text files in subdirectories
        QFileInfoList subDirs = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo &subDirInfo :  std::as_const(subDirs)) {
            // Recursive call to search in subdirectory
            auto subResults = loadFilesFromDirectory(subDirInfo.absoluteFilePath(), true);
            for(auto r: subResults) {
                results.emplace_back(r);
            }
        }
    }

    return results;
}
