#include <QtGui/QApplication>

#include "Defines.hpp"

#include "MainWindow.hpp"

#ifdef USE_MAIN

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main( int argc, char * argv[] ) {
    QApplication a( argc, argv );

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}

#endif


/*! \mainpage
<b> Task: </b>
    Write a program that makes modeling process of error correction and ARQ protocol in Bluetooth standard.

<b> Solution: </b>
    At first I read Bluetooth standard and found that in Bluetooth is used three types of error correction approaches: FEC 1/3, FEC 2/3 and ARQ.

<b> Domain dictionary: </b>
    - FEC 1/3 is used for headers error protection and this work in such way, that we transmit each bit three times and when we receive this header we have to calculate number of ones for each trinity and transform it into a signle bit, this approach can recover a single error.

    - FEC 2/3 is used as name for CRC-16 checksum, that is calculated for payload of packet. Also for header we create a header error correction value as a CRC-8 checksum.

    - Automatic Repeat reQuest (ARQ), also known as Automatic Repeat Query, is an error-control method for data transmission that uses acknowledgements.

<b> Application interface description: </b>

<b> Tab one: </b> Simple Modeling
<img src="Simple_Modeling_Tab.jpg" alt="Simple modeling tab" height="600" width="800">

Here you cat set:
    - error probability for packet( from 0.01 to 0.99 with step 0.01 );
    - number of simulation cycles( from 1 to 100 with step 1 );
    - one simulation in microseconds( from 100 to 500 step 50 );

You can see current cycle number at special area on this tab.

Logs for master, slave and envorinment are aviable below. There you can see modeling results for each cycle.


<b> Tab two: </b> Range Modeling
<img src="Range_Modeling_Tab.jpg" alt="Range modeling tab" height="600" width="800">

You can see current cycle number at special area on this tab.

Modeling results are aviable below.


<b> Information sources: </b>

<a href="http://www.palowireless.com/bluearticles/packets.asp">Bluetooth package structure</a>.

<b> Author: </b>

Oleksandr Halushko
email: alexlesang@gmail.com

*/
