#ifndef Q_YAML_PARSE_H
#define Q_YAML_PARSE_H

#include <QFile>
#include <QString>
#include <QTextStream>

#include "node.h"

namespace YAML {

YAML_CPP_API Node
Load(const QString& input);

YAML_CPP_API Node
Load(const QByteArray& input);

YAML_CPP_API Node
LoadFile(const QString& filename);

YAML_CPP_API Node
LoadFile(QFile& file);

} // end of namespace YAML

#endif // Q_YAML_PARSE_H
