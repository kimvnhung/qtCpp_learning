#ifndef RULELINE_H
#define RULELINE_H

#include <QObject>

class RuleLine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(RuleLineType type READ type WRITE setType NOTIFY typeChanged FINAL)
public:
    enum RuleLineType{
        HIGHEST,
        NORMAL,
        SMALL,
        SMALLEST,
        UNDEFINED
    };
    Q_ENUM(RuleLineType)

    explicit RuleLine(QObject *parent = nullptr,
                      RuleLineType type = UNDEFINED,
                      const QString& text = "",
                      std::chrono::milliseconds pos = std::chrono::milliseconds(0)
                    );



    QString text() const;
    void setText(const QString &newText);

    RuleLineType type() const;
    void setType(RuleLineType type);

    std::chrono::milliseconds position() const;
    void setPosition(std::chrono::milliseconds pos);

signals:
    void textChanged();
    void typeChanged();
    void positionChanged();

private:
    RuleLineType m_type;
    QString m_text;
    std::chrono::milliseconds m_position; // unit in milisecond
};

#endif // RULELINE_H
