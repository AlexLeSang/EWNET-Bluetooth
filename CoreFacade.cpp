#include "CoreFacade.hpp"

#include <QThreadPool>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot.h>

#include "SimulationManager.hpp"

/*!
 * \brief CoreFacade::getInstance
 * \return
 */
CoreFacade &CoreFacade::getInstance() {
    static CoreFacade instance;
    return instance;
}

/*!
 * \brief CoreFacade::sendToMasterLog
 * \param message
 */
void CoreFacade::sendToMasterLog( QString message ) {
    emit addToMasterLog( message );
}

/*!
 * \brief CoreFacade::sendToEnvironmentLog
 * \param message
 */
void CoreFacade::sendToEnvironmentLog( QString message ) {
    emit addToEnvironmentLog( message );
}

/*!
 * \brief CoreFacade::sendToSlaveLog
 * \param message
 */
void CoreFacade::sendToSlaveLog( QString message ) {
    emit addToSlaveLog( message );
}

/*!
 * \brief CoreFacade::sendCurrentCycleNumber
 * \param cycleNumber
 */
void CoreFacade::sendCurrentCycleNumber( quint32 cycleNumber ) {
    emit addCurrentCycleNumber( QString::number( cycleNumber ) );
}

/*!
 * \brief CoreFacade::simulationFinished
 */
void CoreFacade::simulationFinished() {
    emit simulationEnd();
}

/*!
 * \brief CoreFacade::rangeSimulationFinished
 */
void CoreFacade::rangeSimulationFinished() {
    emit rangeSimulationEnd();
}

/*!
 * \brief CoreFacade::maxCycles
 */
int CoreFacade::maxCycles() const {
    return simulationManagerRef.getMAX_CYCLES();
}

/*!
 * \brief CoreFacade::slotDuration
 */
int CoreFacade::slotDuration() const {
    return simulationManagerRef.getSLOT_DURATION();
}

/*!
 * \brief CoreFacade::errorProbability
 * \return
 */
double CoreFacade::errorProbability() const {
    return simulationManagerRef.getErrorProbability();
}

/*!
 * \brief CoreFacade::errorProbabilityFrom
 * \return
 */
double CoreFacade::errorProbabilityFrom() const {
    return simulationManagerRef.getErrorProbabilityFrom();
}

/*!
 * \brief CoreFacade::errorProbabilityTo
 * \return
 */
double CoreFacade::errorProbabilityTo() const {
    return simulationManagerRef.getErrorProbabilityTo();
}

/*!
 * \brief CoreFacade::errorProbabilityStep
 * \return
 */
double CoreFacade::errorProbabilityStep() const {
    return simulationManagerRef.getErrorProbabilityStep();
}

/*!
 * \brief CoreFacade::setPlotPtr
 * \param ptr
 */
void CoreFacade::setPlotPtr(QwtPlot *ptr) {
    plotPtr = ptr;
}

/*!
 * \brief CoreFacade::getMasterResults
 * \return
 */
const QVector<QPointF> CoreFacade::getMasterResults() const {
    return simulationManagerRef.getMasterResultList().toVector();
}

/*!
 * \brief CoreFacade::getSlaveResults
 * \return
 */
const QVector<QPointF> CoreFacade::getSlaveResults() const {
    return simulationManagerRef.getSlaveResultList().toVector();
}

/*!
 * \brief CoreFacade::startSimulation
 */
void CoreFacade::startSimpleSimulation() {
    simulationManagerRef.startSimpleSimulation();
}

/*!
 * \brief CoreFacade::stopSimulation
 */
void CoreFacade::stopSimpleSimulation() {
    simulationManagerRef.stopSimpleSimulation();
}

/*!
 * \brief CoreFacade::startRandeSimulation
 */
void CoreFacade::startRangeSimulation() {
    simulationManagerRef.startRangeSimulation();
}

/*!
 * \brief CoreFacade::stopRangeSimulation
 */
void CoreFacade::stopRangeSimulation() {
    simulationManagerRef.stopRangeSimulation();
}

/*!
 * \brief CoreFacade::setNumberOfSimulationCycles
 * \param number
 */
void CoreFacade::setNumberOfSimulationCycles( int number ) {
    SimulationManager::getInstance().setMAX_CYCLES( number );
}

/*!
 * \brief CoreFacade::setSimulationSlotDuration
 * \param duration
 */
void CoreFacade::setSimulationSlotDuration( int duration ) {
    SimulationManager::getInstance().setSLOT_DURATION( duration );
}

/*!
 * \brief CoreFacade::setErrorProbability
 */
void CoreFacade::setErrorProbability(double probability) {
    SimulationManager::getInstance().setErrorProbability( probability );
}

/*!
 * \brief CoreFacade::setErrorProbabilityFrom
 * \param probability
 */
void CoreFacade::setErrorProbabilityFrom(double probability) {
    SimulationManager::getInstance().setErrorProbabilityFrom( probability );
}

/*!
 * \brief CoreFacade::setErrorProbabilityTo
 * \param probability
 */
void CoreFacade::setErrorProbabilityTo(double probability) {
    SimulationManager::getInstance().setErrorProbabilityTo( probability );
}

/*!
 * \brief CoreFacade::setErrorProbabilityStep
 * \param probability
 */
void CoreFacade::setErrorProbabilityStep(double probability) {
    SimulationManager::getInstance().setErrorProbabilityStep( probability );
}

/*!
 * \brief CoreFacade::CoreFacade
 * \param parent
 */
CoreFacade::CoreFacade( QObject *parent ) :
    QObject( parent ),
    simulationManagerRef( SimulationManager::getInstance() )
{ /*Empty*/ }
