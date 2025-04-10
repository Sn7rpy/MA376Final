#include "graph.h"


QImage processessGraphImg(const QString& pathToImage) {
	QImage inputImg(pathToImage);
	if (inputImg.isNull()) {
		std::cout << "couldn't load image";
		return inputImg;
	}

	QImage outputImg(inputImg.size(), QImage::Format_Grayscale8);


	for (int y = 0; y < inputImg.height(); y++) {
		for (int x = 0; x < inputImg.width(); x++) {
			QColor pixColor = inputImg.pixelColor(x, y);
			if (pixColor.red() >= 200 && pixColor.green() <= 10 && pixColor.blue() <= 10) {
				outputImg.setPixelColor(x, y, QColor(255, 255, 255));
			}
			else {
				outputImg.setPixelColor(x, y, QColor(0, 0, 0));
			}
		}

	}
	return outputImg;
}

std::vector<QPointF> findNodes(QImage inputImg) {
	QImage visited(inputImg.size(), QImage::Format_Grayscale8);
	visited.fill(QColor(0, 0, 0));

	std::vector<QPointF> centers;
	const int dx[8] = { -1, 0, 1, 0, -1, -1, 1, 1 };
	const int dy[8] = { 0, -1, 0, 1, -1, 1, -1, 1 };

	int h = inputImg.height();
	int w = inputImg.width();

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//std::cout << x << "," << y << "\n";
			if (inputImg.pixelColor(x, y) == QColor(255, 255, 255) && visited.pixelColor(x, y) == QColor(0, 0, 0)) {
				//std::cout << "blob moment \n";
				QQueue<QPoint> queue;
				queue.enqueue(QPoint(x, y));
				visited.setPixelColor(x, y, QColor(1, 1, 1));

				std::vector<QPoint> blobPixels;

				while (!queue.isEmpty()) {
					QPoint p = queue.dequeue();
					blobPixels.push_back(p);

					for (int i = 0; i < 8; i++) {
						int nx = p.x() + dx[i];
						int ny = p.y() + dy[i];

						if (nx >= 0 && ny >= 0 && nx < w && ny < h) {
							if (inputImg.pixelColor(nx, ny) == QColor(255, 255, 255) && visited.pixelColor(nx, ny) == QColor(0, 0, 0)) {
								queue.enqueue(QPoint(nx, ny));
								visited.setPixelColor(nx, ny, QColor(1, 1, 1));
							}
						}
					}
				}

				if (!blobPixels.empty()) {
					double sumX = 0, sumY = 0;
					for (const QPoint& p : blobPixels) {
						sumX += p.x();
						sumY += p.y();
					}

					centers.push_back(QPointF(sumX / blobPixels.size(), sumY / blobPixels.size()));
				}
			}
		}
	}

	return centers;
}

void setupView() {

}

