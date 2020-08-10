#ifndef Q_YAML_PARSE_H
#define Q_YAML_PARSE_H

#include <QFile>
#include <QString>
#include <QTextStream>

#include "node.h"

namespace YAML {

/**
   Loads the input QString as a single YAML document.

   @throws {@link ParserException} if it is malformed.
*/
Node Load(const QString& input)
{
   return Load(input.toStdString());
}

/**
   Loads the input QString as a single YAML document.

   @throws {@link ParserException} if it is malformed.
*/
Node Load(const QByteArray& input)
{
   return Load(input.toStdString());
}

/*!
    \brief YAML::LoadFile extension for QString
*/
Node LoadFile(const QString& filename)
{
   return LoadFile(filename.toStdString());
}

/*!
    \brief YAML::LoadFile extension for QString
*/
Node LoadFile(QFile& file)
{
   if (!file.exists()) {
      return Node();
   }

   if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return Node();
   }

   QTextStream istream(&file);
   QString data = istream.readAll();
   return Load(data.toStdString());
}

}
#endif // Q_YAML_PARSE_H
