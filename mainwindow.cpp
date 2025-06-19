#include "mainwindow.h"

#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

struct MainWindow::MainWindowImpl {
  QAction *newAction;
  QAction *openAction;
  QAction *saveAction;

  void setUp(MainWindow *parent) {
    // Create bars
    QToolBar *toolBar = parent->addToolBar(tr("File"));
    QMenu *menu = parent->menuBar()->addMenu(tr("File"));

    // Construct the objects
    newAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                            tr("newFile"), parent);
    newAction->setShortcut(QKeySequence::New);
    openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                             tr("openFile"), parent);
    openAction->setShortcut(QKeySequence::Open);
    saveAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                             tr("saveFile"), parent);
    saveAction->setShortcut(QKeySequence::Save);

    menu->addAction(newAction);
    menu->addAction(openAction);
    menu->addAction(saveAction);

    toolBar->addAction(newAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
  }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), impl_(new MainWindowImpl) {
  impl_->setUp(this);
}

MainWindow::~MainWindow() {}
