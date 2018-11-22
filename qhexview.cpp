//https://github.com/virinext/QHexView

// TODO:
// Сделать возможность скрытия окон через ~width=0~ через config
// FIXME: Адрес не изменяется при масштабировании окна

#include "qhexview.h"

QHexView::QHexView(QWidget *parent, DataStorage *data, Config *config):
    QAbstractScrollArea(parent),
    m_pdata(data), m_config(config) {
    setFont(QFont("Monospace", 14));

    m_charWidth     = uint16_t(fontMetrics().width(QLatin1Char('9')));
    m_charHeight    = uint16_t(fontMetrics().height());

    setMinimumWidth(m_charWidth * 14);

    setFocusPolicy(Qt::StrongFocus);
}


QHexView::~QHexView() {
    if(m_pdata)
        delete m_pdata;
}

void QHexView::recalcView() {
    uint16_t hexAsciiWidth = uint16_t(viewport()->size().width() - m_addressWidth);

    uint16_t gridOne = hexAsciiWidth / 4;

    m_hexWidth      = gridOne * 3;
    m_asciiWidth    = gridOne;
    m_bytesPerLine  = m_hexWidth / (m_charWidth * 3);
    m_posHex        = m_addressWidth;
    m_posAscii      = m_posHex + m_hexWidth;
}

void QHexView::setData(DataStorage *pData) {
    verticalScrollBar()->setValue(0);
    if(m_pdata) delete m_pdata;
    m_pdata = pData;
    m_cursorPos = 0;
    resetSelection(0);

    uint16_t maxAddressLength = uint16_t(QString::number(m_pdata->size() * 16, 16).length());

    m_addressWidth = (maxAddressLength + 1) * m_charWidth;

    recalcView();

    m_posHex    = m_addressWidth;
    m_posAscii  = m_addressWidth + m_hexWidth;
    viewport()->repaint();
}

void QHexView::showFromOffset(uint64_t offset) {
    if(m_pdata && offset < m_pdata->size()) {
        setCursorPos(offset * 2);

        int cursorY = m_cursorPos / (2 * m_bytesPerLine);

        verticalScrollBar() -> setValue(cursorY);
    }
}

void QHexView::clear() {
    verticalScrollBar()->setValue(0);
}

QSize QHexView::fullSize() const {
    if(!m_pdata) return QSize(0, 0);

    int width = m_posAscii + (m_bytesPerLine * m_charWidth);
    int height = int(m_pdata->size() / m_bytesPerLine);
    if(m_pdata->size() % m_bytesPerLine)
        height++;

    height *= m_charHeight;

    return QSize(width, height);
}

void QHexView::paintEvent(QPaintEvent *event) {
    if(!m_pdata) return;
    QPainter painter(viewport());

    recalcView();

    uint16_t linesInFile = uint16_t(m_pdata->size() / m_bytesPerLine);

    QSize areaSize = viewport()->size();
    QSize widgetSize = fullSize();

    verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);
    verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / m_charHeight + 1);

    uint16_t firstLineIdx = uint16_t(verticalScrollBar()->value());

    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;
    if(lastLineIdx >  linesInFile) {
        lastLineIdx = linesInFile;
        if(m_pdata->size() % m_bytesPerLine)
            lastLineIdx++;
    }

    painter.fillRect(event->rect(), this->palette().color(QPalette::Base));

    int maxAddressLength = QString::number(m_pdata->size() * 16, 16).length();

    QColor addressAreaColor = QColor(BG_COLOR_ADDRESS_AREA);
    painter.fillRect(QRect(0, event->rect().top(), m_posHex - m_charWidth / 2, height()), addressAreaColor);

    int linePos = m_posAscii - (GAP_HEX_ASCII / 2);

    painter.setPen(Qt::gray);
    painter.drawLine(linePos, event->rect().top(), linePos, height());
    painter.setPen(Qt::black);

    int yPosStart = m_charHeight;

    QBrush def = painter.brush();
    QBrush selected = QBrush(QColor(BG_COLOR_SELECTION));
    QByteArray data = m_pdata->getData(uint64_t(firstLineIdx * m_bytesPerLine), uint64_t((lastLineIdx - firstLineIdx) * m_bytesPerLine));

    for (int lineIdx = firstLineIdx, yPos = yPosStart;  lineIdx < lastLineIdx; lineIdx += 1, yPos += m_charHeight) {
        if(!(m_config->getViewShowAddress() || m_config->getViewShowHex() || m_config->getViewShowAscii())) break;

        QString address = QString("%1").arg(lineIdx * 16, maxAddressLength, 16, QChar('0'));
        painter.setPen(QColor(COLOR_ADDRESS));
        painter.drawText(0, yPos, address);

        for(int i = 0; (i < m_bytesPerLine) && ((lineIdx - firstLineIdx) * m_bytesPerLine + i) < data.size(); i++) {
            // Render HEX
            if(m_config->getViewShowHex()) {
                int xPos = m_posHex + i * m_charWidth * 3;

                uint64_t pos = uint64_t((lineIdx * m_bytesPerLine + i) * 2);

                if(pos >= m_selectBegin && pos < m_selectEnd) {
                    painter.setBackground(selected);
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }

                QString val = QString::number((data.at((lineIdx - firstLineIdx) * m_bytesPerLine + i) & 0xF0) >> 4, 16).toUpper();
                painter.setPen(QColor(COLOR_HEX));
                painter.drawText(xPos, yPos, val);


                if((pos + 1) >= m_selectBegin && (pos + 1) < m_selectEnd) {
                    // Draw selection
                    painter.setBackground(selected);
                } else {
                    painter.setBackground(def);
                }
                painter.setBackgroundMode(Qt::OpaqueMode);

                val = QString::number((data.at((lineIdx - firstLineIdx) * m_bytesPerLine + i) & 0xF), 16).toUpper();
                painter.setPen(QColor(COLOR_HEX));
                painter.drawText(xPos + m_charWidth, yPos, val); // TODO: Перевести в сетку

                painter.setBackground(def);
                painter.setBackgroundMode(Qt::OpaqueMode);

            }
            // Render ASCII
            if(m_config->getViewShowAscii()) {
                int xPosAscii = m_posAscii + i * m_charWidth;
                uint64_t pos = uint64_t((lineIdx * m_bytesPerLine + i));

                char ch = data[(lineIdx - firstLineIdx) * m_bytesPerLine + i];
                if ((ch < 0x20) || (ch > 0x7e)) ch = '.';

                painter.setPen(QColor(COLOR_ASCII));


                if(pos == m_cursorPos / 2) {
                    painter.setBackground(QBrush(QColor(BG_COLOR_CURSOR)));
                } else {
                    painter.setBackground(painter.brush());
                }
                painter.setBackgroundMode(Qt::OpaqueMode);

                if(pos >= m_selectBegin / 2 && pos < m_selectEnd / 2) {
                    painter.setBackground(selected);
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }

                painter.drawText(xPosAscii, yPos, QString(ch));

                painter.setBackground(def);
                painter.setBackgroundMode(Qt::OpaqueMode);
            }

        }

        if (hasFocus()) {
            uint16_t x = (m_cursorPos % (2 * m_bytesPerLine));
            uint16_t y = uint16_t(m_cursorPos / (2 * m_bytesPerLine));
            y -= firstLineIdx;
            uint16_t cursorX = (((x / 2) * 3) + (x % 2)) * m_charWidth + m_posHex;
            uint16_t cursorY = y * m_charHeight + 4;
            painter.fillRect(cursorX, cursorY, 2, m_charHeight, this->palette().color(QPalette::WindowText));
        }
    }
}

void QHexView::keyPressEvent(QKeyEvent *event) {
    bool setVisible = false;

    /*****************************************************************************/
    /* Cursor movements */
    /*****************************************************************************/
    if(event->matches(QKeySequence::MoveToNextChar)) {
        setCursorPos(m_cursorPos + 1);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousChar)) {
        if(m_cursorPos > 0) setCursorPos(m_cursorPos - 1);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToEndOfLine)) {
        setCursorPos(m_cursorPos | ((m_bytesPerLine * 2) - 1));
        resetSelection((int)m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToStartOfLine)) {
        setCursorPos(m_cursorPos | (m_cursorPos % (m_bytesPerLine * 2)));
        resetSelection((int)m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousLine)) {
        setCursorPos(m_cursorPos - m_bytesPerLine * 2);
        resetSelection((int)m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToNextLine)) {
        setCursorPos(m_cursorPos + m_bytesPerLine * 2);
        resetSelection((int)m_cursorPos);
        setVisible = true;
    }

    if(event->matches(QKeySequence::MoveToNextPage)) {
        setCursorPos(m_cursorPos + (viewport()->height() / m_charHeight - 1) * 2 * m_bytesPerLine);
        resetSelection((int)m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousPage)) {
        setCursorPos(m_cursorPos - (viewport()->height() / m_charHeight - 1) * 2 * m_bytesPerLine);
        resetSelection((int)m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToEndOfDocument)) {
        if(m_pdata)
            setCursorPos(m_pdata->size() * 2);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToStartOfDocument)) {
        setCursorPos(0);
        resetSelection(m_cursorPos);
        setVisible = true;
    }

    /*****************************************************************************/
    /* Select commands */
    /*****************************************************************************/
    if (event->matches(QKeySequence::SelectAll)) {
        resetSelection(0);
        if(m_pdata)
            setSelection(2 * m_pdata->size() + 1);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectNextChar)) {
        int pos = m_cursorPos + 1;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectPreviousChar)) {
        int pos = m_cursorPos - 1;
        setSelection(pos);
        setCursorPos(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectEndOfLine)) {
        int pos = m_cursorPos - (m_cursorPos % (2 * m_bytesPerLine)) + (2 * m_bytesPerLine);
        setCursorPos((int)pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectStartOfLine)) {
        int pos = m_cursorPos - (m_cursorPos % (2 * m_bytesPerLine));
        setCursorPos((int)pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectPreviousLine)) {
        int pos = m_cursorPos - (2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectNextLine)) {
        int pos = m_cursorPos + (2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }

    if (event->matches(QKeySequence::SelectNextPage)) {
        int pos = m_cursorPos + (((viewport()->height() / m_charHeight) - 1) * 2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectPreviousPage)) {
        int pos = m_cursorPos - (((viewport()->height() / m_charHeight) - 1) * 2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectEndOfDocument)) {
        int pos = 0;
        if(m_pdata)
            pos = m_pdata->size() * 2;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectStartOfDocument)) {
        int pos = 0;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }

    if (event->matches(QKeySequence::Copy)) {
        if(m_pdata) {
            QString res;
            int idx = 0;
            int copyOffset = 0;

            QByteArray data = m_pdata->getData(m_selectBegin / 2, (m_selectEnd - m_selectBegin) / 2 + 1);
            if(m_selectBegin % 2) {
                res += QString::number((data.at((idx+1) / 2) & 0xF), 16);
                res += " ";
                idx++;
                copyOffset = 1;
            }

            int selectedSize = m_selectEnd - m_selectBegin;
            for (; idx < selectedSize; idx+= 2) {
                QString val = QString::number((data.at((copyOffset + idx) / 2) & 0xF0) >> 4, 16);
                if(idx + 1 < selectedSize) {
                    val += QString::number((data.at((copyOffset + idx) / 2) & 0xF), 16);
                    val += " ";
                }
                res += val;

                if((idx/2) % m_bytesPerLine == (m_bytesPerLine - 1))
                    res += "\n";
            }
            QClipboard *clipboard = QApplication::clipboard();
            clipboard -> setText(res);
        }
    }

    if(setVisible)
        ensureVisible();
    viewport() -> update();
}

void QHexView::mouseMoveEvent(QMouseEvent * event) {
    int actPos = cursorPos(event->pos());
    setCursorPos(actPos);
    setSelection(actPos);

    viewport() -> update();
}

void QHexView::mousePressEvent(QMouseEvent * event) {
    uint64_t cPos = cursorPos(event->pos());

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) && event->button() == Qt::LeftButton)
        setSelection(int(cPos));
    else
        resetSelection(int(cPos));

    setCursorPos(cPos);

    viewport() -> update();
}


uint64_t QHexView::cursorPos(const QPoint &position) {
    uint64_t pos = 0;

    if ((position.x() >= m_posHex) && (position.x() < m_posAscii - m_charWidth)) {
        int x = (position.x() - m_posHex) / m_charWidth;
        if ((x % 3) == 0)
            x = (x / 3) * 2;
        else
            x = ((x / 3) * 2) + 1;

        int firstLineIdx = verticalScrollBar() -> value();
        int y = (position.y() / m_charHeight) * 2 * m_bytesPerLine;
        pos = uint64_t(x + y + firstLineIdx * m_bytesPerLine * 2);
    } else if(position.x() > m_posAscii) {
        int x = (position.x() - m_posAscii) / m_charWidth;

        int firstLineIdx = verticalScrollBar() -> value();
        int y = (position.y() / m_charHeight) * 2 * m_bytesPerLine;
        pos = uint64_t(x * 2 + 1 + y + firstLineIdx * m_bytesPerLine * 2);
    }
    return pos;
}


void QHexView::resetSelection() {
    m_selectBegin = m_selectInit;
    m_selectEnd = m_selectInit;
}

void QHexView::resetSelection(int pos) {
    if (pos < 0)
        pos = 0;

    m_selectInit    = uint64_t(pos);
    m_selectBegin   = uint64_t(pos);
    m_selectEnd     = uint64_t(pos);
}

void QHexView::setSelection(int pos) {
    if (pos < 0)
        pos = 0;

    if (uint64_t(pos) >= m_selectInit) {
        m_selectEnd = uint16_t(pos);
        m_selectBegin = m_selectInit;
    } else {
        m_selectBegin = uint16_t(pos);
        m_selectEnd = m_selectInit;
    }
}


void QHexView::setCursorPos(uint64_t position = 0) {
    uint64_t maxPos = 0;
    if(m_pdata) {
        maxPos = m_pdata->size() * 2;
        if(m_pdata->size() % m_bytesPerLine)
            maxPos++;
    }

    if(position > maxPos)
        position = maxPos;

    m_cursorPos = uint64_t(position);
}

void QHexView::ensureVisible() {
    QSize areaSize = viewport()->size();

    int firstLineIdx = verticalScrollBar() -> value();
    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;

    int cursorY = int(m_cursorPos / (2 * m_bytesPerLine));

    if(cursorY < firstLineIdx)
        verticalScrollBar() -> setValue(cursorY);
    else if(cursorY >= lastLineIdx)
        verticalScrollBar() -> setValue(cursorY - areaSize.height() / m_charHeight + 1);
}
