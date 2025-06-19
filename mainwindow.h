#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  struct MainWindowImpl;
  friend struct MainWindowImpl;
  QScopedPointer<MainWindowImpl> impl_;
};
#endif // MAINWINDOW_H
