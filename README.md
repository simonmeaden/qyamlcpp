QYamlCpp
======

qyaml is a small collection of YAML:: extensions that allow some
Qt types to be used directly with yaml-cpp. Notably QString,
QList, QMap and QVector.

This is by no means complete, it is just a few things that I needed
myself, but you are welcome to use them.

They were initially set up as a library but I just link in the qyaml.pri
file as they are quite small files.

There are nine files at present.

- qyaml-cpp.h     == This just wraps the other header files in one header file.
- parse.h
- collection.h
- emitter.h
- node.h
- comment.h
- QYamlCpp        == this is just a Qt style wrapper for qyaml-cpp.h

- parse.cpp
- node.cpp

These could probably be placed all in one file or a header/source pair but at some
point I might write some more for these so I left all of the files in.

So far the following Qt classes are covered:

- QString
  Creates a standard YAML string value.
- QColor
  Creates a standard YAML map of values.
- QFont
  Creates a standard YAML map of values.
- QByteArray
  Creates a standard YAML binary value which results in something like:
  ```image: !!binary "iVBORw0KGgoAAAANSUhEUgAA..."```
  Remember this can result in a very long line for a large image.
- QBuffer Creates a QByteArray internally.
- QList
  Creates a standard YAML list.
- QVector
  Creates a standard YAML list.
- QMap
  Creates a standard YAML map.
- QSet
  Creates a standard YAML list.
- QPoint, QPointF, QRect, QRectF, QSize & QSizeF
  Creates standard YAML maps of values.
- QPixmap saved as a PNG file in a QByteArray.
- QImage saved as a PNG QPixmap which is waht the Qt Serialise page suggests.
  If you want a different form such as BMP then the Qt suggested
  method is to create a QBuffer from the QImage then save that.
  The system below should work for any Qt supported format.
  
```cpp
  
// To write to the YAML file.
YAML::Emitter emitter;

QPixmap pixmap = QPixmap::fromImage(img);
QByteArray array;
QBuffer buffer(&array);
buffer.open(QIODevice::WriteOnly);
pixmap.save(&buffer, "BMP");

// You could alternatively save the QByteArray which is what I do internally.
emitter << buffer;

// To read from the YAML file.
QBuffer array = image_node.as<QBuffer>();
QImage img;
img.loadFromData(array.buffer(), "BMP");

```
      
Emitter << operator has been overloaded for all of these classes so 

```cpp

QFont font = QFont("Courier", 10);
...
YAML::Emitter emitter;
emitter << YAML::Key << "font";
emitter << YAML::Value << font;

```

should save to the yaml file correctly, and result in the following
text in the yaml file.

```
font:
  family: Courier 10 Pitch
  bold: false
  capitalization: 0
  fixedpitch: false
  hinting preference: 0
  italic: false
  kerning: true
  letter spacing: 0
  letter spacing type: 0
  overline: false
  point size: 10
  stretch: 0
  strikeout: false
  style: 0
  style hint: 5
  style name: ""
  style strategy: 1
  underline: false
  weight: 50
  word spacing: 0
```

Usage:
======

In the example below mRefSquareData is a `QMap<int, QMap<int, QString>>`, ie a two
level QMap system. Only three lines of code are required to save these to the yaml
document.

To create yaml document just:
```cpp
    YAML::Node root;                    // create root node.
    root["eastings"] = keys;            // add the list
    root["northings"] = mRefSquareData; // add the map
```
To read just:
```cpp
    YAML::Node root = YAML::LoadFile( filename );   // load from file
    QList<int> eastings;                            // create empty list.
    root["eastings"] >> eastings;                   // copy list from document
    QMap<int, QMap< int, QString>> northings;       // create empty map
    root["northings"] >> northings;                 // copy map from document
```
yaml-cpp will handle the multi-level map.

Example YAML file
=================
```yaml
    eastings:
        - 390000
        - 400000
        - 410000
        - 420000
        - 440000
        - 450000
        - 460000
    northings:
        390000:
            1130000: HT93
            1140000: HT94
        400000:
            1160000: HU06
        410000:
            1140000: HU14
            1150000: HU15
            1160000: HU16
        420000:
            1150000: HU25
            1140000: HU24
        440000:
            1200000: HP40
        450000:
            1200000: HP50
            1210000: HP51
        460000:
            1200000: HP60
            1210000: HP61
            1220000: HP62
```
Example Source Code
===================

Load File:
==========
```cpp
    #include <yaml-cpp/yaml.h>
    #include "yaml.h"

    QFile file( Settings::YamlRefSquareFile );

    if ( file.exists() ) {
        YAML::Node baseNode = YAML::LoadFile( Settings::YamlRefSquareFile );

        if ( baseNode.IsNull() )  // file not loaded so when we have data save it.
            bModified =  false;

        QList<int> eastings;
        baseNode["eastings"] >> eastings;
        QMap<int, QMap< int, QString>> northings;
        baseNode["northings"] >> northings;
        mRefSquareData = northings;

    }
```
