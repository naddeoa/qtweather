#include "graphframe.h"
#include <QApplication>
#include "mainwindow.h"

GraphFrame::GraphFrame(QString& title, QPixmap& graphPix){
	layout = new QVBoxLayout();

	this->graph = new QLabel();
	graph->setPixmap(graphPix);
	graph->setScaledContents(true);
	graph->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
	graph->setMinimumWidth(1600);
	graph->setMinimumHeight(220);


	this->title = new QLabel(title);
	this->title->setAutoFillBackground(true);
	MainWindow::colorize(this->title);

	this->scrollArea = new QScrollArea();
	scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	scrollArea->setMinimumHeight(240);
	scrollArea->setWidget(graph);


	layout->addWidget(this->title);
	layout->addWidget(scrollArea);

}


GraphFrame::~GraphFrame(){
	delete layout;
	delete title;
	delete graph;
	delete scrollArea;
}

QVBoxLayout* GraphFrame::getLayout(){
	return layout;
}

void GraphFrame::setPix(QPixmap &pix){
	graph->setPixmap(pix);
}
