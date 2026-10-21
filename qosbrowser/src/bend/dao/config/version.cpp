#include "version.h"


QString Version::version() const
{
    return QString("%1-%2-%3.%4.%5").arg(m_major).arg(m_dev).arg(m_v1).arg(m_v2).arg(m_v3);
}

QString Version::versionNum() const
{
    return QString("%1.%2.%3").arg(m_v1).arg(m_v2).arg(m_v3);
}

QString Version::major() const
{
    return m_major;
}
