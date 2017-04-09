#include "nativeprintersupport.h"

NativePrinterSupport::NativePrinterSupport(SioWorker *sio)
    : BasePrinter(sio),
      mPainter(NULL),
      mFontMetrics(NULL)
{
    setupPrinter();
    setupFont();
}

NativePrinterSupport::~NativePrinterSupport()
{
    if (requiresNativePrinter())
    {
        endPrint();
    }
}

void NativePrinterSupport::setupPrinter() {
    mNativePrinter = new QPrinter();
    mPainter = new QPainter();
}

bool NativePrinterSupport::handleBuffer(QByteArray & /*buffer*/, int /*len*/)
{
    if (!mPrinting)
    {
        mPrinting = true;
        beginPrint();
    }
    return true;
}

void NativePrinterSupport::beginPrint() {
    if (mPrinting)
    {
        mPainter->begin(mNativePrinter);
        mPainter->setFont(mFont);
        mBoundingBox = mNativePrinter->pageRect();
        x = mBoundingBox.left();
        y = mBoundingBox.top() + mFontMetrics->lineSpacing();
    }
}

void NativePrinterSupport::endPrint() {
    if (mPainter->isActive())
    {
        mPainter->end();
    }
}
