//https://github.com/virinext/QHexView

// TODO:
//   Исправить возможность скрытия окон через config

#include "qhexview.h"

QHexView::QHexView(QWidget *parent, DataStorage *data, Config *config, QStatusBar *statusBar, QUndoStack *undoStack) :
    QAbstractScrollArea(parent),
    m_data(data), m_config(config), m_statusBar(statusBar), m_undoStack(undoStack) {
    setFont(QFont("Monospace", 14));

    m_charWidth     = uint16_t(fontMetrics().width(QLatin1Char('9')));
    m_charHeight    = uint16_t(fontMetrics().height());

    m_window        = WINDOW_HEX;
    m_mode          = MODE_READONLY;

    setMinimumWidth(m_charWidth * 10);

    setFocusPolicy(Qt::StrongFocus);
}


QHexView::~QHexView() {
    if(m_data)
        delete m_data;
}

void QHexView::recalcView() {
    if(!m_data) return;
    m_maxAddressLength = uint16_t(QString::number(m_data->size() * m_bytesPerLine * 2, 16).length() - 1);
    m_addressWidth = (m_maxAddressLength + 1) * m_charWidth;

    uint16_t hexAsciiWidth = uint16_t(viewport()->size().width() - m_addressWidth);

    uint16_t gridOne = hexAsciiWidth / 4;

    m_hexWidth      = gridOne * 3;
    m_asciiWidth    = gridOne;
    m_bytesPerLine  = m_hexWidth / (m_charWidth * 3);
    m_posHex        = m_addressWidth;
    m_posAscii      = m_posHex + m_hexWidth;

    if(!m_bytesPerLine) m_bytesPerLine = 1; // Divide by zero
}

uint64_t QHexView::getCursorPosition() {
    return m_cursorPos;
}

void QHexView::setData(DataStorage *pData) {
    verticalScrollBar()->setValue(0);
    if(m_data) delete m_data;
    m_data = pData;

    m_window    = WINDOW_HEX;
    m_mode      = MODE_WRITE_REPLACE;
    setCursorPos(0);
    resetSelection(0);

    recalcView();

    m_posHex    = m_addressWidth;
    m_posAscii  = m_addressWidth + m_hexWidth;
    viewport()->repaint();
    statusBarUpdate();
}

void QHexView::showFromOffset(int64_t offset) {
    if(!m_data) return;
    if(m_data && offset < m_data->size()) {
        setCursorPos(offset);

        int32_t cursorY = int32_t((m_cursorPos / 2) / (2 * m_bytesPerLine));

        verticalScrollBar()->setValue(cursorY);
        update();
    }
}

void QHexView::select(int64_t start, int64_t end) {
    if(start > end || start < 0 || start >= m_data->size() || end < 0 || end >= m_data->size()) {
        qDebug() << "QHexView::select [ERROR]: Invalid start/end position:" << start << end;
        return;
    }
    m_selectBegin   = start;
    m_selectEnd     = end;
}

void QHexView::update() {
    viewport()->update();
}

void QHexView::clear() {
    verticalScrollBar()->setValue(0);
}

QSize QHexView::fullSize() const {
    if(!m_data) return QSize(0, 0);

    int width = m_posAscii + (m_bytesPerLine * m_charWidth);
    int height = int(m_data->size() / m_bytesPerLine);
    if(m_data->size() % m_bytesPerLine)
        height++;

    height *= m_charHeight;

    return QSize(width, height);
}

void QHexView::paintEvent(QPaintEvent *event) {
    if(!m_data) return;
    QPainter painter(viewport());

    recalcView();

    uint16_t linesInFile = uint16_t(m_data->size() / m_bytesPerLine);

    QSize areaSize = viewport()->size();
    QSize widgetSize = fullSize();

    verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);
    verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / m_charHeight + 1);

    uint16_t firstLineIdx = uint16_t(verticalScrollBar()->value());

    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;
    if(lastLineIdx > linesInFile) {
        lastLineIdx = linesInFile;
        if(m_data->size() % m_bytesPerLine)
            lastLineIdx++;
    }

    painter.fillRect(event->rect(), this->palette().color(QPalette::Base));

    QColor addressAreaColor = m_config->getColorAddressArea();
    painter.fillRect(QRect(0, event->rect().top(), m_posHex - m_charWidth / 2, height()), addressAreaColor);

    if(m_window == 0) { // If HEX or ASCII window is activeare
        painter.setPen(m_config->getColorActiveWindow());
    } else {
        painter.setPen(Qt::gray);
    }
    painter.drawLine(m_posHex - m_charWidth / 2, event->rect().top(), m_posHex - m_charWidth / 2, height());

    int linePos = m_posAscii - (GAP_HEX_ASCII / 2);

    if(m_window == 0 || m_window == 1) { // If HEX or ASCII window is activeare
        painter.setPen(m_config->getColorActiveWindow());
    } else {
        painter.setPen(Qt::gray);
    }
    painter.drawLine(linePos, event->rect().top(), linePos, height());
    painter.setPen(Qt::black);

    int yPosStart = m_charHeight;

    QBrush def = painter.brush();
    QBrush selected = QBrush(m_config->getColorSelection());
    QByteArray data = m_data->getData(uint64_t(firstLineIdx * m_bytesPerLine), uint64_t((lastLineIdx - firstLineIdx) * m_bytesPerLine));

    for (int lineIdx = firstLineIdx, yPos = yPosStart;  lineIdx < lastLineIdx; lineIdx += 1, yPos += m_charHeight) {
        if(!(m_config->getViewShowAddress() || m_config->getViewShowHex() || m_config->getViewShowAscii())) break;

        if(m_config->getViewShowAddress()) {
            QString address = QString("%1").arg(lineIdx * m_bytesPerLine * 2, m_maxAddressLength, 16, QChar('0'));

            painter.setPen(m_config->getColorAddress());
            painter.drawText(0, yPos, address);
        }

        for(int i = 0; (i < m_bytesPerLine) && ((lineIdx - firstLineIdx) * m_bytesPerLine + i) < data.size(); i++) {
            // Render HEX
            if(m_config->getViewShowHex()) {
                int xPos = m_posHex + i * m_charWidth * 3;

                uint64_t pos = uint64_t((lineIdx * m_bytesPerLine + i) * 2);

                if(m_mode != 1 && pos == m_cursorPos) {
                    painter.setBackground(QBrush(m_config->getColorCursor()));
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }

                if(pos >= m_selectBegin && pos < m_selectEnd) {
                    painter.setBackground(selected);
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }

                QString val = QString::number((data.at((lineIdx - firstLineIdx) * m_bytesPerLine + i) & 0xF0) >> 4, 16).toUpper();
                painter.setPen(m_config->getColorHex());
                painter.drawText(xPos, yPos, val);


                if((pos + 1) >= m_selectBegin && (pos + 1) < m_selectEnd) {
                    // Draw selection
                    painter.setBackground(selected);
                } else {
                    painter.setBackground(def);
                }
                painter.setBackgroundMode(Qt::OpaqueMode);

                if(m_mode != 1 && pos == m_cursorPos - 1) {
                    painter.setBackground(QBrush(m_config->getColorCursor()));
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }

                val = QString::number((data.at((lineIdx - firstLineIdx) * m_bytesPerLine + i) & 0xF), 16).toUpper();
                painter.setPen(m_config->getColorHex());
                painter.drawText(xPos + m_charWidth, yPos, val); // TODO: Перевести в сетку

                painter.setBackground(def);
                painter.setBackgroundMode(Qt::OpaqueMode);

            }
            // Render ASCII
            if(m_config->getViewShowAscii()) {
                int xPosAscii = m_posAscii + i * m_charWidth;
                uint64_t pos = uint64_t((lineIdx * m_bytesPerLine + i));

                char ch = data[(lineIdx - firstLineIdx) * m_bytesPerLine + i];
                if((ch < 0x20) || (ch > 0x7e)) ch = '.';

                painter.setPen(m_config->getColorAscii());


                if(m_mode != 1 && pos == m_cursorPos / 2) {
                    painter.setBackground(QBrush(m_config->getColorCursor()));
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

        if(m_mode == 1) { // If write (insert) mode
            uint16_t x = (m_cursorPos % (2 * m_bytesPerLine));
            uint16_t y = uint16_t(m_cursorPos / (2 * m_bytesPerLine));
            y -= firstLineIdx;
            uint16_t cursorX = (((x / 2) * 3) + (x % 2)) * m_charWidth + m_posHex;
            uint16_t cursorY = y * m_charHeight + 4;
            painter.fillRect(cursorX, cursorY, 2, m_charHeight, this->palette().color(QPalette::WindowText));

            x = (m_cursorPos / 2) % (m_bytesPerLine);
            cursorX = m_posAscii + (x * m_charWidth);
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
        if(m_window == 0) // HEX window
            setCursorPos(m_cursorPos + 1);
        else
            setCursorPos(m_cursorPos + 2);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousChar)) {
        if(m_window == 0) // HEX window
            setCursorPos(m_cursorPos - 1);
        else
            setCursorPos(m_cursorPos - 2);
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
        if(m_data)
            setCursorPos(m_data->size() * 2);
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
    if(event->matches(QKeySequence::SelectAll)) {
        resetSelection(0);
        if(m_data)
            setSelection(2 * m_data->size() + 1);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectNextChar)) {
        int pos = m_cursorPos + 1;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectPreviousChar)) {
        int pos = m_cursorPos - 1;
        setSelection(pos);
        setCursorPos(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectEndOfLine)) {
        int pos = m_cursorPos - (m_cursorPos % (2 * m_bytesPerLine)) + (2 * m_bytesPerLine);
        setCursorPos((int)pos);
        setSelection(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectStartOfLine)) {
        int pos = m_cursorPos - (m_cursorPos % (2 * m_bytesPerLine));
        setCursorPos((int)pos);
        setSelection(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectPreviousLine)) {
        int pos = m_cursorPos - (2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectNextLine)) {
        int pos = m_cursorPos + (2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }

    if(event->matches(QKeySequence::SelectNextPage)) {
        int pos = m_cursorPos + (((viewport()->height() / m_charHeight) - 1) * 2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectPreviousPage)) {
        int pos = m_cursorPos - (((viewport()->height() / m_charHeight) - 1) * 2 * m_bytesPerLine);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectEndOfDocument)) {
        int pos = 0;
        if(m_data)
            pos = m_data->size() * 2;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::SelectStartOfDocument)) {
        int pos = 0;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }

    /*****************************************************************************/
    /* Other commands */
    /*****************************************************************************/

    if(event->matches(QKeySequence::Copy)) {
        if(m_data) {
            QString res;
            int idx = 0;
            int copyOffset = 0;

            QByteArray data = m_data->getData(m_selectBegin / 2, (m_selectEnd - m_selectBegin) / 2 + 1);
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
            clipboard->setText(res);
        }
    }

    if(event->key() == Qt::Key_Tab) { // Toggle window
        switch(m_window) {
            case WINDOW_HEX:
                m_window = WINDOW_ASCII;
                break;
            case WINDOW_ASCII:
                m_window = WINDOW_HEX;
                break;
        }
    }

    if(m_data != nullptr && int64_t(m_cursorPos) >= m_data->size() * 2) m_cursorPos = m_data->size() * 2 - 2; // FIXME: Crutch

    if(event->key() == Qt::Key_Insert) {
        switch(m_mode) {
            case MODE_READONLY:
                break;
            case MODE_WRITE_INSERT:
                m_mode = MODE_WRITE_REPLACE;
                break;
            case MODE_WRITE_REPLACE:
                m_mode = MODE_WRITE_INSERT;
                break;
        }
    }

    if(!event->text().isEmpty()) {
        if(event->text() == "\r")           return; // Enter
        else if(event->text() == "\t")      return; // Tab
        else if(event->text() == "\x1b")    return; // Esc
        else if(event->text() == "") { // Del
            if(m_mode != MODE_READONLY) {
                m_undoStack->push(new UndoDelete(m_data, m_data->at(m_cursorPos / 2), m_cursorPos / 2));
                //m_data->remove(m_cursorPos / 2);
            }
            update();
            return;
        } else if(event->text() == "\b") {
            if(m_mode != MODE_READONLY && m_cursorPos / 2) {
                m_undoStack->push(new UndoDelete(m_data, m_data->at(m_cursorPos / 2 - 1), m_cursorPos / 2 - 1));
                //m_data->remove(m_cursorPos / 2 - 1);
                setCursorPos(m_cursorPos - 2);
            }
            update();
            return;
        } // Backspace
        if(QApplication::keyboardModifiers() & Qt::ControlModifier) return;

        qDebug() << "Letter:" << event->text();
        inputSymbol(QChar(event->text()[0]));
    }

    if(setVisible)
        ensureVisible();
    update();
}

void QHexView::keyReleaseEvent(QKeyEvent *event) {
    statusBarUpdate();
}

void QHexView::statusBarUpdate() {
    if(!m_data) return;

    QString mode;

    switch(m_mode) {
        case MODE_READONLY:
            mode = tr("[READONLY] ");
            break;
        case MODE_WRITE_INSERT:
            mode = tr("[INSERT] ");
            break;
        case MODE_WRITE_REPLACE:
            mode = tr("[REPLACE] ");
            break;
    }

    if(m_selectBegin == m_selectEnd) {
        m_statusBar->showMessage(mode + tr("Offset: ") + QString::number(m_cursorPos));
    } else {
        m_statusBar->showMessage(mode + tr("Selection: from ") + QString::number(m_selectBegin) + tr(" to ") + QString::number(m_selectEnd) + tr(" total: ") + QString::number(m_selectEnd - m_selectBegin));
    }
}

void QHexView::inputSymbol(QChar symbol) {
    if(m_mode == MODE_READONLY) return;
    if(!symbol.toLatin1()) return;
    if(!m_data->size()) m_cursorPos = 0; // FIXME: Crutch

    qDebug() << "inputSymbol(" << symbol << ")";

    int32_t value = 0;

    if(m_window == WINDOW_HEX) { // Hex
        value = QString(symbol).toInt(nullptr, 16);
    } else if(m_window == WINDOW_ASCII) { // ASCII
        switch(m_mode) {
            case MODE_READONLY:
                qDebug() << "MODE_READONLY";
                return;
            case MODE_WRITE_INSERT:
                qDebug() << "MODE_INSERTED";
                m_undoStack->push(new UndoInsert(m_data, symbol.toLatin1(), m_cursorPos / 2));
                //m_data->insert(int32_t(m_cursorPos / 2), symbol.toLatin1());
                setCursorPos(m_cursorPos + 2);
                break;
            case MODE_WRITE_REPLACE:
                qDebug() << "MODE_REPLACED";
                m_undoStack->push(new UndoReplace(m_data, m_data->at(m_cursorPos / 2), symbol.toLatin1(), m_cursorPos / 2));
                //m_data->replace(int32_t(m_cursorPos / 2), symbol.toLatin1());
                break;
        }
    }

    qDebug() << "Value =" << value;
}

void QHexView::mouseMoveEvent(QMouseEvent *event) {
    int actPos = int32_t(getCursorPos(event->pos()));
    setCursorPos(actPos);
    setSelection(actPos);

    viewport()->update();
}

void QHexView::mousePressEvent(QMouseEvent *event) {
    int cPos = int(getCursorPos(event->pos()));

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) && event->button() == Qt::LeftButton)
        setSelection(cPos);
    else
        resetSelection(cPos);

    setCursorPos(cPos);

    viewport()->update();
    statusBarUpdate();
}

uint64_t QHexView::getCursorPos(const QPoint &position) {
    if(!m_data) return 0;
    uint64_t pos = 0;

    if((position.x() >= m_posHex) && (position.x() < m_posAscii - m_charWidth)) { // HEX window
        m_window = WINDOW_HEX;

        int x = (position.x() - m_posHex) / m_charWidth;
        if((x % 3) == 0)
            x = (x / 3) * 2;
        else
            x = ((x / 3) * 2) + 1;

        int firstLineIdx = verticalScrollBar()->value();
        int y = (position.y() / m_charHeight) * 2 * m_bytesPerLine;
        pos = uint64_t(x + y + firstLineIdx * m_bytesPerLine * 2);
    } else if(position.x() > m_posAscii) { // ASCII window
        m_window = WINDOW_ASCII;

        int x = (position.x() - m_posAscii) / m_charWidth;

        int firstLineIdx = verticalScrollBar()->value();
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
    if(pos < 0)
        pos = 0;

    m_selectInit    = uint64_t(pos);
    m_selectBegin   = uint64_t(pos);
    m_selectEnd     = uint64_t(pos);
}

void QHexView::setSelection(int pos) {
    if(pos < 0)
        pos = 0;

    if(uint64_t(pos) >= m_selectInit) {
        m_selectEnd = uint16_t(pos);
        m_selectBegin = m_selectInit;
    } else {
        m_selectBegin = uint16_t(pos);
        m_selectEnd = m_selectInit;
    }
}

void QHexView::setCursorPos(int64_t tmpposition = 0) {
    uint64_t position = uint64_t(tmpposition < 0 ? 0 : tmpposition);

    uint64_t maxPos = 0;
    if(m_data && m_data->size() && m_bytesPerLine) {
        maxPos = m_data->size() * 2 - 2;
        if(m_data->size() % m_bytesPerLine)
            maxPos++;
    }

    if(position > maxPos)
        position = maxPos;

    m_cursorPos = uint64_t(position);
    if(m_data && m_data->size()) emit valueChanged(uint8_t(m_data->at(m_cursorPos / 2)));
}

void QHexView::ensureVisible() {
    if(!m_data) return;
    QSize areaSize = viewport()->size();

    int firstLineIdx = verticalScrollBar()->value();
    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;

    int cursorY = int(m_cursorPos / (2 * m_bytesPerLine));

    if(cursorY < firstLineIdx)
        verticalScrollBar()->setValue(cursorY);
    else if(cursorY >= lastLineIdx)
        verticalScrollBar()->setValue(cursorY - areaSize.height() / m_charHeight + 1);
}
