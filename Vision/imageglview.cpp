#include "imageglview.h"

/**
 * @brief ImageGLView::ImageGLView
 */
ImageGLView::ImageGLView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    isMouseClicked(false),
    redMax(0),
    redMin(255),
    greenMax(0),
    greenMin(255),
    blueMax(0),
    blueMin(255)
{

}

/**
 * @brief ImageGLView::~ImageGLView
 */
ImageGLView::~ImageGLView()
{

}

/**
 * @brief ImageGLView::slotImageLoad
 * @param image
 */
void ImageGLView::slotImageLoad(QPixmap image)
{
    frameImage = image;
    this->repaint();
}

/**
 * @brief ImageGLView::paintEvent
 * @param event
 */
void ImageGLView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if( frameImage.isNull() )   return;

    QPainter painter;

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painterWidth  = painter.window().width();
    painterHeight = painter.window().height();

    painter.fillRect(0, 0, painterWidth, painterHeight, Qt::black);

    if( painterWidth/4*3 < painterHeight )  {
        startX = 0;
        endX   = painterWidth;
        startY = (painterHeight-(painterWidth/4*3))/2;
        endY   = painterHeight-startY;
    }
    else    {
        startX = (painterWidth-painterHeight/3*4)/2;
        endX   = painterWidth-startX;
        startY = 0;
        endY   = painterHeight;
    }


    QImage image = frameImage.toImage();
    QRect rectDrawArea(startX, startY, endX-startX, endY-startY);

    int len = 180*120*4;
    unsigned char *imageData = image.bits();

    for(int i=0; i<len; i+=4)   {
        int blue = imageData[i+0];
        int green = imageData[i+1];
        int red = imageData[i+2];

        if( (red <= redMax && red >= redMin) &&
            (green <= greenMax && green >= greenMin) &&
            (blue <= blueMax && blue >= blueMin) )
        {
            imageData[i+0] = 0;
            imageData[i+1] = 0;
            imageData[i+2] = 255;
        }
    }

    painter.drawImage(rectDrawArea, image);

    //            painter.drawEllipse(QPoint((loc%2560)/4, loc/2560), 10, 10);
}

/**
 * @brief ImageGLView::mousePressEvent
 * @param event
 */
void ImageGLView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    isMouseClicked = true;
}

/**
 * @brief ImageGLView::mouseReleaseEvent
 * @param event
 */
void ImageGLView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    isMouseClicked = false;
}

/**
 * @brief ImageGLView::mouseMoveEvent
 * @param event
 */
void ImageGLView::mouseMoveEvent(QMouseEvent *event)
{
    if( isMouseClicked )    {
        mousePosX = event->x();
        mousePosY = event->y();

        QColor maskColor = QColor::fromRgb(frameImage.toImage().pixel(mousePosX, mousePosY));

        int red = maskColor.red();
        int green = maskColor.green();
        int blue = maskColor.blue();

        redMax   = (redMax<red) ? red : redMax;
        redMin   = (redMin>red) ? red : redMin;
        greenMax = (greenMax<green) ? green : greenMax;
        greenMin = (greenMin>green) ? green : greenMin;
        blueMax  = (blueMax<blue) ? blue : blueMax;
        blueMin  = (blueMin>blue) ? blue : blueMin;
    }

    qDebug("%d %d    %d %d    %d %d", redMax, redMin, greenMax, greenMin, blueMax, blueMin);
}
