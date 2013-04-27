#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif

#include <qwt_plot_curve.h>


namespace Ui {
    class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void displayResults();
    
private:
    Ui::MainWindow *ui;

    QwtPlotCurve masterCurve;
    QwtPlotCurve slaveCurve;
};

#endif // MAINWINDOW_HPP
