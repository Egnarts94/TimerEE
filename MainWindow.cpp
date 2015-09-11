#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AboutDialog.h"

CMainWindow::CMainWindow(QWidget *parent)
  :  QMainWindow(parent)
  ,  ui(new Ui::CMainWindow)
  ,  m_secToRun()
  ,  m_state()
  ,  mp_timer()
  ,  m_executeProgram()
{
  m_state = false;
  mp_timer = new QTimer();
  mp_timer->setTimerType(Qt::VeryCoarseTimer);

  ui->setupUi(this);
  ui->pauseButton->hide();
  ui->remainTimeShow->hide();
  connectSignals();
}// CMainWindow(QWidget *parent)

CMainWindow::~CMainWindow()
{
  delete mp_timer;
  delete ui;
}// ~CMainWindow()

int CMainWindow::secToMin(int sec)
{
  return qRound((float)sec / 60);
}// secToMin(int sec)

int CMainWindow::minToSec(int min)
{
  return min * 60;
}// minToSec(int min)

void CMainWindow::on_action_Close_triggered()
{
  mp_timer->stop();
  close();
}// on_action_FileClose_triggered()

void CMainWindow::on_action_About_triggered()
{
  CAboutDialog dialog;
  dialog.exec();
}// on_action_About_triggered()

void CMainWindow::connectSignals()
{
  connect(ui->startstopButton, SIGNAL(clicked()), SLOT(processClickedButton()));
  connect(mp_timer, SIGNAL(timeout()), SLOT(showState()));
  connect(ui->pauseButton, SIGNAL(toggled(bool)), SLOT(processPauseButton(bool)));
}// connectSignals()

int CMainWindow::getPercentageProgress()
{
  int passedSec = m_secToRun - m_remainingSec;
  int percentageProgress = qRound(((float)passedSec / (float)m_secToRun) * 100);
  return percentageProgress;
}// getPercentageProgress()

void CMainWindow::showState()
{
  ui->remainTimeShow->setText(QString("%1 sek").arg(m_remainingSec));
  ui->progressBar->setValue(getPercentageProgress());
  ui->progressBar->setToolTip(QString("%1 Minuten verbleibend").arg(secToMin(m_remainingSec)));
  if (m_remainingSec <= 0) {
    executeCommand();
  }
  m_remainingSec--;
}// showState()

void CMainWindow::executeCommand()
{
  stopTimer(true);
  QString test = ui->pathBox->currentText();
  m_executeProgram.start(test);
  disconnect(mp_timer, SIGNAL(timeout()));
  connect(mp_timer, SIGNAL(timeout()), SLOT(on_action_Close_triggered()));
  mp_timer->start(10000);
}// executeCommand()

void CMainWindow::startTimer()
{
  m_state = true;
  ui->startstopButton->setText("Stop");
  ui->pauseButton->show();
  ui->pathBox->setEnabled(false);
  ui->timeBox->setEnabled(false);
  m_secToRun = minToSec(ui->timeBox->value());
  m_remainingSec = m_secToRun;
  mp_timer->start(1000);
  ui->remainTimeShow->show();
  showState();
}// startTimer()

void CMainWindow::stopTimer(bool finished)
{
  m_state = false;
  mp_timer->stop();
  ui->startstopButton->setText("Start");
  if (ui->pauseButton->isChecked()) {
    ui->pauseButton->toggle();
  }
  ui->pathBox->setEnabled(true);
  ui->timeBox->setEnabled(true);
  ui->pauseButton->hide();
  if (!finished) {
    ui->progressBar->setValue(0);
    ui->remainTimeShow->setText("0");
  }
}// stopTimer(bool finished)

void CMainWindow::processClickedButton()
{
  if (false == m_state) { //Start
    startTimer();
  }
  else {                  //Stop
    stopTimer();
  }
}// processClickedButton()

void CMainWindow::processPauseButton(bool pressed)
{
  if (pressed) {
    mp_timer->stop();
    ui->pathBox->setEnabled(true);
    ui->pauseButton->setText("Fortsetzen");
  }
  else {
    mp_timer->start(1000);
    ui->pathBox->setEnabled(false);
    ui->pauseButton->setText("Pause");
  }
}// processPauseButton(bool pressed)
