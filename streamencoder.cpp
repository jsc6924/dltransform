#include "streamencoder.h"
#include <QString>

QString DoubleLineStreamEncoder::encode(const Stream &stream)
{
    QString res;
    int jtag = stream.getChannelIndex("jtags");
    int jtext = stream.getChannelIndex("jtexts");
    int ctag = stream.getChannelIndex("ctags");
    int ctext = stream.getChannelIndex("ctexts");
    for(int i = 0; i < stream.frameCount(); i++) {
        res.append(QString("★%1★%2\n").arg(stream[jtag][i], stream[jtext][i]));
        res.append(QString("☆%1☆%2\n\n\n").arg(stream[ctag][i], stream[ctext][i]));
    }
    return res;
}
