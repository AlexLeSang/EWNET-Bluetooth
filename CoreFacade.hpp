#ifndef COREFACADE_HPP
#define COREFACADE_HPP

#include <QObject>
#include <QPointF>
#include <QVector>


class SimulationManager;

class QwtPlot;

/*!
 * \brief The CoreFacade class
 */
class CoreFacade : public QObject {
    Q_OBJECT
public:
    static CoreFacade & getInstance();

    void sendToMasterLog( QString message );
    void sendToEnvironmentLog( QString message );
    void sendToSlaveLog( QString message );
    void sendCurrentCycleNumber( quint32 cycleNumber );
    void simulationFinished();
    void rangeSimulationFinished();

    int maxCycles() const;
    int slotDuration() const;

    double errorProbability() const;

    double errorProbabilityFrom() const;
    double errorProbabilityTo() const;
    double errorProbabilityStep() const;

    void setPlotPtr(QwtPlot *ptr );

    const QVector<QPointF> getMasterResults() const;
    const QVector<QPointF> getSlaveResults() const;

signals:
    void addToMasterLog( QString message );
    void addToEnvironmentLog( QString message );
    void addToSlaveLog( QString message );
    void addCurrentCycleNumber( QString cycleNumberString );
    void simulationEnd();
    void rangeSimulationEnd();

public slots:
    void startSimpleSimulation();
    void stopSimpleSimulation();

    void startRangeSimulation();
    void stopRangeSimulation();

    void setNumberOfSimulationCycles( int number );
    void setSimulationSlotDuration( int duration );

    void setErrorProbability(double probability);
    void setErrorProbabilityFrom(double probability);
    void setErrorProbabilityTo(double probability);
    void setErrorProbabilityStep(double probability);

private:
    explicit CoreFacade( QObject *parent = 0 );

private:
    SimulationManager & simulationManagerRef;
    QwtPlot * plotPtr;
};

#endif // COREFACADE_HPP
