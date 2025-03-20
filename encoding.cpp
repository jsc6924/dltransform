#include "encoding.h"
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <clocale>
#include <QLocale>

encoding::encoding() {}

std::vector<const char *> encoding::supportedEncodings = {
    "UTF-8", "UTF-16LE", "UTF-16BE", "SHIFT-JIS", "GB2312"
};


std::optional<QByteArray> encoding::convert(const QByteArray &in, Encoding inCodec, Encoding outCodec) {
    const char *inCodecStr = enumToString(inCodec);
    const char *outCodecStr = enumToString(outCodec);
    char outCodeStrTranslit[128];

    sprintf(outCodeStrTranslit, "%s//IGNORE", outCodecStr);

    iconv_t cd = iconv_open(outCodeStrTranslit, inCodecStr);
    if (cd == (iconv_t)-1) {
        return std::nullopt;
    }

    size_t outbufSize = in.size() * 2 + 16;
     std::unique_ptr<char[]> outbuf(new char[outbufSize]);
    memset(outbuf.get(), 0, outbufSize);
    char *inbuf = (char *)in.constData();
    char *inptr = inbuf;
    size_t input_remaining = in.size();

    QByteArray out;

    while (input_remaining > 0) {
        memset(outbuf.get(), 0, outbufSize);
        char *outptr = outbuf.get();
        size_t result = iconv(cd, &inptr, &input_remaining, &outptr, &outbufSize);
        if (result == (size_t)-1) {
            return std::nullopt;
        }
        out.append(outbuf.get(), outptr - outbuf.get());
    }

    return out;
}


int testICU() {
    const char *encodings[] = {"UTF-8", "SHIFT-JIS", "GB2312", "ISO-8859-1", "UTF-16LE", "UTF-16BE", NULL};

    qDebug() << "日本語おはようございます！";

    const char* input = "日本語お疲れ様です！";
    size_t inputLen = strlen(input);
    iconv_t cd = iconv_open("SHIFT-JIS", "UTF-8");
    if (cd == (iconv_t)-1) {
        qDebug() << "iconv_open failed!";
        return 1;
    }

    // Prepare input and output buffers
    char output[1024];  // Buffer to hold the UTF-8 output
    memset(output, 0, 1024);
    char* inputPtr = (char*)input;
    char* outputPtr = output;
    size_t outputLen = sizeof(output);

    // Perform the conversion
    size_t result = iconv(cd, &inputPtr, &inputLen, &outputPtr, &outputLen);
    if (result == (size_t)-1) {
        qDebug() << "iconv conversion failed!";
    } else {
        qDebug() << "Converted string: " << std::string_view(output, outputLen);
        const char *filename = "converted.txt";
        FILE *file = fopen(filename, "wb");
        if (file) {
            fwrite(output, 1, (outputPtr - output), file);
            fclose(file);
            printf("Converted text saved to %s\n", filename);
        }
    }

    iconv_close(cd);  // Don't forget to close the iconv descriptor
    return 0;
}
