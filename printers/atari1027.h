#ifndef ATARI1027_H
#define ATARI1027_H

#include "atariprinter.h"

#include <QFont>
#include <QFontMetrics>
#include <QPrinter>
#include <QRect>

class Atari1027 : public AtariPrinter
{
    Q_OBJECT
public:
    Atari1027(SioWorker *worker);

    virtual void setupFont();

private:
    int m_lastOperation;
    bool mESC;

    virtual bool handleBuffer(QByteArray &buffer, int len);
    bool handleEscapedCodes(const char b);
    bool handlePrintableCodes(const char b);
};

#endif // ATARI1027_H
