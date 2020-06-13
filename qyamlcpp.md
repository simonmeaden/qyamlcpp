QYamlCpp
========

QYamlCpp is a simple extension to YamlCpp that allows some (but not all) Qt classes to be used with the YamlCpp library.

Emitter has been extended so that you can use the
    emitter << object;
notation.

Load now takes QString and QByteArray.
LoadFile takes QString and QFile.
Node has also been extended so:

    QString filename("prefs.yaml");
    ...
    QFile file(filename);
    if (file.exists()) {
        // this works
        Node m_preferences = YAML::LoadFile(file);
        // this also works
        QString username = m_preferences["username"].as<QString>();
        QRect rect = m_preferences["position"].as<QRect>();
        QColor normal_color = codenormal["normal_color"].as<QColor>();
     }

will also work.



Things to note:
---------------
These are probably not the best way to accomodate these types. I used the existing code of YamlCpp to work out a way that worked.

I needed to use each of these at some time or other in a yaml file so wrote these add-ons to accomodate my need. Hence the limited list.

Collections
-----------
QMap
QVector
QSet
QList

Other
-----
QString
QStringList
QVariant
QByteArray
QBuffer
QPixmap
QColor
QFont
QPoint
QPointF
QRect
QRectF
QSize
QSizeF

License
=======
This is released under the MIT license as it is the most open, pretty much.
Use it as you will, just don't blame me for the global pandemic or the fact
that your boat has sunk.
