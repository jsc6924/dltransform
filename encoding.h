#ifndef ENCODING_H
#define ENCODING_H
#include <QByteArray>
#include <vector>
#include <optional>

class encoding
{
public:
    enum class Encoding {
        UTF8, UTF16LE, UTF16BE, SHIFT_JIS, GB2312,
        _COUNT
    };
    encoding();
    static const char* enumToString(Encoding codec) {
        return supportedEncodings[static_cast<int>(codec)];
    }

    static Encoding stringToEnum(const std::string& str) {
        for (int i = 0; i < static_cast<int>(Encoding::_COUNT); ++i) {
            if (str == supportedEncodings[i]) {
                return static_cast<Encoding>(i);
            }
        }
        return Encoding::UTF8; // Default to UTF8 if not found
    }
    static std::optional<QByteArray> convert(const QByteArray &in, Encoding inCodec, Encoding outCodec);
    static std::vector<const char *> supportedEncodings;

};

#endif // ENCODING_H
