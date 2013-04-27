#include "SimulationManager.hpp"

#include <QThreadPool>

#include <QDebug>

#include "Sleep.hpp"
#include "Defines.hpp"

#include "CoreFacade.hpp"

/*!
 * \brief SimulationManager::MAX_CYCLES
 */
quint32 SimulationManager::MAX_CYCLES = 10;

/*!
 * \brief SimulationManager::SLOT_DURATION
 */
quint32 SimulationManager::SLOT_DURATION = 150;

/*!
 * \brief SimulationManager::getInstance
 * \return
 */
SimulationManager &SimulationManager::getInstance() {
    static SimulationManager instance;
    return instance;
}

/*!
 * \brief SimulationManager::~SimulationManager
 */
SimulationManager::~SimulationManager() { /*Empty*/ }

/*!
 * \brief SimulationManager::simpleSimulation
 */
void SimulationManager::simpleSimulation() {
    auto count = MAX_CYCLES;
    const quint32 & slotDurationConsRef = SLOT_DURATION;
    stopFlag = false;

    masterRef.reinit();
    slaveRef.reinit();
    environmentRef.reinit();

    while ( ( count != 0 ) && ( ! stopFlag ) ) {

        masterRef.send( environmentRef );
        environmentRef.transmit();
        slaveRef.receive( environmentRef );

        Sleep::msleep( slotDurationConsRef / 2 );

        slaveRef.send( environmentRef );
        environmentRef.transmit();
        masterRef.receive( environmentRef );

        Sleep::msleep( slotDurationConsRef / 2 );

        --count;
        CoreFacade::getInstance().sendCurrentCycleNumber( count );
    }

    masterRef.sendTransmitedPackageNumber();
    masterRef.sendReceivedPacketsNumber();

    slaveRef.sendTransmitedPackageNumber();
    slaveRef.sendReceivedPacketsNumber();
}

/*!
 * \brief SimulationManager::rangeSimulation
 */
void SimulationManager::rangeSimulation() {
    masterResultList.clear();
    slaveResultList.clear();

    stopFlag = false;

    // Save previous value of probability
    const auto prevErrorProbability = environmentRef.getErrorProbability();

    for( auto probabilityValue = environmentRef.getErrorProbabilityFrom();
         ( probabilityValue < environmentRef.getErrorProbabilityTo() ) && ( !stopFlag );
         probabilityValue += environmentRef.getErrorProbabilityStep() )
    {
        environmentRef.setErrorProbability( probabilityValue );

        auto count = MAX_CYCLES;

        masterRef.reinit();
        slaveRef.reinit();
        environmentRef.reinit();

        while ( ( count != 0 ) && ( ! stopFlag ) ) {

            masterRef.send( environmentRef );
            environmentRef.transmit();
            slaveRef.receive( environmentRef );

            slaveRef.send( environmentRef );
            environmentRef.transmit();
            masterRef.receive( environmentRef );

            --count;
        }

        masterRef.sendTransmitedPackageNumber();
        masterRef.sendReceivedPacketsNumber();

        slaveRef.sendTransmitedPackageNumber();
        slaveRef.sendReceivedPacketsNumber();

        // Add master point
        const qreal masterRatio = ( 1.0 * masterRef.getReceivePackageCounter() ) / ( 1.0 * masterRef.getTransmitedPackageCounter() );
        masterResultList.append( QPointF( probabilityValue, masterRatio ) );

        // Add slave point
        const qreal slaveRatio = ( 1.0 * slaveRef.getReceivePackageCounter() ) / ( 1.0 * slaveRef.getTransmitedPackageCounter() );
        slaveResultList.append( QPointF( probabilityValue, slaveRatio ) );
    }

    // Restore previous value of probability
    environmentRef.setErrorProbability( prevErrorProbability );
}

/*!
 * \brief SimulationManager::run
 */
void SimulationManager::run() {
    if ( useSimpleSimulationFlag ) {
        simpleSimulation();
        CoreFacade::getInstance().simulationFinished();
    }
    else {
        rangeSimulation();
        CoreFacade::getInstance().rangeSimulationFinished();
    }
}

/*!
 * \brief SimulationManager::setErrorProbability
 * \param probability
 */
void SimulationManager::setErrorProbability(double probability) {
    environmentRef.setErrorProbability( probability );
}

/*!
 * \brief SimulationManager::setErrorProbabilityFrom
 * \param probability
 */
void SimulationManager::setErrorProbabilityFrom(double probability) {
    environmentRef.setErrorProbabilityFrom( probability );
}

/*!
 * \brief SimulationManager::setErrorProbabilityTo
 * \param probability
 */
void SimulationManager::setErrorProbabilityTo(double probability) {
    environmentRef.setErrorProbabilityTo( probability );
}

/*!
 * \brief SimulationManager::setErrorProbabilityStep
 * \param probability
 */
void SimulationManager::setErrorProbabilityStep(double probability) {
    environmentRef.setErrorProbabilityStep( probability );
}

/*!
 * \brief SimulationManager::setMAX_CYCLES
 * \param max_cycles
 */
void SimulationManager::setMAX_CYCLES( quint32 max_cycles ) {
    MAX_CYCLES = max_cycles;
}

/*!
 * \brief SimulationManager::setSLOT_DURATION
 * \param slot_duration
 */
void SimulationManager::setSLOT_DURATION( quint32 slot_duration ) {
    SLOT_DURATION = slot_duration;
}

/*!
 * \brief SimulationManager::startSimulation
 */
void SimulationManager::startSimpleSimulation() {
    useSimpleSimulationFlag = true;
    QThreadPool::globalInstance()->start( &SimulationManager::getInstance() );
}

/*!
 * \brief SimulationManager::stopSimulation
 */
void SimulationManager::stopSimpleSimulation() {
    stopFlag = true;
}

/*!
 * \brief SimulationManager::startRangeSimulation
 */
void SimulationManager::startRangeSimulation() {
    useSimpleSimulationFlag = false;
    QThreadPool::globalInstance()->start( &SimulationManager::getInstance() );
}

/*!
 * \brief SimulationManager::stopRangeSimulation
 */
void SimulationManager::stopRangeSimulation() {
    stopFlag = true;
}

/*!
 * \brief SimulationManager::getMAX_CYCLES
 * \return
 */
quint32 SimulationManager::getMAX_CYCLES() {
    return MAX_CYCLES;
}

/*!
 * \brief SimulationManager::getSLOT_DURATION
 * \return
 */
quint32 SimulationManager::getSLOT_DURATION() {
    return SLOT_DURATION;
}

/*!
 * \brief SimulationManager::getErrorProbability
 * \return
 */
double SimulationManager::getErrorProbability() const {
    return environmentRef.getErrorProbability();
}

/*!
 * \brief SimulationManager::getErrorProbabilityFrom
 * \return
 */
double SimulationManager::getErrorProbabilityFrom() const {
    return environmentRef.getErrorProbabilityFrom();
}

/*!
 * \brief SimulationManager::getErrorProbabilityTo
 * \return
 */
double SimulationManager::getErrorProbabilityTo() const {
    return environmentRef.getErrorProbabilityTo();
}

/*!
 * \brief SimulationManager::getErrorProbabilityStep
 * \return
 */
double SimulationManager::getErrorProbabilityStep() const {
    return environmentRef.getErrorProbabilityStep();
}

/*!
 * \brief SimulationManager::getMasterResultList
 * \return
 */
const QList<QPointF> &SimulationManager::getMasterResultList() const {
    return masterResultList;
}

/*!
 * \brief SimulationManager::getSlaveResultList
 * \return
 */
const QList<QPointF> &SimulationManager::getSlaveResultList() const {
    return slaveResultList;
}

/*!
 * \brief SimulationManager::SimulationManager
 */
SimulationManager::SimulationManager() :
    masterRef( Master::getInstance() ),
    slaveRef( Slave::getInstance() ),
    environmentRef( Environment::getInstance() )
{
    setAutoDelete( false );
}


#ifdef SIMULATION_MANAGER_TEST
#include <QtTest/QtTest>
#include <QThreadPool>

class SimulationManagerTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() { /*Empty*/ }

    // Here you can implement your own test slots
    void runTest () {
        QThreadPool::globalInstance()->start( &SimulationManager::getInstance() );
        QThreadPool::globalInstance()->waitForDone();
    }

    void cleanupTestCase() { /*Empty*/ }
};

QTEST_MAIN( SimulationManagerTest )
#include "SimulationManager.moc"

#endif
