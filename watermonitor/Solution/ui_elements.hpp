#pragma once

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QEvent>
#include "dataHandler.hpp"


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
class DataHandler;


QWidget* createHeading(QString titleText, int fontSize);
QWidget* createCard(const QString& title, QStackedWidget* pageStack, int targetIndex, QWidget* parent, QWidget* graph, DataHandler* dataHandler);
QWidget* createNavButton(const QString& title, QStackedWidget* pageStack, int targetIndex, QWidget* parent = nullptr);
QWidget* createSidePanel();
QFrame* createFrame();
QWidget* createCard();
QWidget* createSearchBar();
QWidget* createTitleHeader();
QWidget* createNavigationBar(QStackedWidget* pageStack, QWidget* parent);
QWidget* createDashboardCards(QStackedWidget* pageStack, QWidget* parent, DataHandler* dataHandler, QWidget* pollutantsOverviewChart, QWidget* complianceDashboardChart, QWidget* fluorinatedCompoundsChart, QWidget* environmentalLitterIndicatorsChart, QWidget* popChart, QWidget* geographicalHotspotsChart);
QWidget* createParagraph(QString text);
void onCheckBoxClicked();

class NavigationBar : public QWidget {
  Q_OBJECT

  public:
    void createNavButton();

  private:
    QStackedWidget* pageStack;
}; 



class CreateFilters : public QWidget {
    Q_OBJECT

public:
    explicit CreateFilters(QWidget* parent = nullptr);
    void addCustomButton(QString buttonText);
    void addTimePeriodButtons();
    QWidget* getFiltersFrame();
    void changeToHorizontalLayout();

signals:
    void filterSelected(QString filterType);
    void filterChanged(const QString &filterName);

private slots:
    void handleButtonClick(QAbstractButton* button);

private:
    QFrame* filtersFrame;
    QButtonGroup* buttonGroup;

};


class ComplianceIndicator : public QWidget {
    Q_OBJECT

public:
    ComplianceIndicator(double safeEnd, double moderateEnd, double dangerEnd, double initialValue = 0, QWidget* parent = nullptr);

    void ubdateIndicator(double newSafeEnd, double newModerateEnd, double newDangerEnd, double neweValue);
    

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double safeEnd;       
    double moderateEnd;   
    double dangerEnd;     
    double value;
};


// Factory Function Declaration
QWidget* createComplianceIndicator(double safeBound, double moderateBound, double dangerBound, double currentValue, QWidget* parent = nullptr);

void showPopup(QWidget* parent, const QString& title, const QString& message, bool state);
void attachTooltip(QWidget* widget, QWidget* parent, const QString& title, const QString& message);

class TooltipEventFilter : public QObject {
    Q_OBJECT
public:
    TooltipEventFilter(QWidget* parent, const QString& title, const QString& message)
        : parentWidget(parent), tooltipTitle(title), tooltipMessage(message) {}

protected:

    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::HoverEnter) {
            showPopup(parentWidget, tooltipTitle, tooltipMessage, true);
        } else if (event->type() == QEvent::HoverLeave) {
            showPopup(parentWidget, "", "", false);
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QWidget* parentWidget;
    QString tooltipTitle;
    QString tooltipMessage;
};
