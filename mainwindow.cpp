#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include <QGLWidget>

#include "Views/GLScene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Setup
    ui->setupUi(this);
    scene = new GLScene ();
    scene->inward_surface_box = ui->inward_suface_box;
    scene->outward_surface_box = ui->outward_surface_box;
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setViewport(new QGLWidget( QGLFormat(QGL::SampleBuffers  | QGL::DirectRendering)));
    ui->graphicsView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate);

    //Connections
    connect(ui->actionDelete_all_curves, SIGNAL(triggered()), scene, SLOT(delete_all()));
    connect(ui->inward_suface_box, SIGNAL(clicked(bool)), scene, SLOT(change_inward_outward_surface()));
    connect(ui->outward_surface_box, SIGNAL(clicked(bool)), scene, SLOT(change_inward_outward_surface()));
    connect(ui->subdivideCurveButton, SIGNAL(clicked(bool)), scene, SLOT(subdivide_current_spline()));
    connect(ui->onlyShowCurvePointsBox, SIGNAL(toggled(bool)), scene, SLOT(toggleShowCurrentCurvePoints(bool)));

    connect(ui->actionOpen_Image, SIGNAL(triggered()), ui->graphicsView, SLOT(loadImage()));
    connect(ui->actionSave_Image, SIGNAL(triggered()), ui->graphicsView, SLOT(saveImage()));
    connect(ui->actionOpen_Curves, SIGNAL(triggered()), ui->graphicsView, SLOT(loadCurves()));
    connect(ui->actionSave_Curves, SIGNAL(triggered()), ui->graphicsView, SLOT(saveCurves()));
    connect(ui->actionCreate_BSpline, SIGNAL(triggered()), ui->graphicsView, SLOT(create_bspline()));

    connect(ui->createCurveButton, SIGNAL(pressed()), ui->graphicsView, SLOT(create_bspline()));
    connect(ui->editCurveButton, SIGNAL(pressed()), ui->graphicsView, SLOT(edit_bspline()));
    connect(ui->moveCurveButton, SIGNAL(pressed()), ui->graphicsView, SLOT(move_bsplines()));
    connect(ui->pointSizeSlider, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeControlPointSize(int)));
    connect(ui->surfaceWidthSlider, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeSurfaceWidth(int)));
    connect(ui->showControlMeshBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(showControlMesh(bool)));
    connect(ui->showControlPointsBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(showControlPoints(bool)));

    connect(ui->actionDistance_transform, SIGNAL(triggered()), ui->graphicsView, SLOT(createDistanceTransformDEBUG()));

    connect(ui->actionView_surface_in_3D, SIGNAL(triggered()), ui->graphicsView, SLOT(show3Dwidget()));

    connect(ui->getShadingButton, SIGNAL(pressed()), ui->graphicsView, SLOT(applyShading()));

    connect(ui->setBrushButton, SIGNAL(pressed()), ui->graphicsView, SLOT(setBrush()));
    connect(ui->brushLightness, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeBrushLightness(int)));
    connect(ui->brushSizeSlider, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeBrushSize(int)));
    connect(ui->freehandBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(changeFreehand(bool)));
    connect(ui->radioContinuous, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(changeBrushTypeC(bool)));
    connect(ui->radioDiscrete, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(changeBrushTypeD(bool)));

    connect(ui->super1, SIGNAL(clicked()), ui->graphicsView, SLOT(setSuper1()));
    connect(ui->super2, SIGNAL(clicked()), ui->graphicsView, SLOT(setSuper2()));
    connect(ui->super4, SIGNAL(clicked()), ui->graphicsView, SLOT(setSuper4()));
    connect(ui->imgShowAll, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setImgShowAll(bool)));
    connect(ui->imgWriteAll, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setImgWriteAll(bool)));
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::center()
{
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}
