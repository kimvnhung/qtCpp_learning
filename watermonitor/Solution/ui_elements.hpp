#pragma once

#include <QWidget>

const int FONT_SIZE = 14;
const int TITLE_SIZE = 22;
const int SUBHEADING_SIZE = 18;

class QFrame;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QFont;
class QRect;
class QButtonGroup;
class QCheckBox;
class QFrame;
class QWidget;
class CreateFilters;
class QAbstractButton;

QWidget *createHeading(QString titleText, int fontSize);
QWidget *createSidePanel();
QFrame *createFrame();
QWidget *createCard();
QWidget *createSearchBar();
QWidget *createNavigationBar();
QWidget *createNavigationButton();
QWidget *createTitleHeader();
QWidget *createParagraph(QString text);
void onCheckBoxClicked();

class CreateFilters : public QWidget
{
    Q_OBJECT

public:
    explicit CreateFilters(QWidget *parent = nullptr);
    void addCustomButton(QString buttonText);
    void addTimePeriodButtons();
    QWidget *getFiltersFrame();
    void changeToHorizontalLayout();

signals:
    void filterChanged(const QString &filterName);

private slots:
    void handleButtonClick(QAbstractButton *button);

private:
    QFrame *filtersFrame;
    QButtonGroup *buttonGroup;
};

class ComplianceIndicator : public QWidget
{
    Q_OBJECT

public:
    ComplianceIndicator(double safeEnd, double moderateEnd, double dangerEnd, double initialValue = 0, QWidget *parent = nullptr);

    void ubdateIndicator(double newSafeEnd, double newModerateEnd, double newDangerEnd, double neweValue);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double safeEnd;
    double moderateEnd;
    double dangerEnd;
    double value;
};

// Factory Function Declaration
QWidget *createComplianceIndicator(double safeBound, double moderateBound, double dangerBound, double currentValue, QWidget *parent = nullptr);
