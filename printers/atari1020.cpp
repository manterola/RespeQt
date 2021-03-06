#include "atari1020.h"
#include "logdisplaydialog.h"
#include "math.h"
#include "respeqtsettings.h"

#include <stdexcept>
#include <QFontDatabase>
#include <QPoint>

namespace Printers
{
    const unsigned char Atari1020::BLACK = 0;
    const unsigned char Atari1020::BLUE = 1;
    const unsigned char Atari1020::RED = 3;
    const unsigned char Atari1020::GREEN = 2;
    std::map<unsigned char, QColor> Atari1020::sColorMapping = {
        { Atari1020::BLACK, QColor("black") },
        { Atari1020::BLUE, QColor("blue") },
        { Atari1020::RED, QColor("red") },
        { Atari1020::GREEN, QColor("green") }
    };

    Atari1020::Atari1020(SioWorker *sio)
        : AtariPrinter(sio),
          mGraphicsMode(false),
          mEsc(false),
          mPrintText(false),
          mTextOrientation(0)
    {
        mTypeId = ATARI1020;
        mTypeName = QString("Atari 1020");
    }

    void Atari1020::setupOutput()
    {
        AtariPrinter::setupOutput();
        if (mOutput)
        {
            mOutput->setWindow(QRect(0, -999, 480, 1000));
        }
    }

    void Atari1020::setupFont()
    {
        if (mOutput)
        {
            QFontDatabase fonts;
            if (!fonts.hasFamily("Lucida Console"))
            {
                QFontDatabase::addApplicationFont(":/fonts/1020");
            }
            // TODO calculate the correct font size.
            mFontSize = 10;
            QFont *font = new QFont("Lucida Console", mFontSize);
            font->setUnderline(false);
            mOutput->setFont(font);
            mOutput->calculateFixedFontSize(80);
            mOutput->setPen(sColorMapping.at(0));
        }
    }

    bool Atari1020::handleBuffer(QByteArray &buffer, int len)
    {
        len = std::min(len, buffer.count());
        for(int i = 0; i < len; i++) {
            unsigned char b = buffer.at(i);

            if (b == 155) // EOL
            {
                endCommandLine();
                return true; // Drop the rest of the buffer
            }

            if (mGraphicsMode) {
                if (b == 65) // A; Leave graphics mode
                {
                    mGraphicsMode = false;
                } else if (mPrintText)
                    handlePrintableCodes(b);
                else
                    handleGraphicsMode(buffer, len, i);
            } else {
                // Textmode
                if (b == 27) // ESC
                {
                    mEsc = true;
                } else if (mEsc && b == 27) // Cancel Esc
                {
                    mEsc = false;
                } else if (mEsc && b == 16) // CTRL+P: 20 characters
                {
                    mOutput->calculateFixedFontSize(20);
                    /*QFont *font = mOutput->font();
                    font->setPixelSize(mFontSize * 4);
                    mOutput->setFont(font);*/
                    mEsc = false;
                } else if (mEsc && b == 19) // CTRL+S: 80 characters
                {
                    mOutput->calculateFixedFontSize(80);
                    /*QFont *font = mOutput->font();
                    font->setPixelSize(mFontSize);
                    mOutput->setFont(font);*/
                    mEsc = false;
                } else if (mEsc && b == 14) // CTRL+N: 40 characters
                {
                    mOutput->calculateFixedFontSize(40);
                    /*QFont *font = mOutput->font();
                    font->setPixelSize(mFontSize * 2);
                    mOutput->setFont(font);*/
                    mEsc = false;
                } else if (mEsc && b == 23) // CTRL+W: Enter international mode
                {
                    mInternational = true;
                    mEsc = false;
                } else if (mEsc && b == 24) // CTRL+X: Exit international mode
                {
                    mInternational = false;
                    mEsc = false;
                } else
                    handlePrintableCodes(b);
            }
        }

        return true;
    }

    void Atari1020::endCommandLine()
    {
        if (mGraphicsMode)
        {
            mPrintText = false;
        } else {
            mOutput->newLine();
        }
    }

    bool Atari1020::handleGraphicsMode(QByteArray &buffer, int len, int &i)
    {
        char b = buffer.at(i);

        switch (b)
        {
            case 'H': // return to HOME
                mPenPoint.setX(0);
                mPenPoint.setY(0);
            break;

            case 'S': // Scale characters
                try {
                    int end;
                    int scale = fetchIntFromBuffer(buffer, len, i, end);
                    if (scale >= 0 && scale <= 63)
                    {
                        QFont *font = mOutput->font();
                        font->setPixelSize(font->pixelSize() * scale);
                        i = end;
                    } else
                        handlePrintableCodes(b);
                } catch(...) {
                    qDebug() << "!n" << tr("[%1] parsing error for scale command").arg(deviceName());
                    handlePrintableCodes(b);
                }
            break;

            case 'C': // Set Color
            {
                char next = buffer.at(i + 1);
                if (next >= '0' && next <= '3')
                {
                    mOutput->setPen(sColorMapping[next - '0']);
                    i++;
                } else
                    handlePrintableCodes(b);
            }
            break;

            case 'L': // Line mode 0 = solid, anything else dashed
                try {
                    int end;
                    int line = fetchIntFromBuffer(buffer, len, i, end);
                    if (line >= 0 && line <= 15)
                    {
                        QPen pen = mOutput->pen();
                        if (line == 0) {
                            pen.setStyle(Qt::SolidLine);
                        } else {
                            pen.setStyle(Qt::CustomDashLine);
                            QVector<qreal> pattern;
                            pattern << 1 << line;
                            pen.setDashPattern(pattern);
                        }
                        mOutput->setPen(pen);
                        i = end;
                    } else {
                        handlePrintableCodes(b);
                    }
                } catch(...) {
                    qDebug() << "!n" << tr("[%1] parsing error for line command").arg(deviceName());
                    handlePrintableCodes(b);
                }
            break;

            case 'I': // Init plotter
                mOutput->translate(mPenPoint);
            break;

            case 'D': // draw to point
            case 'M': // move to point
            case 'J': // draw relative
            case 'R': // move relative
            {
                char command = b;
                do {
                    try {

                        int endx, endy;
                        int x = fetchIntFromBuffer(buffer, len, i, endx);
                        if (buffer.at(endx + 1) != ',')
                            throw new std::invalid_argument("expected ,");
                        if (x < 0 || x > 480)
                            throw new std::invalid_argument("x coordinate out of range");

                        int y = fetchIntFromBuffer(buffer, len, endx + 1, endy);
                        if (y < -999 && y > 999)
                            throw new std::invalid_argument("y coordinate out of range");

                        i = endy;

                        QPointF point(x, y);

                        switch (command) {
                            case 'D':
                                mOutput->drawLine(mPenPoint, point);
                                mPenPoint = point;
                            break;

                            case 'J':
                                point += mPenPoint;
                                mOutput->drawLine(mPenPoint, point);
                                mPenPoint = point;
                            break;

                            case 'M':
                                mPenPoint = point;
                            break;

                            case 'R':
                                mPenPoint += point;
                            break;
                        }

                        b = buffer.at(i + 1);
                    } catch(...)
                    {
                        qDebug() << "!n" << tr("[%1] parsing error for draw command").arg(deviceName());
                        handlePrintableCodes(b);
                        break;
                    }
                } while(b == ';');
                if (b == '*')
                {
                    i++;
                }
            }
            break;

            case 'X': // draw axes 0 = Y-axis, otherwise X-axis
                try {
                    int i_ = i, end, mode, count, size;
                    mode = fetchIntFromBuffer(buffer, len, i_, end);
                    if (len < end + 1) throw new std::invalid_argument("expected comma, buffer too short");
                    i_ = end + 1; // separator check
                    if (buffer.at(i_)) throw new std::invalid_argument(QString("expected comma, got %1").arg(b).toStdString());
                    i_++;

                    size = fetchIntFromBuffer(buffer, len, i_, end);
                    i_ = end + 1; // separator check
                    if (buffer.at(i_)) throw new std::invalid_argument(QString("expected comma, got %1").arg(b).toStdString());
                    i_++;

                    count = fetchIntFromBuffer(buffer, len, i_, end);
                    i_ = end;
                    drawAxis(mode == 0, size, count);
                    i = i_;
                } catch(...)
                {
                    qDebug() << "!n" << tr("[%1] parsing error for axis draw").arg(deviceName());
                }
            break;

            case 'Q': // select text orientation
                try {
                    int endx;
                    mTextOrientation = fetchIntFromBuffer(buffer, len, i, endx);
                    if (mTextOrientation < 0 || mTextOrientation > 3) throw new std::invalid_argument("orientation number out of range");
                    i = endx;
                } catch(...) {
                    qDebug() << "!n" << tr("[%1] parsing error for text orientation").arg(deviceName());
                    handlePrintableCodes(b);
                    break;
                }
            break;

            case 'P': // print text
                mPrintText = true;
            break;
        }

        return true;
    }

    bool Atari1020::drawAxis(bool xAxis, int size, int count)
    {
        QPointF start = QPointF(mPenPoint);
        QPointF end = QPointF(start);

        if (xAxis)
        {
            end.setX(end.x() + size * count);
        } else {
            end.setY(end.y() + size * count);
        }
        mOutput->drawLine(start, end);

        for (int c = 1; c <= count; c++)
        {
            if (xAxis)
            {
                qreal xc = mPenPoint.x() + c * size;
                mOutput->drawLine(QPointF(xc, mPenPoint.y() - 2), QPointF(xc, mPenPoint.y() + 2));
            } else {
                qreal yc = mPenPoint.y() + c * size;
                mOutput->drawLine(QPointF(mPenPoint.x() - 2, yc), QPointF(mPenPoint.x() + 2, yc));
            }
        }

        return true;
    }

    int Atari1020::fetchIntFromBuffer(QByteArray &buffer, int /*len*/, int i, int &end)
    {
        int result = 0;
        QString number("");

        end = i + 1;
        char next = buffer.at(end);

        // Is there a sign?
        if (next == '-' || next == '+') {
            number.append(next);
            end++;
            next = buffer.at(end);
        }

        // Search for next digit
        while (next >= '0' && next <= '9')
        {
            number.append(next);
            end++;
            next = buffer.at(end);
        }
        bool ok;
        result = number.toInt(&ok);
        if (ok)
            return result;
        else
            throw new std::invalid_argument("couldn't convert to int");
    }

    bool Atari1020::handlePrintableCodes(const char b)
    {
        QChar qb = translateAtascii(b & 127); // Masking inverse characters.
        mOutput->printChar(qb);

        return true;
    }
}
