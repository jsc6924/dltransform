#include "data.h"


int Stream::addChannel(const QString &name)
{
    if (frameCount() != 0) {
        return -1;
    }
    if (nameToChannelIndex.count(name)) {
        return nameToChannelIndex.at(name);
    }
    m_data.append(QStringList{});
    nameToChannelIndex[name] = m_data.size() - 1;
    return m_data.size() - 1;
}

bool Stream::addFrame(const QStringList &frame)
{
    if (frame.size() != channelCount()) {
        return false;
    }
    for (int i = 0; i < frame.size(); i++) {
        m_data[i].append(frame[i]);
    }
    return true;
}
