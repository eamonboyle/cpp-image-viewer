#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent)
	, fileMenu(nullptr)
	, viewMenu(nullptr)
	, currentImage(nullptr)
{
	initUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUI()
{
	this->resize(800, 600);
	fileMenu = menuBar()->addMenu("&File");
	viewMenu = menuBar()->addMenu("&View");

	fileToolBar = addToolBar("File");
	viewToolBar = addToolBar("View");

	imageScene = new QGraphicsScene(this);
	imageView = new QGraphicsView(imageScene);
	setCentralWidget(imageView);

	mainStatusBar = statusBar();
	mainStatusLabel = new QLabel(mainStatusBar);
	mainStatusBar->addPermanentWidget(mainStatusLabel);
	mainStatusLabel->setText("Image Information will be here!");

	createActions();
}

void MainWindow::createActions()
{
	openAction = new QAction("&Open", this);
	fileMenu->addAction(openAction);
	saveAsAction = new QAction("&Save as", this);
	fileMenu->addAction(saveAsAction);
	exitAction = new QAction("E&xit", this);
	fileMenu->addAction(exitAction);

	zoomInAction = new QAction("Zoom in", this);
	viewMenu->addAction(zoomInAction);
	zoomOutAction = new QAction("Zoom Out", this);
	viewMenu->addAction(zoomOutAction);
	prevAction = new QAction("&Previous Image", this);
	viewMenu->addAction(prevAction);
	nextAction = new QAction("&Next Image", this);
	viewMenu->addAction(nextAction);

	// add actions to toolbars
	fileToolBar->addAction(openAction);
	viewToolBar->addAction(zoomInAction);
	viewToolBar->addAction(zoomOutAction);
	viewToolBar->addAction(prevAction);
	viewToolBar->addAction(nextAction);

	// functions for the menu items
	connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
	connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openImage()));
	connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
	connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
	connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
	connect(prevAction, SIGNAL(triggered(bool)), this, SLOT(prevImage()));
	connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextImage()));

	setupShortcuts();
}

void MainWindow::setupShortcuts()
{
	QList<QKeySequence> shortcuts;
	shortcuts << Qt::Key_Plus << Qt::Key_Equal;
	zoomInAction->setShortcuts(shortcuts);

	shortcuts.clear();
	shortcuts << Qt::Key_Minus << Qt::Key_Underscore;
	zoomOutAction->setShortcuts(shortcuts);

	shortcuts.clear();
	shortcuts << Qt::Key_Up << Qt::Key_Left;
	prevAction->setShortcuts(shortcuts);

	shortcuts.clear();
	shortcuts << Qt::Key_Down << Qt::Key_Right;
	nextAction->setShortcuts(shortcuts);
}

void MainWindow::openImage()
{
	qDebug() << "slot openImage is called.";

	QFileDialog dialog(this);
	dialog.setWindowTitle("Open Image");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));

	QStringList filePaths;

	if (dialog.exec())
	{
		filePaths = dialog.selectedFiles();
		showImage(filePaths.at(0));
	}
}

void MainWindow::showImage(QString path)
{
	imageScene->clear();
	imageView->resetMatrix();
	QPixmap image(path);
	currentImage = imageScene->addPixmap(image);
	imageScene->update();
	imageView->setSceneRect(image.rect());
	currentImagePath = path;
	
	QString status = QString("%1, %2x%3, %4 Bytes").arg(path).arg(image.width()).arg(image.height()).arg(QFile(path).size());

	mainStatusLabel->setText(status);
}

void MainWindow::zoomIn()
{
	imageView->scale(1.2, 1.2);
}

void MainWindow::zoomOut()
{
	imageView->scale(1 / 1.2, 1 / 1.2);
}

void MainWindow::saveAs()
{
	if (currentImage == nullptr)
	{
		QMessageBox::information(this, "Information", "Nothing to save.");
		return;
	}

	QFileDialog dialog(this);
	dialog.setWindowTitle("Save Image As...");
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));

	QStringList fileNames;

	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();

		if (QRegExp(".+\\.(png|bmp|jpg)").exactMatch(fileNames.at(0)))
		{
			currentImage->pixmap().save(fileNames.at(0));
		}
		else
		{
			QMessageBox::information(this, "Information", "Save error, bad format or filename.");
		}
	}
}

void MainWindow::prevImage()
{
	QFileInfo current(currentImagePath);
	QDir dir = current.absoluteDir();
	QStringList nameFilters;
	nameFilters << "*.png" << "*.bmp" << "*.jpg";
	QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
	int index = fileNames.indexOf(QRegExp(QRegExp::escape(current.fileName())));

	if (index > 0)
	{
		showImage(dir.absoluteFilePath(fileNames.at(index - 1)));
	}
	else
	{
		QMessageBox::information(this, "Information", "Current image is the first one.");
	}
}

void MainWindow::nextImage()
{
	QFileInfo current(currentImagePath);
	QDir dir = current.absoluteDir();
	QStringList nameFilters;
	nameFilters << "*png" << "*bmp" << "*jpg";
	QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);

	int index = fileNames.indexOf(QRegExp(QRegExp::escape(current.fileName())));

	if (index < fileNames.size() - 1)
	{
		showImage(dir.absoluteFilePath(fileNames.at(index + 1)));
	}
	else
	{
		QMessageBox::information(this, "Information", "Current image is the last one.");
	}
}
