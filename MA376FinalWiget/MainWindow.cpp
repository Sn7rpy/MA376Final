#include "MainWindow.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <algorithm>
#include <QPushButton>
#include <QDataStream>
#include <QDebug>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindowClass)
{
	ui->setupUi(this);

	MainWindow::scene = new QGraphicsScene(this);
	ui->graphicsView->setScene(scene);

	ui->graphicsView->setRenderHint(QPainter::Antialiasing);
	ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

	QPushButton* saveButton = new QPushButton("Save Graph", this); 
	saveButton->move(10, 15);
	saveButton->resize(100, 30);

	connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveFeaturesToFile);

	QPushButton* loadButton = new QPushButton("Load Weights", this);
	loadButton->move(120, 15);
	loadButton->resize(100, 30);

	connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadWeights);

	QPushButton* djkButton = new QPushButton("Run Dijkstra's", this);
	djkButton->move(10, 740);
	djkButton->resize(100, 30);

	connect(djkButton, &QPushButton::clicked, this, &MainWindow::runDA);

	QPushButton* fwButton = new QPushButton("Run FW Algrthm", this);
	fwButton->move(120, 740);
	fwButton->resize(100, 30);

	connect(fwButton, &QPushButton::clicked, this, &MainWindow::runFW);
	
	QGroupBox* modeGroup = new QGroupBox("Mode", this);
	modeGroup->move(1300, 15);
	modeGroup->resize(200, 130);

	drawEdgeBtn = new QRadioButton("Draw Edges", modeGroup);
	changeIndexesBtn = new QRadioButton("Change Indicies", modeGroup);
	dijktraBtn = new QRadioButton("Run Dijkstra's Algorithm", modeGroup);
	deleteEdgesBtn = new QRadioButton("Delete Edges", modeGroup);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(drawEdgeBtn);
	vbox->addWidget(changeIndexesBtn);
	vbox->addWidget(dijktraBtn);
	vbox->addWidget(deleteEdgesBtn);
	vbox->addStretch(1);
	modeGroup->setLayout(vbox);

	drawEdgeBtn->setChecked(true);
	state = WindowState::DrawEdges;


	connect(drawEdgeBtn, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) {
			state = WindowState::DrawEdges;
		}
		});

	connect(changeIndexesBtn, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) {
			state = WindowState::ChangeIndicies;
		}
		});

	connect(dijktraBtn, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) {
			state = WindowState::DijkstraMode;
		}
		});

	connect(deleteEdgesBtn, &QRadioButton::toggled, this, [=](bool checked) {
		if (checked) {
			state = WindowState::DeleteEdges;
		}
		});

	edgesFile = "edges.dat";
	nodesFile = "nodes.dat";

}

void MainWindow::resizeToImg(const QSizeF& size) {
	ui->graphicsView->setSceneRect(QRectF(QPointF(0, 0), size));
	//ui->graphicsView->resize(size.width(), size.height());

}

void MainWindow::setBackgroundImg(const QString& filepath) {
	QPixmap bg(filepath);
	QGraphicsPixmapItem* bgItem = scene->addPixmap(bg);
	bgItem->setZValue(-1000);
	bgItem->setPos(0, 0);
}

void MainWindow::promptIndexChange()
{
	bool ok;
	QString inputText;
	if (firstSelected->index != "") {
		QString inputTitle("Rename Node");
	}
	else {
		QString inputTitle("Name Node");
	}
	QString newIndex = QInputDialog::getText(this, inputText, "Name:", QLineEdit::Normal, firstSelected->getIndex(), &ok);

	if (ok && !newIndex.isEmpty()) {
		firstSelected->setIndex(newIndex);
	}
}

void MainWindow::handleClickedNode(NodeItem* clickedNode) {

	QPen highlightPen(Qt::white, 2);
	QPen defaultPen(Qt::black, 1);

	if (!firstSelected) {
		firstSelected = clickedNode;
		firstSelected->setPen(highlightPen);
		if (state == WindowState::ChangeIndicies) {
			promptIndexChange();
			firstSelected->setPen(defaultPen);
			firstSelected = nullptr;

		}
		else if (state == WindowState::DijkstraMode) {
			dijOutput u = dijkstraResult.value(firstSelected->getHash());
			while (u.predecesor.predNode != NULL) {
				EdgeItem* edge = edgesMap.value(u.predecesor.predEdge);
				edge->setPen(QPen(Qt::green,3));
				highlightedEdges.push_back(edge);
				u = dijkstraResult.value(u.predecesor.predNode);

			}

			//firstSelected->setPen(defaultPen);
			//firstSelected = nullptr;
		}
	}
	else if (firstSelected != clickedNode) {
		if (state == WindowState::DrawEdges) {
			EdgeItem* edge = new EdgeItem(QLineF(firstSelected->scenePos(), clickedNode->scenePos()));


			if (!edgesMap.contains(edge->getHash())) {
				scene->addItem(edge);
				connect(edge, &EdgeItem::edgeClicked, this, &MainWindow::onEdgeClicked);
				edgesMap.insert(edge->getHash(), edge);

				connections.insert(firstSelected->getHash(),
					{ edge->getHash(),clickedNode->getHash(),0 }
				);

				connections.insert(clickedNode->getHash(),
					{ edge->getHash(), firstSelected->getHash(),1 }
				);

				firstSelected->setPen(defaultPen);
				firstSelected = clickedNode;
				firstSelected->setPen(highlightPen);
			}
			else {
				firstSelected->setPen(defaultPen);
				firstSelected = nullptr;
			}
		}
		else {
			firstSelected->setPen(defaultPen);
			firstSelected = nullptr;
			resetEdges();
		}


	}
	else {
		firstSelected->setPen(defaultPen);
		firstSelected = nullptr;
		resetEdges();
	}
}

void MainWindow::resetEdges()
{
	for (EdgeItem* edge : highlightedEdges) {
		edge->setPen(QPen(Qt::blue, 2));
	}
	highlightedEdges.clear();
}

void MainWindow::addNodesFromPoints(const std::vector<QPointF>& points)
{

	for (const QPointF& pt : points) {
		NodeItem* node = new NodeItem(pt);
		nodesMap.insert(node->getHash(), node);
		scene->addItem(node);

		connect(node, &NodeItem::nodeClicked, this, [=](NodeItem* clickedNode) {
			handleClickedNode(clickedNode);
			});
	}

	

}

void MainWindow::loadNodesFromFile(const QString& fileName)
{
	QPen highlightPen(Qt::white, 2);
	QPen defaultPen(Qt::black, 1);

	QFile file(fileName);
	file.open(QIODeviceBase::ReadOnly);

	QDataStream in(&file);
	quint32 count = 0;
	in >> count;

	for (quint32 i = 0; i < count; i++) {
		QPointF point;
		QString index;
		in >> point;
		in >> index;

		NodeItem* node = new NodeItem(point);

		scene->addItem(node);
		node->setIndex(index);
		connect(node, &NodeItem::nodeClicked, this, [=](NodeItem* clickedNode) {
			handleClickedNode(clickedNode);
			});
		nodesMap.insert(node->getHash(),node);

	}

	file.close();
}

void MainWindow::saveFeaturesToFile() {
	QFile edgeFile(edgesFile);
	edgeFile.open(QIODeviceBase::WriteOnly);

	QDataStream out(&edgeFile);
	out << static_cast<quint32>(edgesMap.size());
	for (EdgeItem* edge : edgesMap.values()) {
		out << edge->getLine();
		out << edge->getIndex();
	}
	edgeFile.close();

	QFile nodeFile(nodesFile);
	nodeFile.open(QIODeviceBase::WriteOnly);

	QDataStream out2(&nodeFile);
	out2 << static_cast<quint32>(nodesMap.size());
	for (NodeItem* node : nodesMap.values()) {
		out2 << node->getPoint();
		out2 << node->getIndex();
	}
	nodeFile.close();
}

void MainWindow::loadLinesFromFile(const QString& fileName) {
	QFile file(fileName);
	file.open(QIODeviceBase::ReadOnly);

	QDataStream in(&file);
	quint32 count = 0;
	in >> count;

	for (quint32 i = 0; i < count; i++) {
		QLineF line;
		QString index;
		in >> line;
		in >> index;

		EdgeItem* edge = new EdgeItem(line);
		scene->addItem(edge);
		if (index != "") {
			edge->setIndex(index);
		}
		connect(edge, &EdgeItem::edgeClicked, this, &MainWindow::onEdgeClicked);
		edgesMap.insert(edge->getHash(),edge);
		if (index != "") {
			edgeIndexes.insert(index, edge);
		}
		connections.insert(edge->getHashP1(),
			{ edge->getHash(), edge->getHashP2(),0 }
		);
		connections.insert(edge->getHashP2(), 
			{ edge->getHash(), edge->getHashP1(), 1 }
		);

	}

	file.close();

}

void MainWindow::setWeightsFromCSV(const QString& filepath)
{
	QFile csv(filepath);
	csv.open(QIODevice::ReadOnly);

	QTextStream in(&csv);
	QString header = in.readLine();
	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList cells = line.split(",");
		QString index = cells[0].trimmed() + cells[1].trimmed();
		weightStruct newWeights;

		newWeights.timeForw = cells[2].trimmed().toInt();
		newWeights.timeBack = cells[3].trimmed().toInt();
		newWeights.scene = cells[4].trimmed().toInt();
		newWeights.inside = cells[5].trimmed().toInt();
		newWeights.cover = cells[6].trimmed().toInt();
		newWeights.distance = cells[7].trimmed().toDouble();

		if (edgeIndexes.contains(index)){ 
			edgeIndexes.value(index)->setWeights(newWeights); }
		else { continue; }
		

	}

	csv.close();

}

void MainWindow::getNodeFile(const QString& fileName)
{
	nodesFile = fileName;
}

void MainWindow::getEdgeFile(const QString& fileName)
{
	edgesFile = fileName;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onEdgeClicked(EdgeItem* edge)
{
	if (state == WindowState::ChangeIndicies) {
		bool ok;

		QString newIdx = QInputDialog::getText(this, "Name Edge", "Name:", QLineEdit::Normal, edge->getIndex(), &ok);

		if (ok && !newIdx.isEmpty()) {
			edge->setIndex(newIdx);
			edgeIndexes.insert(newIdx, edge);
		}
	}
	else if (state == WindowState::DeleteEdges) {
		size_t edgeHash = edge->getHash();
		size_t p1Hash = edge->getHashP1();
		size_t p2Hash = edge->getHashP2();
		edgesMap.remove(edgeHash);
		QList<connStruct> p1Values = connections.values(p1Hash);
		QList<connStruct> p2Values = connections.values(p2Hash);
		connections.remove(p1Hash);
		connections.remove(p2Hash);
		for (connStruct value : p1Values) {
			if (value.edge != edgeHash) {
				connections.insert(p1Hash, value);
			}
		}
		for (connStruct value : p2Values) {
			if (value.edge != edgeHash) {
				connections.insert(p2Hash, value);
			}
		}
		scene->removeItem(edge);
		delete edge;
	}
}

void MainWindow::loadWeights()
{
	QStringList csvPathList = QFileDialog::getOpenFileNames(this, tr("Open CSV File"),__FILE__,"CSV files (*.csv)");

	for (QString csvPath: csvPathList)
	{
		setWeightsFromCSV(csvPath);
	}
}

QHash<size_t, dijOutput> MainWindow::runDijkstras(NodeItem* startNode) {
	QHash<size_t, dijOutput> output;
	QHash<size_t, predOut> preds;
	QHash<size_t, double> yEdge;
	QHash<size_t, bool> temp;
	double pos_inf = std::numeric_limits<double>::infinity();

	for (NodeItem* node : nodesMap.values()) {
		size_t hash = node->getHash();
		preds.insert(hash, {NULL, NULL});
		yEdge.insert(hash, pos_inf);
		temp.insert(hash, true);
	}

	size_t u = startNode->getHash();

	yEdge[u] = 0;

	while (u != NULL) {

		for (const connStruct& connection : connections.values(u)) {
			size_t j = connection.outNode;
			if (!temp.contains(j)) {
				continue;
			}

			EdgeItem* edge = edgesMap.value(connection.edge);
			double edgeWeight = edge->getTimeWeight(connection.reverseDir);
			double dPrime = yEdge.value(u) + edgeWeight;
			if (dPrime < yEdge.value(j)) {
				yEdge[j] = dPrime;
				preds[j] = {u,edge->getHash()};
			}

		}
		output.insert(u, { {preds.value(u),},yEdge.value(u) });
		temp.remove(u);
		if (temp.isEmpty()) {
			break;
		}
		else { u = temp.key(true); }

		for (size_t candidateV : temp.keys()) {
			double yV = yEdge.value(candidateV);
			if (yV < yEdge.value(u)) {
				u = candidateV;
			}
		}
		if (yEdge.value(u) == pos_inf) {
			break;
		}

	}


	return output;

}

std::vector<std::vector<double>> MainWindow::runFloydWar() {
	int n = nodesMap.size();
	double pos_inf = std::numeric_limits<double>::infinity();
	std::vector<std::vector<double>> output(n,std::vector<double>(n,pos_inf));

	QHash<size_t, int> colNrows;

	int a = 0;
	for (size_t nodeHash : nodesMap.keys()) {
		colNrows.insert(nodeHash,a);
		output[a][a] = 0;
		a++;
	}
	//make adjacency matrix
	for (size_t origin : connections.keys()) {
		for (const connStruct& conn : connections.values(origin)) {
			if (conn.reverseDir) { 
				int edgeWeight = edgesMap.value(conn.edge)->getTimeWeight(conn.reverseDir);
				output[colNrows.value(conn.outNode)][colNrows.value(origin)] = edgeWeight;
			}
			int edgeWeight = edgesMap.value(conn.edge)->getTimeWeight(conn.reverseDir);
			output[colNrows.value(origin)][colNrows.value(conn.outNode)] = edgeWeight;
		}
	}

	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (output[i][k] + output[k][j] < output[i][j]) {
					output[i][j] = output[i][k] + output[k][j];
				}
			}
		}
	}

	QFile file("resultsFW.txt");
	file.open(QIODevice::WriteOnly);

	QTextStream out(&file);
	out << "    ";
	for (int x = 0; x < n; x++) {
		NodeItem* node = nodesMap.value(colNrows.key(x));
		out << node->getIndex() << ", ";
	}
	out << "\n \n";
	int y = 0;
	for (std::vector<double> rows : fwResult) {
		NodeItem* node = nodesMap.value(colNrows.key(y));
		out << node->getIndex() << "| ";
		y++;
		for (double item : rows) {
			out << item << ",";
		}
		out << "\n";
	}
	file.close();

	return output;
}

void MainWindow::runDA()
{
	dijkstraResult = runDijkstras(firstSelected);
	firstSelected->setBrush(Qt::yellow);
	firstSelected->setPen(QPen(Qt::black, 1));
	firstSelected = nullptr;
	dijktraBtn->setChecked(true);
	
	QFile file("resultsDA.txt");
	file.open(QIODevice::WriteOnly);

	QTextStream out(&file);

	if (true) {
		for (size_t nodeHash : dijkstraResult.keys()) {
			dijOutput nodeResult = dijkstraResult.value(nodeHash);
			if (nodeResult.predecesor.predNode == NULL) { continue; }
			NodeItem* node = nodesMap.value(nodeHash);
			NodeItem* predNode = nodesMap.value(nodeResult.predecesor.predNode);
			EdgeItem* predEdge = edgesMap.value(nodeResult.predecesor.predEdge);
			double cost = nodeResult.minPath;

			if (node->getIndex() != "" && predNode->getIndex() != "") {
				out << "Node:" << node->getIndex()<< " | Pred:" << predNode->getIndex()
					<< " | Edge:" << predEdge->getIndex() << " | Cost:" << cost << "\n";
			}else
			{
				out << "Node:" << node->getPoint().x() << "," << node->getPoint().y()
					<< " | Pred:" << predNode->getPoint().x() << "," << predNode->getPoint().y()
					<< " | Edge:" << predEdge->getIndex() << " | Cost:" << cost << "\n";
			}

		}
	}else
	{
		for (dijOutput nodeResult : dijkstraResult.values()) {
			if (edgesMap.contains(nodeResult.predecesor.predEdge))
			{
				EdgeItem* predEdge = edgesMap.value(nodeResult.predecesor.predEdge);
				out << predEdge->getIndex() << " , " << nodeResult.minPath << "\n";
			}
			else {
				out << nodeResult.predecesor.predEdge << " , " << nodeResult.minPath << "\n";
			}
		}
	}

	out << dijkstraResult.size();

	file.close();
}

void MainWindow::runFW()
{
	fwResult = runFloydWar();
	

}


