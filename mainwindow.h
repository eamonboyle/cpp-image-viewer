#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QLabel>
#include <QObject>
#include <QGraphicsPixmapItem>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	void initUI();

private slots:
	void openImage();
	void showImage(QString path);
	void zoomIn();
	void zoomOut();
	void saveAs();
	void prevImage();
	void nextImage();

private:
	QMenu* fileMenu;
	QMenu* viewMenu;

	QToolBar* fileToolBar;
	QToolBar* viewToolBar;

	QAction* openAction;
	QAction* saveAsAction;
	QAction* exitAction;
	QAction* zoomInAction;
	QAction* zoomOutAction;
	QAction* prevAction;
	QAction* nextAction;

	QGraphicsScene* imageScene;
	QGraphicsView* imageView;

	QStatusBar* mainStatusBar;
	QLabel* mainStatusLabel;

	QGraphicsPixmapItem* currentImage;
	QString currentImagePath;

	void createActions();
	void setupShortcuts();
};