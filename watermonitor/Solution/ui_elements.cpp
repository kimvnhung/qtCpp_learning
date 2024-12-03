#include "ui_elements.hpp"
#include "interactive_card.hpp"
#include <QtWidgets>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QPalette>

// IMPORTANT NOTES:
// When creating new UI element functions, always wrap the element in a QFrame object
// using the createFrame function - this will help us see that space that the object
// takes up on the screen.

// Benefit of separating elements is that we can add QStyle to the base element, to ensure
// consistant design throughout the application. (Currently WIP)

// When adding a new element to a different page, include the header file, and then
// use <layoutName>->addWidget(<functionCall>()<optional arguments>) - all functions
// return a QWidget object.

// NEED TO WORK OUT HOW TO DO SCALEABLE UI ELEMENTS - need to decide how to handle different screen sizes
// NEED TO WORK OUT TO DO STYLISE WIDGETS
// As per last meeting - Joanna work on the cards, Kavisha work on the navigation bar, and Tahmid work on the search and filter bar.
//                       proposed 2 functions each, listed below for us to work on.
//                       After dashboard done, finish of the additional ui elements (bottom of file) and start populating info pages.

QFrame *createFrame()
{
    QFrame *frame = new QFrame();
    // Creates a border around the Frame - currently only used to show the space the
    // Widget takes up on the screen. Once layout finalised, we can set LineWidth to 0
    // to remove the frame border.
    frame->setFrameShape(QFrame::Box);
    return frame;
}

QWidget *createHeading(QString titleText, int fontSize)
{
    QFrame *titleBanner = createFrame();
    QLabel *titleLabel = new QLabel(titleText);
    titleLabel->setAlignment(Qt::AlignCenter);

    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(fontSize);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleBanner->setLayout(titleLayout);

    return titleBanner;
}

// FUNCTIONS TO CREATE - return types can be replaced with another object that inherits from QWidget

QWidget *createSearchBar()
{
    QFrame *searchBarFrame = createFrame();
    QVBoxLayout *searchBarLayout = new QVBoxLayout(searchBarFrame);
    QLineEdit *searchBarField = new QLineEdit();
    searchBarField->setPlaceholderText("Search...");
    searchBarLayout->addWidget(searchBarField);

    return searchBarFrame;
}

CreateFilters::CreateFilters(QWidget *parent) : QWidget(parent)
{
    filtersFrame = createFrame();
    QVBoxLayout *filtersLayout = new QVBoxLayout(filtersFrame);
    buttonGroup = new QButtonGroup();
    connect(buttonGroup, &QButtonGroup::buttonClicked, this, &CreateFilters::handleButtonClick);
}

void CreateFilters::addTimePeriodButtons()
{
    QCheckBox *pastMonth = new QCheckBox("Past 30 Days");
    QCheckBox *past3Months = new QCheckBox("Past 3 Months");
    QCheckBox *past6Months = new QCheckBox("Past 6 Months");
    QCheckBox *pastYearTillDate = new QCheckBox("Year Till Date");
    QCheckBox *pastYear = new QCheckBox("Past 1 Year");
    QCheckBox *allTime = new QCheckBox("All Time");

    buttonGroup->addButton(pastMonth);
    buttonGroup->addButton(past3Months);
    buttonGroup->addButton(past6Months);
    buttonGroup->addButton(pastYearTillDate);
    buttonGroup->addButton(pastYear);
    buttonGroup->addButton(allTime);

    filtersFrame->layout()->addWidget(pastMonth);
    filtersFrame->layout()->addWidget(past3Months);
    filtersFrame->layout()->addWidget(past6Months);
    filtersFrame->layout()->addWidget(pastYearTillDate);
    filtersFrame->layout()->addWidget(pastYear);
    filtersFrame->layout()->addWidget(allTime);
}

void CreateFilters::addCustomButton(QString buttonText)
{
    QCheckBox *customButton = new QCheckBox(buttonText);
    buttonGroup->addButton(customButton);
    filtersFrame->layout()->addWidget(customButton);
}

void CreateFilters::changeToHorizontalLayout()
{
    delete filtersFrame->layout();
    filtersFrame->setLayout(new QHBoxLayout());
}

void CreateFilters::handleButtonClick(QAbstractButton *button)
{
    filtersFrame->layout()->addWidget(createHeading(button->text(), TITLE_SIZE));
}

QWidget *CreateFilters::getFiltersFrame()
{
    return this->filtersFrame;
}

QWidget *createSidePanel()
{
    // Side Panel: search and filter
    QWidget *sidePanel = createFrame();
    QVBoxLayout *sidePanelLayout = new QVBoxLayout(sidePanel);

    // Change alignment that widget aren't in the middle of the layout.
    sidePanelLayout->setAlignment(Qt::AlignTop);

    // Makes the width of the panel fixed and length of the panel stretch
    // Unfixed width makes the search bar stretch wider than the width of
    // the layout.
    sidePanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    // Figured out how to do object specific style change - style sheet is
    // almost exactly the same as CSS
    sidePanel->setStyleSheet("QFrame { background-color: yellow}");

    return sidePanel;
}

QWidget *createCard(const QString &title, QObject *parent, std::function<void()> onClick)
{
    interactiveCard *card = new interactiveCard(title, qobject_cast<QWidget *>(parent));
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    QObject::connect(card, &interactiveCard::cardClicked, parent, onClick);
    return card;
}

QWidget *createButton()
{
    return new QWidget(); // TODO: Implement button creation
}

QWidget *createTitleHeader()
{
    QWidget *title = createHeading("Water Quality Monitor", TITLE_SIZE);
    title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    return title;
}

QWidget *createNavigationBar()
{
    // TODO: Implement navigation bar creation - since each page will have its own navigation bar with,
    // this function will just be used to create the basic layout for the bar, and within each of the
    // pages that will have their own navigation bar, the createButton function will be used to add
    // desired buttons to the bar.
    // i.e. bar = createNavigationBar(...);
    //      bar->addWidget(createNavigationButton(...));
    QWidget *temp = createHeading("<Add Navigation Bar Here>", TITLE_SIZE);
    temp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    return temp;
}

QWidget *createNavigationButton()
{
    // TODO: Implement button creation - different from standard button, as it will have a different
    // QStyle. Currently as we aren't adding QStyle this sprint, both buttoncreation function will
    // be the same.
    return new QWidget();
}

// OTHER FUNCTIONS TO BE ADDED - don't think we need this for dashboard but will do for information pages
// - createParagraph - DONE
// - createImage
// - createGraph
// - probs others I can't think of right now - if you do add it to list :p

QWidget *createParagraph(QString text)
{
    QTextEdit *paragraphContent = new QTextEdit(text);
    paragraphContent->setReadOnly(true);
    paragraphContent->setAlignment(Qt::AlignLeft);

    QFont font = paragraphContent->font();
    font.setPointSize(FONT_SIZE);
    paragraphContent->setFont(font);

    QVBoxLayout *paragraphLayout = new QVBoxLayout();
    paragraphLayout->addWidget(paragraphContent);
    paragraphLayout->setAlignment(Qt::AlignTop);

    return paragraphContent;
}

ComplianceIndicator::ComplianceIndicator(double safeEnd, double moderateEnd, double dangerEnd, double initialValue, QWidget *parent)
    : QWidget(parent), safeEnd(safeEnd), moderateEnd(moderateEnd), dangerEnd(dangerEnd), value(initialValue)
{
    // Ensure thresholds are in ascending order
    if (safeEnd >= moderateEnd || moderateEnd >= dangerEnd)
    {
        qWarning() << "Thresholds must be in ascending order (safeEnd < moderateEnd < dangerEnd)";
        this->safeEnd = 0;
        this->moderateEnd = 50;
        this->dangerEnd = 100;
    }
}
void ComplianceIndicator::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int radius = std::min(width, height) / 2 - 10;

    // Semi-circle bounding rectangle
    QRectF semiCircleRect(width / 2 - radius, 10, 2 * radius, 2 * radius);

    // Calculate spans for each zone based on the thresholds
    double totalRange = dangerEnd - 0;
    double greenSpan = 180.0 * (safeEnd - 0) / totalRange;           // Green zone
    double amberSpan = 180.0 * (moderateEnd - safeEnd) / totalRange; // Amber zone
    double redSpan = 180.0 * (dangerEnd - moderateEnd) / totalRange;

    QBrush brush(Qt::red, Qt::SolidPattern);
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawPie(semiCircleRect, 0, greenSpan * 16);

    brush.setColor(QColor(255, 191, 0)); // Amber color
    painter.setBrush(brush);
    painter.drawPie(semiCircleRect, greenSpan * 16, amberSpan * 16);

    brush.setColor(Qt::green);
    painter.setBrush(brush);
    painter.drawPie(semiCircleRect, (greenSpan + amberSpan) * 16, redSpan * 16);

    // Draw the semi-circle border
    QPen pen(Qt::black, 3);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(semiCircleRect, 0, 180 * 16);

    // Draw the thresholds
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    painter.setPen(Qt::black);

    auto drawThresholdLabel = [&](double threshold, double angleDegrees)
    {
        double angleRadians = qDegreesToRadians(180.0 - angleDegrees);
        int x = width / 2 + (radius + 15) * std::cos(angleRadians);
        int y = radius + 10 - (radius + 15) * std::sin(angleRadians);
        QString label = QString::number(threshold, 'f', 1);
        painter.drawText(QPointF(x - 10, y + 5), label);
    };

    // Calculate positions for the threshold labels
    drawThresholdLabel(0, 0);
    drawThresholdLabel(safeEnd, greenSpan);
    drawThresholdLabel(moderateEnd, greenSpan + amberSpan);
    drawThresholdLabel(dangerEnd, 180.0);

    double valueAngle = 180.0 * (value / totalRange); // Scale the value
    valueAngle = std::clamp(valueAngle, 0.0, 180.0);
    double radians = qDegreesToRadians(180.0 - valueAngle);

    QPoint center(width / 2, radius + 10);
    QPoint arrowTip(center.x() + radius * std::cos(radians), center.y() - radius * std::sin(radians));

    // Draw the arrow line
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setBrush(Qt::black);
    painter.drawLine(center, arrowTip);

    // Draw the arrowhead (triangle)
    QPointF arrowBaseLeft(
        arrowTip.x() - 10 * std::cos(radians + M_PI / 6),
        arrowTip.y() + 10 * std::sin(radians + M_PI / 6));
    QPointF arrowBaseRight(
        arrowTip.x() - 10 * std::cos(radians - M_PI / 6),
        arrowTip.y() + 10 * std::sin(radians - M_PI / 6));

    QPolygonF arrowHead;
    arrowHead << arrowTip << arrowBaseLeft << arrowBaseRight;

    painter.setBrush(Qt::black);
    painter.drawPolygon(arrowHead);

    // Draw the value text at the center of the semi-circle
    QString valueText = QString::number(value, 'f', 2);
    QRectF textRect(center.x() - 30, center.y() + 20, 60, 20);
    painter.drawText(textRect, Qt::AlignCenter, valueText);
}
void ComplianceIndicator::ubdateIndicator(double safe, double moderate, double danger, double newvalue)
{
    // Update the thresholds
    safeEnd = safe;
    moderateEnd = moderate;
    dangerEnd = danger;
    value = newvalue;

    update();
}

QWidget *createComplianceIndicator(double safeEnd, double moderateEnd, double dangerEnd, double initialValue, QWidget *parent)
{
    ComplianceIndicator *complianceIndicator = new ComplianceIndicator(safeEnd, moderateEnd, dangerEnd, initialValue, parent);
    return complianceIndicator;
}