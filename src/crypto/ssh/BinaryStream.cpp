/*
 *  Copyright (C) 2017 Toni Spets <toni.spets@iki.fi>
 *  Copyright (C) 2017 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BinaryStream.h"
#include <QDataStream>
#include <QtEndian>

BinaryStream::BinaryStream(QIODevice* device)
    : QObject(device)
    , m_stream(new QDataStream(device))
{
    m_stream->setByteOrder(QDataStream::BigEndian);
}

BinaryStream::BinaryStream(QByteArray* ba, QObject* parent)
    : QObject(parent)
    , m_stream(new QDataStream(ba, QIODevice::ReadWrite))
{
    m_stream->setByteOrder(QDataStream::BigEndian);
}

const QString BinaryStream::errorString() const
{
    return m_stream->device()->errorString();
}

bool BinaryStream::read(QByteArray& ba)
{
    return m_stream->readRawData(ba.data(), ba.size()) != -1;
}

bool BinaryStream::read(quint32& i)
{
    *m_stream >> i;
    return true;
}

bool BinaryStream::read(quint16& i)
{
    *m_stream >> i;
    return true;
}

bool BinaryStream::read(quint8& i)
{
    *m_stream >> i;
    return true;
}

bool BinaryStream::readString(QByteArray& ba)
{
    quint32 length;
    *m_stream >> length;

    ba.resize(length);
    return m_stream->readRawData(ba.data(), length) != -1;
}

bool BinaryStream::readString(QString& str)
{
    QByteArray ba;

    if (!readString(ba)) {
        return false;
    }

    str = QString::fromLatin1(ba);
    return true;
}

bool BinaryStream::flush()
{
    if (!m_stream->device()->waitForBytesWritten(3000)) {
        return false;
    }

    return true;
}

bool BinaryStream::write(const QByteArray& ba)
{
    return m_stream->writeRawData(ba.constData(), ba.size()) != -1;
}

bool BinaryStream::write(quint32 i)
{
    *m_stream << i;
    return true;
}

bool BinaryStream::write(quint16 i)
{
    *m_stream << i;
    return true;
}

bool BinaryStream::write(quint8 i)
{
    *m_stream << i;
    return true;
}

bool BinaryStream::writeString(const QByteArray& ba)
{
    m_stream->writeBytes(ba.data(), ba.size());
    return true;
}

bool BinaryStream::writeString(const QString& s)
{
    return writeString(s.toLatin1());
}
