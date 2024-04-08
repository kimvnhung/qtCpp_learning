#ifndef RULELINE_H
#define RULELINE_H

#include <QObject>

class RuleLine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(RuleLineType type READ type WRITE setType NOTIFY typeChanged FINAL)
    Q_PROPERTY(double position READ position WRITE setPosition NOTIFY positionChanged FINAL)
public:
    enum class RuleLineType : quint8
    {
        HIGHEST,
        NORMAL,
        SMALL,
        SMALLEST,
        UNDEFINED
    };
    Q_ENUM(RuleLineType)

    explicit RuleLine(QObject *parent = nullptr,
                      RuleLineType type = RuleLineType::UNDEFINED,
                      std::chrono::milliseconds value = std::chrono::milliseconds(0)
                    );



    QString text() const;
    void setText(const QString &newText);

    RuleLineType type() const;
    void setType(RuleLineType type);

    double position() const;
    void setPosition(double pos);

signals:
    void textChanged();
    void typeChanged();
    void positionChanged();

private:
    RuleLineType m_type;
    QString m_text;
    std::chrono::milliseconds m_value; // unit in milisecond
    double m_position;
};

#endif // RULELINE_H
