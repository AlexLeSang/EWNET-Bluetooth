#include "Master.hpp"

#include "SimulationManager.hpp"

/*!
 * \brief Master::getInstance
 * \return
 */
Master &Master::getInstance() {
    static Master instance;
    return instance;
}

/*!
 * \brief Master::~Master
 */
Master::~Master() { /*Empty*/ }

/*!
 * \brief Master::reinit
 */
void Master::reinit() {
    BluetoothNode::reinit();

    Message constantMasterMessage;
    constantMasterMessage.setAM_ADDR( MASTER_AM_ADDR );
    for ( qint32 i = SimulationManager::getMAX_CYCLES(); i != 0; --i ) {

        auto information = QByteArray( "Master #" + QString::number(i).toLocal8Bit() );
        information.reserve( PAYLOAD_MAX_INFORMATION_LENGHT / 8 );
        constantMasterMessage.addInformationToPayload( information );
        addMessageToSend( constantMasterMessage );

    }
}

/*!
 * \brief Master::Master
 */
Master::Master() {
    nameString = "Master";
    addToLog = &CoreFacade::sendToMasterLog;
    reinit();
}
