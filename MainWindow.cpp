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
  connectSignals();
  readPathsFile();
  refreshRemainSeconds();
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

int CMainWindow::hourToSec(int hour) {
  return hour * 3600;
}// hourToSec(int hour)

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
  connect(ui->secBox, SIGNAL(valueChanged(int)), SLOT(processSecValueChanged(int)));
  connect(ui->minBox, SIGNAL(valueChanged(int)), SLOT(processMinValueChanged(int)));
  connect(ui->hourBox, SIGNAL(valueChanged(int)), SLOT(processHourValueChanged(int)));
}// connectSignals()

void CMainWindow::readPathsFile() {
  QFile paths("paths.txt");
  if (paths.open(QIODevice::ReadOnly)) {
    QTextStream in(&paths);
    while (!in.atEnd()) {
      ui->pathBox->addItem(in.readLine());
    }
    paths.close();
  } else {
    ui->pathBox->addItem("shutdown.exe -f -s -t 0");
    ui->pathBox->addItem("explorer.exe");
  }
}

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
  QString program = ui->pathBox->currentText();
  //QStringList args;
  //m_executeProgram.startDetached(test, args);
  m_executeProgram.startDetached(program);
  disconnect(mp_timer, SIGNAL(timeout()), 0, 0);
  connect(mp_timer, SIGNAL(timeout()), SLOT(on_action_Close_triggered()));
  mp_timer->start(5000);
}// executeCommand()

void CMainWindow::startTimer()
{
  m_state = true;
  ui->startstopButton->setText("Stop");
  ui->pauseButton->show();
  ui->pathBox->setEnabled(false);
  ui->hourBox->setEnabled(false);
  ui->minBox->setEnabled(false);
  ui->secBox->setEnabled(false);
  m_secToRun = ui->secBox->value();
  m_secToRun += minToSec(ui->minBox->value());
  m_secToRun += hourToSec(ui->hourBox->value());
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
    disconnect(ui->pauseButton, SIGNAL(toggled(bool)), 0, 0);
    ui->pauseButton->toggle();
    connect(ui->pauseButton, SIGNAL(toggled(bool)), SLOT(processPauseButton(bool)));
  }
  ui->pathBox->setEnabled(true);
  ui->hourBox->setEnabled(true);
  ui->minBox->setEnabled(true);
  ui->secBox->setEnabled(true);
  ui->pauseButton->hide();
  if (!finished) {
    ui->progressBar->setValue(0);
    refreshRemainSeconds();
  }
}// stopTimer(bool finished)

void CMainWindow::refreshRemainSeconds() {
  m_secToRun = ui->secBox->value();
  m_secToRun += minToSec(ui->minBox->value());
  m_secToRun += hourToSec(ui->hourBox->value());
  ui->remainTimeShow->setText(QString("%1 sek").arg(m_secToRun));
}

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

void CMainWindow::processSecValueChanged(int value) {
  if (60 == value) {
    ui->secBox->setValue(0);
    ui->minBox->setValue(ui->minBox->value() + 1);
  }
  if (-1 == value) {
    if (ui->minBox->value() <= 0 && ui->hourBox->value() <= 0) {
      ui->secBox->setValue(0);
      if (ui->minBox->value() <= 0 && ui->hourBox->value() > 0) {
        ui->minBox->setValue(-1);
        ui->secBox->setValue(59);
      }
    } else {
      ui->secBox->setValue(59);
      ui->minBox->setValue(ui->minBox->value() - 1);
    }
  }
  refreshRemainSeconds();
}

void CMainWindow::processMinValueChanged(int value) {
  if (60 == value) {
    ui->minBox->setValue(0);
    ui->hourBox->setValue(ui->hourBox->value() + 1);
  }
  if (-1 == value) {
    if (ui->hourBox->value() <= 0) {
      ui->minBox->setValue(0);
    } else {
      ui->minBox->setValue(59);
      ui->hourBox->setValue(ui->hourBox->value() - 1);
    }
  }
  refreshRemainSeconds();
}

void CMainWindow::processHourValueChanged(int value) {
  refreshRemainSeconds();
}
