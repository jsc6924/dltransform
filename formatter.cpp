#include "formatter.h"

QString DefaultFormatter::format(const Stream &s) const
{
    QString res;
    auto i1 = s.getChannelIndex("jtags");
    auto i2 = s.getChannelIndex("jtexts");
    auto j1 = s.getChannelIndex("ctags");
    auto j2 = s.getChannelIndex("ctexts");
    for(int i = 0; i < s.frameCount(); i++) {
        res.append(QString("○%1○%2\n").arg(s[i1][i], s[i2][i]));
        res.append(QString("●%1●%2\n\n").arg(s[j1][i], s[j2][i]));
    }
    return res;
}
