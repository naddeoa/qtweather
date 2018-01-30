#ifndef GRAPHFRAME_H
#define GRAPHFRAME_H

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

class GraphFrame
{
public:
	GraphFrame(QString& title, QPixmap& graphPix);
	~GraphFrame();
	QVBoxLayout* getLayout();
	void setPix(QPixmap& pix);

private:
	QVBoxLayout* layout;
	QLabel* title;
	QLabel* graph;
	QScrollArea* scrollArea;

};

#endif // GRAPHFRAME_H
