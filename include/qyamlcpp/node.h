/*
   Copyright 2013-2020 Simon Meaden

   Permission is hereby granted, free of charge, to any person obtaining a copy
                                                of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                                                          copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    @author: Simon Meaden

*/
#ifndef NODE_H
#define NODE_H

#include <QBuffer>
#include <QByteArray>
#include <QColor>
#include <QFile>
#include <QFont>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QSizeF>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <string>
#include <yaml-cpp/yaml.h>

namespace YAML {

/* = QVariant
   =========================================================================================*/
template<>
struct convert<QVariant>
{
   static Node encode(const QVariant& rhs) {
      Node node;
      node = rhs;
      return node;
   }

   static bool decode(const Node& node, QVariant& rhs) {
      if (!node.IsScalar()) {
         return false;
      }

      //      std::string sstr = node
      rhs = QVariant(node);

      return true;
   }
};

void operator>>(const Node node, QVariant& q)
{
   std::string sstr;
   sstr = node.as<std::string>();
   q = QString(sstr.c_str());
}


void operator<<(Node node, const QVariant& q)
{
   std::string sstr = q.toString().toStdString();
   node = sstr;
}


/* = QString
   =========================================================================================*/
template<>
struct convert<QString>
{
   static Node encode(const QString& rhs) {
      Node node;
      node = rhs.toStdString();
      return node;
   }

   static bool decode(const Node& node, QString& rhs) {
      if (!node.IsScalar()) {
         return false;
      }

      std::string sstr = node.as<std::string>();
      rhs = QString(sstr.c_str());

      return true;
   }
};

void operator>>(const Node node, QString& q)
{
   std::string sstr;
   sstr = node.as<std::string>();
   q = QString(sstr.c_str());
}


void operator<<(Node node, const QString& q)
{
   std::string sstr = q.toStdString();
   node = sstr;
}

/* = QByteArray
   ======================================================================================*/
template<>
struct convert<QByteArray>
{
   static Node encode(const QByteArray& rhs) {
      Node node;
      const char* data = rhs.constData();
      YAML::Binary binary(reinterpret_cast<const unsigned char*>(data),
                          size_t(rhs.size()));
      node = binary;
      return node;
   }

   static bool decode(const Node& node, QByteArray& rhs) {
      if (!node.IsScalar()) {
         return false;
      }

      YAML::Binary binary = node.as<YAML::Binary>();
      const char* data = reinterpret_cast<const char*>(binary.data());
      int size = int(binary.size());
      rhs = QByteArray(data, size);

      return true;
   }
};

void operator>>(const Node node, QByteArray& q)
{
   YAML::Binary binary = node.as<YAML::Binary>();
   const char* data = reinterpret_cast<const char*>(binary.data());
   int size = int(binary.size());
   q = QByteArray(data, size);
}

void operator<<(Node node, const QByteArray& q)
{
   const char* data = q.constData();
   YAML::Binary binary(reinterpret_cast<const unsigned char*>(data),
                       size_t(q.size()));
   node = binary;
}


/* = QBuffer
   ======================================================================================*/
template<>
struct convert<QBuffer>
{
   static Node encode(const QBuffer& rhs) {
      Node node;
      QByteArray data = rhs.buffer();
      node = data;
      return node;
   }

   static bool decode(const Node& node, QBuffer& rhs) {
      if (!node.IsScalar()) {
         return false;
      }

      QByteArray array = node.as<QByteArray>();
      rhs.setBuffer(&array);

      return true;
   }
};

void operator>>(const Node node, QBuffer& q)
{
   QByteArray array = node.as<QByteArray>();
   q.setBuffer(&array);
}


void operator<<(Node node, const QBuffer& q)
{
   QByteArray data = q.buffer();
   node = data;
}


/* = QColor
   ==========================================================================================*/
/*
   Converts QColor to Node and back. Enables QColor to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QColor>
{
   static Node encode(const QColor& rhs) {
      Node node;
      node["red"] = rhs.red();
      node["green"] = rhs.green();
      node["blue"] = rhs.blue();
      node["alpha"] = rhs.alpha();
      return node;
   }

   static bool decode(const Node& node, QColor& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      int red = node["red"].as<int>();
      int green = node["green"].as<int>();
      int blue = node["blue"].as<int>();
      int alpha = node["alpha"].as<int>();
      rhs = QColor(red, green, blue, alpha);

      return true;
   }
};

void operator>>(const Node node, QColor& q)
{
   int red = node["red"].as<int>();
   int green = node["green"].as<int>();
   int blue = node["blue"].as<int>();
   int alpha = node["alpha"].as<int>();
   q = QColor(red, green, blue, alpha);
}

void operator<<(Node node, const QColor& q)
{
   node["red"] = q.red();
   node["green"] = q.green();
   node["blue"] = q.blue();
   node["alpha"] = q.alpha();
}



/* = QFont
   ===========================================================================================*/
/*
   Converts QFont to Node and back. Enables QFont to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QFont>
{
   static Node encode(const QFont& rhs) {
      Node node;
      node["family"] = rhs.family();
      node["bold"] = rhs.bold();
      node["capitalization"] = int(rhs.capitalization());
      node["fixedpitch"] = rhs.fixedPitch();
      node["hinting preference"] = int(rhs.hintingPreference());
      node["italic"] = rhs.italic();
      node["kerning"] = rhs.kerning();
      node["letter spacing"] = rhs.letterSpacing();
      node["letter spacing type"] = int(rhs.letterSpacingType());
      node["overline"] = rhs.overline();
      // not recommended to use pixelSize()
      node["point size"] = rhs.pointSize();
      node["stretch"] = rhs.stretch();
      node["strikeout"] = rhs.strikeOut();
      node["style"] = int(rhs.style());
      node["style hint"] = int(rhs.styleHint());
      node["style name"] = rhs.styleName();
      node["style strategy"] = int(rhs.styleStrategy());
      node["underline"] = rhs.underline();
      node["weight"] = int(rhs.weight());
      node["word spacing"] = rhs.wordSpacing();

      return node;
   }

   static bool decode(const Node& node, QFont& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      QFont font;
      rhs.setFamily(node["family"].as<QString>());
      rhs.setBold(node["bold"].as<bool>());
      rhs.setCapitalization(
         QFont::Capitalization(node["capitalization"].as<int>()));
      rhs.setFixedPitch(node["fixedpitch"].as<bool>());
      rhs.setHintingPreference(
         QFont::HintingPreference(node["hinting preference"].as<int>()));
      rhs.setItalic(node["italic"].as<bool>());
      rhs.setKerning(node["kerning"].as<bool>());
      rhs.setLetterSpacing(
         QFont::SpacingType(node["letter spacing type"].as<int>()),
         node["letter spacing"].as<double>());
      rhs.setOverline(node["overline"].as<bool>());
      // not recommended to use pixelSize()
      rhs.setPointSize(node["point size"].as<int>());
      rhs.setStretch(node["stretch"].as<int>());
      rhs.setStrikeOut(node["strikeout"].as<bool>());
      rhs.setStyle(QFont::Style(node["style"].as<int>()));
      rhs.setStyleHint(QFont::StyleHint(node["style hint"].as<int>()));
      rhs.setStyleName(node["style name"].as<QString>());
      rhs.setStyleStrategy(
         QFont::StyleStrategy(node["style strategy"].as<int>()));
      rhs.setUnderline(node["underline"].as<bool>());
      rhs.setWeight(QFont::Weight(node["weight"].as<int>()));
      rhs.setWordSpacing(node["word spacing"].as<int>());

      return true;
   }
};

void operator>>(const Node node, QFont& q)
{
   q.setFamily(node["family"].as<QString>());
   q.setBold(node["bold"].as<bool>());
   q.setCapitalization(QFont::Capitalization(node["capitalization"].as<int>()));
   q.setFixedPitch(node["fixedpitch"].as<bool>());
   q.setHintingPreference(
      QFont::HintingPreference(node["hinting preference"].as<int>()));
   q.setItalic(node["italic"].as<bool>());
   q.setKerning(node["kerning"].as<bool>());
   q.setLetterSpacing(QFont::SpacingType(node["letter spacing type"].as<int>()),
                      node["letter spacing"].as<double>());
   q.setOverline(node["overline"].as<bool>());
   // not recommended to use pixelSize()
   q.setPointSize(node["point size"].as<int>());
   q.setStretch(node["stretch"].as<int>());
   q.setStrikeOut(node["strikeout"].as<bool>());
   q.setStyle(QFont::Style(node["style"].as<int>()));
   q.setStyleHint(QFont::StyleHint(node["style hint"].as<int>()));
   q.setStyleName(node["style name"].as<QString>());
   q.setStyleStrategy(QFont::StyleStrategy(node["style strategy"].as<int>()));
   q.setUnderline(node["underline"].as<bool>());
   q.setWeight(QFont::Weight(node["weight"].as<int>()));
   q.setWordSpacing(node["word spacing"].as<int>());
}
void operator<<(Node node, const QFont& q)
{
   node["family"] = q.family();
   node["bold"] = q.bold();
   node["capitalization"] = int(q.capitalization());
   node["fixedpitch"] = q.fixedPitch();
   node["hinting preference"] = int(q.hintingPreference());
   node["italic"] = q.italic();
   node["kerning"] = q.kerning();
   node["letter spacing"] = q.letterSpacing();
   node["letter spacing type"] = int(q.letterSpacingType());
   node["overline"] = q.overline();
   // not recommended to use pixelSize()
   node["point size"] = q.pointSize();
   node["stretch"] = q.stretch();
   node["strikeout"] = q.strikeOut();
   node["style"] = int(q.style());
   node["style hint"] = int(q.styleHint());
   node["style name"] = q.styleName();
   node["style strategy"] = int(q.styleStrategy());
   node["underline"] = q.underline();
   node["weight"] = int(q.weight());
   node["word spacing"] = q.wordSpacing();
}

/* = QPoint
   =====================================================================================*/
/*
   Converts QPoint to Node and back. Enables QPoint to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QPoint>
{
   static Node encode(const QPoint& rhs) {
      Node node;
      node["x"] = rhs.x();
      node["y"] = rhs.y();
      return node;
   }

   static bool decode(const Node& node, QPoint& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      int x = node["x"].as<int>();
      int y = node["y"].as<int>();
      rhs = QPoint(x, y);

      return true;
   }
};

void operator>>(const Node node, QPoint& q)
{
   int x = node["x"].as<int>();
   int y = node["y"].as<int>();
   q = QPoint(x, y);
}

void operator<<(Node node, const QPoint& q)
{
   node["x"] = q.x();
   node["y"] = q.y();
}


/* = QPointF
   =====================================================================================*/
/*
   Converts QPointF to Node and back. Enables QPointF to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QPointF>
{
   static Node encode(const QPointF& rhs) {
      Node node;
      node["x"] = rhs.x();
      node["y"] = rhs.y();
      return node;
   }

   static bool decode(const Node& node, QPointF& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      qreal x = node["x"].as<qreal>();
      qreal y = node["y"].as<qreal>();
      rhs = QPointF(x, y);

      return true;
   }
};

void operator>>(const Node& node, QPointF& q)
{
   qreal x = node["x"].as<qreal>();
   qreal y = node["y"].as<qreal>();
   q = QPointF(x, y);
}

void operator<<(Node& node, const QPointF& q)
{
   node["x"] = q.x();
   node["y"] = q.y();
}


/* = QRect
   ======================================================================================*/
/*
   Converts QRect to Node and back. Enables QRect to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QRect>
{
   static Node encode(const QRect& rhs) {
      Node node;
      node["left"] = rhs.left();
      node["top"] = rhs.top();
      node["width"] = rhs.width();
      node["height"] = rhs.height();
      return node;
   }

   static bool decode(const Node& node, QRect& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      int left = node["left"].as<int>();
      int top = node["top"].as<int>();
      int width = node["width"].as<int>();
      int height = node["height"].as<int>();
      rhs = QRect(left, top, width, height);

      return true;
   }
};

void operator>>(const Node node, QRect& q)
{
   int left = node["left"].as<int>();
   int top = node["top"].as<int>();
   int width = node["width"].as<int>();
   int height = node["height"].as<int>();
   q = QRect(left, top, width, height);
}

void operator<<(Node node, const QRect& q)
{
   node["left"] = q.left();
   node["top"] = q.top();
   node["width"] = q.width();
   node["height"] = q.height();
}


/* = QRectF
   ======================================================================================*/
/*
   Converts QRectF to Node and back. Enables QRectF to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QRectF>
{
   static Node encode(const QRectF& rhs) {
      Node node;
      node["left"] = rhs.left();
      node["top"] = rhs.top();
      node["width"] = rhs.width();
      node["height"] = rhs.height();
      return node;
   }

   static bool decode(const Node& node, QRectF& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      qreal left = node["left"].as<qreal>();
      qreal top = node["top"].as<qreal>();
      qreal width = node["width"].as<qreal>();
      qreal height = node["height"].as<qreal>();
      rhs = QRectF(left, top, width, height);

      return true;
   }
};

void operator>>(const Node node, QRectF& q)
{
   qreal left = node["left"].as<qreal>();
   qreal top = node["top"].as<qreal>();
   qreal width = node["width"].as<qreal>();
   qreal height = node["height"].as<qreal>();
   q = QRectF(left, top, width, height);
}

void operator<<(Node node, const QRectF& q)
{
   node["left"] = q.left();
   node["top"] = q.top();
   node["width"] = q.width();
   node["height"] = q.height();
}


/* = QSize
   ======================================================================================*/
/*
   Converts QSize to Node and back. Enables QSize to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QSize>
{
   static Node encode(const QSize& rhs) {
      Node node;
      node["width"] = rhs.width();
      node["height"] = rhs.height();
      return node;
   }

   static bool decode(const Node& node, QSize& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      int width = node["width"].as<int>();
      int height = node["height"].as<int>();
      rhs = QSize(width, height);

      return true;
   }
};

void operator>>(const Node node, QSize& q)
{
   int width = node["width"].as<int>();
   int height = node["height"].as<int>();
   q = QSize(width, height);
}

void operator<<(Node node, const QSize& q)
{
   node["width"] = q.width();
   node["height"] = q.height();
}


/* = QSizeF
   ======================================================================================*/
/*
   Converts QSizeF to Node and back. Enables QSizeF to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QSizeF>
{
   static Node encode(const QSizeF& rhs) {
      Node node;
      node["width"] = rhs.width();
      node["height"] = rhs.height();
      return node;
   }

   static bool decode(const Node& node, QSizeF& rhs) {
      if (!node.IsMap()) {
         return false;
      }

      qreal width = node["width"].as<qreal>();
      qreal height = node["height"].as<qreal>();
      rhs = QSizeF(width, height);

      return true;
   }
};

void operator>>(const Node node, QSizeF& q)
{
   qreal width = node["width"].as<qreal>();
   qreal height = node["height"].as<qreal>();
   q = QSizeF(width, height);
}

void operator<<(Node node, const QSizeF& q)
{
   node["width"] = q.width();
   node["height"] = q.height();
}


/* = QPixmap
   ======================================================================================*/
/*
   Converts QPixmap to Node and back. Enables QPixmap to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QPixmap>
{
   static Node encode(const QPixmap& rhs) {
      Node node;
      QByteArray array;
      QBuffer buffer(&array);
      buffer.open(QIODevice::WriteOnly);
      rhs.save(&buffer, "PNG");
      node = array;
      return node;
   }

   static bool decode(const Node& node, QPixmap& rhs) {
      if (!node.IsScalar()) {
         return false;
      }

      YAML::Binary binary = node.as<YAML::Binary>();
      const unsigned char* data = binary.data();
      std::size_t size = binary.size();
      QPixmap pixmap;
      bool res = pixmap.loadFromData(data, uint(size));

      if (res) {
         rhs = pixmap;
      }

      return res;
   }
};

void operator>>(const Node node, QPixmap& q)
{
   YAML::Binary binary = node.as<YAML::Binary>();
   const char* data = reinterpret_cast<const char*>(binary.data());
   int size = int(binary.size());
   const QByteArray array(data, size);
   QPixmap pixmap;
   bool res = pixmap.loadFromData(array, "PNG");

   if (res) {
      q = pixmap;
   }
}

void operator<<(Node node, const QPixmap& q)
{
   QByteArray array;
   QBuffer buffer(&array);
   buffer.open(QIODevice::WriteOnly);
   q.save(&buffer, "PNG");
   node = array;
}


/* = QImage
   ======================================================================================*/
/*
   Converts QImage to Node and back. Enables QImage to be sent/received from a
   YAML file via yaml-cpp.
*/
template<>
struct convert<QImage>
{
   static Node encode(const QImage& rhs) {
      Node node;
      QPixmap pixmap = QPixmap::fromImage(rhs);
      QByteArray array;
      QBuffer buffer(&array);
      buffer.open(QIODevice::WriteOnly);
      pixmap.save(&buffer, "PNG");
      node = array;
      return node;
   }

   static bool decode(const Node& node, QImage& rhs) {
      if (!node.IsScalar()) {
         return false;
      }

      QPixmap pixmap = node.as<QPixmap>();
      rhs = pixmap.toImage();
      return rhs.isNull();
   }
};

void operator>>(const Node node, QImage& q){
  QPixmap pixmap = node.as<QPixmap>();
  q = pixmap.toImage();
}

void operator<<(Node node, const QImage& q){
  QPixmap pixmap = QPixmap::fromImage(q);
  node = pixmap;
}


} // end of namespace YAML

#endif // NODE_H
