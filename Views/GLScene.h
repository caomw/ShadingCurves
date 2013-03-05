#ifndef GLSCENE_H
#define GLSCENE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QCheckBox>
#include "../Utilities/ImageUtils.h"
#include "../Curve/BSplineGroup.h"

class GLScene : public QGraphicsScene
{
    Q_OBJECT
public:
    typedef enum SketchMode
    {
        ADD_CURVE_MODE,
        IDLE_MODE
    } SketchMode;


    explicit GLScene(QObject *parent = 0);
    virtual ~GLScene();

    //IO functions
    bool openImage(std::string fname);
    void saveImage(std::string fname);
    bool openCurves(std::string fname);
    void saveCurves(std::string fname);
    void saveOff(std::string fname);

    //Sketching functions
    void createBSpline();
    void setSurfaceWidth(float _surface_width);
    int computeSurface(int spline_id);
    int registerPointAtScenePos(QPointF scenePos);
    QPointF sceneToImageCoords(QPointF scenePos);
    QPointF imageToSceneCoords(QPointF imgPos);
    bool pick(const QPoint& _mousePos, unsigned int &_nodeIdx,
               unsigned int& _targetIdx, QPointF* _hitPointPtr = NULL);

    //Drawing functions
    void display(bool only_show_splines = false);
    void draw_image(cv::Mat &image);
    void draw_control_point(int point_id);
    void draw_spline(int spline_id, bool only_show_splines = false, bool transform = true);
    void draw_surface(int surface_id);
    void adjustDisplayedImageSize();

    unsigned int getImageHeight() {return m_curImage.cols;}
    unsigned int getImageWidth()  {return m_curImage.rows;}

    bool writeCurrentSurface(std::ostream &ofs)
    {
        if (m_curSplineIdx != -1)
        {
            for (int i=0; i< m_splineGroup.num_surfaces(); ++i)
            {
                if (m_splineGroup.surface(i).connected_spline_id == m_curSplineIdx)
                {
                    m_splineGroup.surface(i).writeOFF(ofs);
                    return true;
                }
            }
        }
        return false;
    }

    std::vector<std::string> OFFSurfaces()
    {
        std::vector<std::string> surface_strings(m_splineGroup.num_surfaces());
        for (int i=0; i< m_splineGroup.num_surfaces(); ++i)
        {
            surface_strings[i] =  m_splineGroup.surface(i).surfaceToOFF();
        }
        return surface_strings;
    }

    cv::Mat curvesImage(bool only_closed_curves = false);
    void update_region_coloring();

    cv::Mat& currentImage()
    {
        return m_curImage;
    }

    SketchMode& sketchmode()
    {
        return m_sketchmode;
    }

    void setImage(cv::Mat im)
    {
        m_curImage = im;
        update();
    }

    cv::Mat* getImage()
    {
        return &m_curImage;
    }

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:

public slots:
    void change_inward_outward_surface();
    void delete_all();
    void subdivide_current_spline();
    void toggleShowCurrentCurvePoints(bool status);


private:
    cv::Mat m_curImage;
    int m_curSplineIdx;

    SketchMode m_sketchmode;
    double  m_modelview [16];
    double  m_projection [16];
    QList<std::pair<uint, uint> > selectedObjects;

    QVector<int> modified_spline_ids;

public:
    BSplineGroup m_splineGroup;
    float pointSize;
    float surfaceWidth;
    bool showControlMesh;
    bool showControlPoints;
    bool showCurrentCurvePoints;
    QCheckBox *inward_surface_box;
    QCheckBox *outward_surface_box;
    bool brush;
    int brushType;
    float brushSize;
    bool freehand;
    cv::Mat surfaceImg;

    QSizeF imSize;
};

void nurbsError(uint errorCode);

#endif // GLSCENE_H
