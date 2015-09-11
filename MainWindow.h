#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QProcess>

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit CMainWindow(QWidget *parent = 0);
  ~CMainWindow();
  int secToMin(int sec);
  int minToSec(int min);

private:
  Ui::CMainWindow *ui;
  int m_secToRun;
  bool m_state;
  QTimer *mp_timer;
  int m_remainingSec;
  QProcess m_executeProgram;

  void connectSignals();
  int getPercentageProgress();
  void startTimer();
  void stopTimer(bool finished = false);

public slots:
  void on_action_Close_triggered();
  void on_action_About_triggered();
  void showState();
  void executeCommand();
  void processClickedButton();
  void processPauseButton(bool pressed);
};

#endif // MAINWINDOW_H
