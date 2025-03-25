#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QStringList>
#include <QList>
#include <unordered_map>
class Stream {
public:
    QList<QStringList> &data() {
        return m_data;
    }
    int channelCount() const {
        return m_data.size();
    }
    int frameCount() const {
        if (m_data.isEmpty()) {
            return 0;
        }
        return m_data[0].size();
    }
    int getChannelIndex(const QString &name) const {
        if (nameToChannelIndex.count(name)) {
            return nameToChannelIndex.at(name);
        }
        return -1;
    }
    int addChannel(const QString &name);

    bool addFrame(const QStringList &frame);


    const QStringList &operator[](int i) const {
        return m_data[i];
    }
    QStringList &operator[](int i) {
        return m_data[i];
    }
private:
    QList<QStringList> m_data;
    std::unordered_map<QString, int> nameToChannelIndex;
};

struct Context {
    QString filename;
};

#endif // DATA_H
