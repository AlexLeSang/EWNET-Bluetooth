#ifndef SIMULATIONMANAGER_HPP
#define SIMULATIONMANAGER_HPP

#include "Master.hpp"
#include "Slave.hpp"
#include "Environment.hpp"

#include <QRunnable>
#include <QList>
#include <QPointF>

/*!
 * \brief The SimulationManager class
 */
class SimulationManager : public QRunnable {
public:
    static SimulationManager & getInstance();

    ~SimulationManager();
    void run();

    void setErrorProbability( double probability );
    void setErrorProbabilityFrom( double probability );
    void setErrorProbabilityTo( double probability );
    void setErrorProbabilityStep( double probability );

    static void setMAX_CYCLES( quint32 max_cycles );
    static void setSLOT_DURATION( quint32 slot_duration );

    static quint32 getMAX_CYCLES();
    static quint32 getSLOT_DURATION();

    double getErrorProbability() const;
    double getErrorProbabilityRange() const;
    double getErrorProbabilityFrom() const;
    double getErrorProbabilityTo() const;
    double getErrorProbabilityStep() const;

    const QList< QPointF > & getMasterResultList() const;
    const QList< QPointF > & getSlaveResultList() const;

    void startSimpleSimulation();
    void stopSimpleSimulation();

    void startRangeSimulation();
    void stopRangeSimulation();


    SimulationManager( const SimulationManager &  ) = delete;
    SimulationManager & operator = ( const SimulationManager & ) = delete;
    bool operator == ( const SimulationManager &  ) = delete;
    bool operator != ( const SimulationManager &  ) = delete;

private:
    explicit SimulationManager();

    void simpleSimulation();
    void rangeSimulation();
private:
    static quint32 MAX_CYCLES;
    static quint32 SLOT_DURATION;

    volatile bool stopFlag = false;

    bool useSimpleSimulationFlag = true;

    Master & masterRef;
    Slave & slaveRef;
    Environment & environmentRef;

    QList< QPointF > masterResultList;
    QList< QPointF > slaveResultList;
};

#endif // SIMULATIONMANAGER_HPP
