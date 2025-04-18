#include "formatter.h"

DoubleLineFormatter::DoubleLineFormatter()
:   jpLineFormat("○%1○%2")
    , cnLineFormat("●%1●%2")
{

}
QString DoubleLineFormatter::format(const Stream &s) const
{
    QString res;
    auto i1 = s.getChannelIndex("jtags");
    auto i2 = s.getChannelIndex("jtexts");
    auto j1 = s.getChannelIndex("ctags");
    auto j2 = s.getChannelIndex("ctexts");
    QString jp = QString("%1\n").arg(jpLineFormat);
    QString cn = QString("%1\n\n").arg(cnLineFormat);

    for(int i = 0; i < s.frameCount(); i++) {
        res.append(jp.arg(s[i1][i], s[i2][i]));
        res.append(cn.arg(s[j1][i], s[j2][i]));
    }
    return res;
}
