#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include "CoreFacade.hpp"

#include <QThreadPool>

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle( "Bluetooth FEC modeling" );

    // Set UI default values
    ui->numberOfSimulationsSpinBox->setValue( CoreFacade::getInstance().maxCycles() );
    ui->durationSpinBox->setValue( CoreFacade::getInstance().slotDuration() );
    ui->errorProbabilityDoubleSpinBox->setValue( CoreFacade::getInstance().errorProbability() );

    // Set UI default values for range simulation
    ui->errorProbabilityFromDoubleSpinBox->setValue( CoreFacade::getInstance().errorProbabilityFrom() );
    ui->errorProbabilityToDoubleSpinBox->setValue( CoreFacade::getInstance().errorProbabilityTo() );
    ui->errorProbabilityStepDoubleSpinBox->setValue( CoreFacade::getInstance().errorProbabilityStep() );


    // Move core to anouther thread
    CoreFacade::getInstance().moveToThread( QThreadPool::globalInstance()->thread() );

    // Buttons connections
    QObject::connect( ui->startButton, SIGNAL( clicked() ),
             &CoreFacade::getInstance(), SLOT( startSimpleSimulation() ) );

    QObject::connect( ui->stopButton, SIGNAL( clicked() ),
             &CoreFacade::getInstance(), SLOT( stopSimpleSimulation() ) );

    // Buttons connections for range simulation
    QObject::connect( ui->startRangeButton, SIGNAL( clicked() ),
             &CoreFacade::getInstance(), SLOT( startRangeSimulation() ) );

    QObject::connect( ui->stopRangeButton, SIGNAL( clicked() ),
             &CoreFacade::getInstance(), SLOT( stopRangeSimulation() ) );


    // Paramiters connections
    QObject::connect( ui->errorProbabilityDoubleSpinBox, SIGNAL( valueChanged(double) ),
             &CoreFacade::getInstance(), SLOT( setErrorProbability(double) ) );

    QObject::connect( ui->numberOfSimulationsSpinBox, SIGNAL( valueChanged(int) ),
             &CoreFacade::getInstance(), SLOT( setNumberOfSimulationCycles(int) ) );

    QObject::connect( ui->durationSpinBox, SIGNAL( valueChanged(int) ),
             &CoreFacade::getInstance(), SLOT( setSimulationSlotDuration(int) ) );

    // Connect interface for range modeling
    QObject::connect( ui->errorProbabilityFromDoubleSpinBox, SIGNAL( valueChanged(double) ),
                      &CoreFacade::getInstance(), SLOT( setErrorProbabilityFrom(double) ) );

    QObject::connect( ui->errorProbabilityToDoubleSpinBox, SIGNAL( valueChanged(double) ),
                      &CoreFacade::getInstance(), SLOT( setErrorProbabilityTo(double) ) );

    QObject::connect( ui->errorProbabilityStepDoubleSpinBox, SIGNAL( valueChanged(double) ),
                      &CoreFacade::getInstance(), SLOT( setErrorProbabilityStep(double) ) );


    // Facade to log connections
    QObject::connect( &CoreFacade::getInstance(), SIGNAL( addToMasterLog(QString) ),
                      ui->masterLog, SLOT( append(QString) ) );

    QObject::connect( &CoreFacade::getInstance(), SIGNAL( addToEnvironmentLog(QString) ),
                      ui->environmentLog, SLOT( append(QString) ) );

    QObject::connect( &CoreFacade::getInstance(), SIGNAL( addToSlaveLog(QString) ),
                      ui->slaveLog, SLOT( append(QString) ) );

    QObject::connect( &CoreFacade::getInstance(), SIGNAL( addCurrentCycleNumber(QString) ),
                      ui->currentCycleLabel, SLOT( setText(QString)) );
    QObject::connect( &CoreFacade::getInstance(), SIGNAL( simulationEnd() ),
                      ui->stopButton, SLOT( click() ) );

    QObject::connect( &CoreFacade::getInstance(), SIGNAL( rangeSimulationEnd() ),
                      ui->stopRangeButton, SLOT( click() ) );

    QObject::connect( &CoreFacade::getInstance(), SIGNAL( rangeSimulationEnd() ),
                      this, SLOT( displayResults() ) );

    // Qwt Plot
    ui->qwtPlot->setTitle( "Receive/Transmite ratio" );
    ui->qwtPlot->setCanvasBackground( Qt::white );
    ui->qwtPlot->setAxisScale( QwtPlot::yLeft, 0.0, 1.0 );
    ui->qwtPlot->setAxisTitle( QwtPlot::yLeft, "Ratio" );
    ui->qwtPlot->setAxisScale( QwtPlot::xBottom, 0.0, 1.0 );
    ui->qwtPlot->setAxisTitle( QwtPlot::xBottom, "Error probability" );
    ui->qwtPlot->insertLegend( new QwtLegend() );

    // Qwt grid
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach( ui->qwtPlot );

    // Master curve
    masterCurve.setTitle( "Master" );
    masterCurve.setPen( QPen( Qt::blue, 1.5 ) );
    masterCurve.setRenderHint( QwtPlotItem::RenderAntialiased, true );

    QwtSymbol *masterSymbol = new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::yellow ), QPen( Qt::red, 1.5 ), QSize( 5.5, 5.5 ) );
    masterCurve.setSymbol( masterSymbol );

    // Slave curve
    slaveCurve.setTitle( "Slave" );
    slaveCurve.setPen( QPen( Qt::red, 1.5 ) );
    slaveCurve.setRenderHint( QwtPlotItem::RenderAntialiased, true );

    QwtSymbol *slaveSymbol = new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::green ), QPen( Qt::blue, 1.5 ), QSize( 5.5, 5.5 ) );
    slaveCurve.setSymbol( slaveSymbol );

    CoreFacade::getInstance().setPlotPtr( ui->qwtPlot );
}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow() {
    CoreFacade::getInstance().stopSimpleSimulation();
    delete ui;
}

void MainWindow::displayResults() {
    ui->qwtPlot->detachItems( QwtPlotItem::Rtti_PlotCurve, false );
    ui->qwtPlot->replot();

    // Master curve
    QPolygonF masterPoints( CoreFacade::getInstance().getMasterResults() );
    masterCurve.setSamples( masterPoints );
    masterCurve.attach( ui->qwtPlot );

    // Slave curve
    QPolygonF slavePoints( CoreFacade::getInstance().getSlaveResults() );
    slaveCurve.setSamples( slavePoints );
    slaveCurve.attach( ui->qwtPlot );
}
