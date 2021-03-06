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
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(false);

    addDockWidget(Qt::LeftDockWidgetArea, scene->shadingProfileView);
    scene->shadingProfileView->toggleViewAction()->setEnabled(true);
    scene->shadingProfileView->toggleViewAction()->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    ui->menuFile->addAction(scene->shadingProfileView->toggleViewAction());
    setStyleSheet("QMainWindow::separator { border-color: rgb(0, 0, 0); width: 1px; border-width: 1px; border-style: solid;}");
    scene->shadingProfileView->setMinimumWidth(230);

    scene->glWidget = new QGLWidget( QGLFormat(QGL::SampleBuffers  | QGL::DirectRendering));
    ui->graphicsView->setViewport(scene->glWidget);
    ui->graphicsView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate);

    showStatusMessage("Idle Mode");
    ui->drawingSubdivisionsBox->setValue(scene->drawingSubdLevels);
    ui->curveSubdivisionsBox->setValue(scene->curveSubdLevels);

//    QPixmap *mainWindowPixmap = new QPixmap("../imageshading/icons/MWicon2D.png");
//    QIcon mainWindowIcon(*mainWindowPixmap);
//    setWindowIcon(mainWindowIcon);

    //Connections
    connect(ui->actionDelete_all_curves, SIGNAL(triggered()), scene, SLOT(delete_all()));
    connect(ui->actionOpen_project, SIGNAL(triggered()), ui->graphicsView, SLOT(loadProject()));
    connect(ui->actionSave_Project, SIGNAL(triggered()), ui->graphicsView, SLOT(saveProject()));

    connect(ui->actionOpen_Backgroung_Image, SIGNAL(triggered()), ui->graphicsView, SLOT(loadBackgroungImage()));
    connect(ui->actionOpen_Image, SIGNAL(triggered()), ui->graphicsView, SLOT(loadImage()));
    connect(ui->actionSave_Image, SIGNAL(triggered()), ui->graphicsView, SLOT(saveImage()));
    connect(ui->actionSave_Screenshot, SIGNAL(triggered()), ui->graphicsView, SLOT(saveScreenshot()));
    connect(ui->actionSave_CurrentState, SIGNAL(triggered()), ui->graphicsView, SLOT(saveCurrentState()));
    connect(ui->actionOpen_Curves, SIGNAL(triggered()), ui->graphicsView, SLOT(loadCurves()));
    connect(ui->actionSave_Curves, SIGNAL(triggered()), ui->graphicsView, SLOT(saveCurves()));
    connect(ui->actionCreate_BSpline, SIGNAL(triggered()), ui->graphicsView, SLOT(create_bspline()));
    connect(ui->actionChange_resolution, SIGNAL(triggered()), ui->graphicsView, SLOT(changeResolution()));
    connect(ui->actionReuse_results_image, SIGNAL(triggered()), scene, SLOT(reuseResultsImage()));

    connect(ui->createCurveButton, SIGNAL(pressed()), ui->graphicsView, SLOT(create_bspline()));
    connect(ui->editCurveButton, SIGNAL(pressed()), ui->graphicsView, SLOT(edit_bspline()));
    connect(ui->moveCurveButton, SIGNAL(pressed()), ui->graphicsView, SLOT(move_bsplines()));
    connect(ui->pointSizeSlider, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeControlPointSize(int)));
    connect(ui->showControlMeshBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(showControlMesh(bool)));
    connect(ui->showControlPointsBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(showControlPoints(bool)));
    connect(ui->ShowCurvesBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(showCurves(bool)));
    connect(ui->ShowNormalsBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(showNormals(bool)));
    connect(ui->showColorsBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(showColors(bool)));
    connect(ui->curveSubdivisionsBox, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeCurveSubdLevels(int)));
    connect(ui->drawingSubdivisionsBox, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeDisplaySubdLevels(int)));
    connect(ui->global_thickness_box, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeGlobalThickness(int)));
    connect(ui->bresenhamCheckBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(toggleUseBresenham(bool)));

    connect(ui->sharp_box, SIGNAL(clicked(bool)), this, SLOT(change_point_sharpness()));
    connect(scene, SIGNAL(point_parameters_changed(bool, bool)), this, SLOT(update_point_sharpness_ui(bool,bool)));

    connect(ui->inward_suface_box, SIGNAL(clicked(bool)), this, SLOT(change_inward_outward_direction()));
    connect(ui->outward_surface_box, SIGNAL(clicked(bool)), this, SLOT(change_inward_outward_direction()));
    connect(ui->slope_curve_box, SIGNAL(clicked(bool)), this, SLOT(change_slope_curve()));
    connect(ui->uniform_subdivision_curve_box, SIGNAL(clicked(bool)), this, SLOT(change_uniform_subdivision()));
    //connect(ui->surfaceWidthSlider, SIGNAL(sliderReleased()), this, SLOT(change_bspline_parameters()));
    connect(ui->thickness_box, SIGNAL(valueChanged(int)), this, SLOT(change_bspline_parameters()));

    connect(ui->inward_bcolor_button, SIGNAL(clicked(bool)), scene, SLOT(change_inward_boundary_colour()));
    connect(ui->outward_bcolor_button, SIGNAL(clicked(bool)), scene, SLOT(change_outward_boundary_colour()));
    connect(scene, SIGNAL(bspline_parameters_changed(bool,float,bool,bool,bool,bool, int, QColor, QColor)), this, SLOT(update_bspline_parameters_ui(bool,float,bool,bool,bool,bool, int, QColor, QColor)));

    connect(ui->subdivideCurveButton, SIGNAL(clicked(bool)), scene, SLOT(subdivide_current_spline()));
    connect(ui->onlyShowCurvePointsBox, SIGNAL(toggled(bool)), scene, SLOT(toggleShowCurrentCurvePoints(bool)));

    connect(ui->actionDistance_transform, SIGNAL(triggered()), ui->graphicsView, SLOT(createDistanceTransformDEBUG()));
    connect(ui->actionDT_in_3D, SIGNAL(triggered()), ui->graphicsView, SLOT(showDistanceTransform3D()));
    connect(ui->actionCurves_in_3D, SIGNAL(triggered()), ui->graphicsView, SLOT(showCurvesImage3D()));

    connect(ui->actionView_surface_in_3D, SIGNAL(triggered()), ui->graphicsView, SLOT(show3Dwidget()));

    connect(ui->getShadingButton, SIGNAL(pressed()), ui->graphicsView, SLOT(applyShading()));

    connect(ui->setBrushButton, SIGNAL(pressed()), ui->graphicsView, SLOT(setBrush()));
    connect(ui->brushLightness, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeBrushLightness(int)));
    connect(ui->brushSizeSlider, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeBrushSize(int)));
    connect(ui->freehandBox, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(changeFreehand(bool)));
    connect(ui->radioContinuous, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(changeBrushTypeC(bool)));
    connect(ui->radioDiscrete, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(changeBrushTypeD(bool)));

    connect(ui->graphicsView, SIGNAL(setStatusMessage(QString)), this, SLOT(showStatusMessage(QString)));
    connect(scene, SIGNAL(setStatusMessage(QString)), this, SLOT(showStatusMessage(QString)));

    //shading stuff
    connect(ui->super1, SIGNAL(clicked()), ui->graphicsView, SLOT(setSuper1()));
    connect(ui->super2, SIGNAL(clicked()), ui->graphicsView, SLOT(setSuper2()));
    connect(ui->super4, SIGNAL(clicked()), ui->graphicsView, SLOT(setSuper4()));
    connect(ui->spinBoxSubd, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(setSurfSubdLevel(int)));
    connect(ui->imgShowAll, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setImgShowAll(bool)));
    connect(ui->imgWriteAll, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setImgWriteAll(bool)));
    connect(ui->clipping, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setClipping(bool)));
    connect(ui->clipMin, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(setClipMin(int)));
    connect(ui->clipMax, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(setClipMax(int)));
    connect(ui->shadingLab, SIGNAL(clicked()), ui->graphicsView, SLOT(setShadingLab()));
    connect(ui->shadingHLS, SIGNAL(clicked()), ui->graphicsView, SLOT(setShadingHLS()));
    connect(ui->shadingOwn, SIGNAL(clicked()), ui->graphicsView, SLOT(setShadingOwn()));
    connect(ui->shadingMatlab, SIGNAL(clicked()), ui->graphicsView, SLOT(setShadingMatlab()));
    connect(ui->shadingYxy, SIGNAL(clicked()), ui->graphicsView, SLOT(setShadingYxy()));
    connect(ui->shadingRGB, SIGNAL(clicked()), ui->graphicsView, SLOT(setShadingRGB()));
    connect(ui->blackOut, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setBlackOut(bool)));
    connect(ui->graphicsView, SIGNAL(setTimeOutput(QString)), ui->timeOutput, SLOT(setText(QString)));
    connect(ui->graphicsView, SIGNAL(setTimeOutputSub(QString)), ui->timeOutputSub, SLOT(setText(QString)));
    connect(ui->interactiveShading, SIGNAL(clicked(bool)), scene, SLOT(setInteractiveShading(bool)));
    connect(ui->ghostSurfacesEnabled, SIGNAL(clicked(bool)), scene, SLOT(setGhostSurfacesEnabled(bool)));
    connect(ui->clipHeight, SIGNAL(clicked(bool)), scene, SLOT(setClipHeight(bool)));
    connect(scene, SIGNAL(triggerShading()), ui->graphicsView, SLOT(applyShading()));
    connect(ui->flatImage, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setFlatImage(bool)));
    connect(ui->clrVsTxtr, SIGNAL(clicked(bool)), ui->graphicsView, SLOT(setClrVsTxtr(bool)));
    connect(ui->multiSubdRun, SIGNAL(clicked()), ui->graphicsView, SLOT(runMultiSubd()));
    connect(ui->multiSubdSpin, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(setMultiSubd(int)));
    connect(ui->graphicsView, SIGNAL(setMultiSubdOutputMin(QString)), ui->multiSubdOutputMin, SLOT(setText(QString)));
    connect(ui->graphicsView, SIGNAL(setMultiSubdOutputAvrg(QString)), ui->multiSubdOutputAvrg, SLOT(setText(QString)));
    connect(ui->graphicsView, SIGNAL(setMultiSubdOutputMax(QString)), ui->multiSubdOutputMax, SLOT(setText(QString)));

    QAction *actionActivateBackup = new QAction(QString("Backup Current Project"), ui->menuFile);
    actionActivateBackup->setCheckable(true);
    actionActivateBackup->setChecked(true);
    ui->menuFile->addAction(actionActivateBackup);
    connect(actionActivateBackup, SIGNAL(toggled(bool)), ui->graphicsView, SLOT(toggleBackupStatus(bool)));

    guiCleanup();
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::guiCleanup()
{
    /*ui->curveSubdivisionsBox->setVisible(false);
      ui->curveSubdivisionsLabel->setVisible(false);

      scene->curveSubdLevels  = ui->graphicsView->getSurfSubdLevel();
      connect(ui->spinBoxSubd, SIGNAL(valueChanged(int)), ui->graphicsView, SLOT(changeCurveSubdLevels(int)));
      */
}

void MainWindow::change_slope_curve()
{
    if (ui->slope_curve_box->isChecked())
    {
        ui->inward_suface_box->setChecked(true);
        ui->outward_surface_box->setChecked(true);
        ui->uniform_subdivision_curve_box->setChecked(false);
    }
    change_bspline_parameters();
}

void MainWindow::change_uniform_subdivision()
{
    if (ui->uniform_subdivision_curve_box->isChecked())
    {
        ui->slope_curve_box->setChecked(false);
    }
    change_bspline_parameters();
}

void MainWindow::change_inward_outward_direction()
{
    if (!ui->inward_suface_box->isChecked() || !ui->outward_surface_box->isChecked())
    {
        ui->slope_curve_box->setChecked(false);
    }
    change_bspline_parameters();
}

void MainWindow::change_bspline_parameters()
{

    scene->change_bspline_parameters(-1, //ui->surfaceWidthSlider->value(),
                                     ui->slope_curve_box->isChecked(),
                                     ui->uniform_subdivision_curve_box->isChecked(),
                                     ui->inward_suface_box->isChecked(),
                                     ui->outward_surface_box->isChecked(),
                                     ui->thickness_box->value());
}

void MainWindow::update_bspline_parameters_ui(bool enabled, float extent, bool _is_slope, bool _has_uniform_subdivision, bool _has_inward, bool _has_outward, int thickness, QColor inward_bcolor, QColor outward_bcolor)
{
    //ui->surfaceWidthSlider->setEnabled(enabled);
    ui->slope_curve_box->setEnabled(enabled);
    ui->uniform_subdivision_curve_box->setEnabled(enabled);
    ui->inward_suface_box->setEnabled(enabled);
    ui->outward_surface_box->setEnabled(enabled);
    ui->inward_bcolor_button->setEnabled(enabled);
    ui->outward_bcolor_button->setEnabled(enabled);
    ui->thickness_box->setEnabled(enabled);

    //ui->surfaceWidthSlider->setValue(extent);
    ui->slope_curve_box->setChecked(_is_slope);
    ui->uniform_subdivision_curve_box->setChecked(_has_uniform_subdivision);
    ui->inward_suface_box->setChecked(_has_inward);
    ui->outward_surface_box->setChecked(_has_outward);
    ui->thickness_box->setValue(thickness);

    QString style = "background: rgba(%1, %2, %3, %4);";
    ui->inward_bcolor_button->setStyleSheet(style.arg(inward_bcolor.red()).arg(inward_bcolor.green()).arg(inward_bcolor.blue()).arg(inward_bcolor.alpha()));
    ui->outward_bcolor_button->setStyleSheet(style.arg(outward_bcolor.red()).arg(outward_bcolor.green()).arg(outward_bcolor.blue()).arg(inward_bcolor.alpha()));
}

void MainWindow::change_point_sharpness()
{
    scene->change_point_parameters(ui->sharp_box->isChecked());
}

void MainWindow::update_point_sharpness_ui(bool enabled, bool isSharp)
{
    ui->sharp_box->setEnabled(enabled);
    ui->sharp_box->setChecked(isSharp);
}

void MainWindow::center()
{
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

void MainWindow::showStatusMessage(QString message)
{
    if (message.size() == 0)
    {
        message = scene->modeText;
        if (message.size() == 0)    message = "Idle Mode";
        if (scene->stats.size() > 0)
            message += " [" + scene->stats + "]";
    }
    statusBar()->showMessage(message, 0);
}
