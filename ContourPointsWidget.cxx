#include <vtkSmartPointer.h>
// To setup the ContourWidget and its representation:
#include <vtkProperty.h>
// To create the geometry:
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkMath.h>
// Usual VTK pipeline elements:
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCommand.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QApplication>
#include <QVTKOpenGLNativeWidget.h>
#include <QSurfaceFormat>
#include <vtkProperty.h>
#include <vtkInteractorStyleImage.h>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkMath.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

// Handle mouse events
class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera
{
public:
  static MouseInteractorHighLightActor* New();
  vtkTypeMacro(MouseInteractorHighLightActor, vtkInteractorStyleTrackballCamera);

  MouseInteractorHighLightActor()
  {
    LastPickedActor = NULL;
    LastPickedProperty = vtkProperty::New();
  }
  virtual ~MouseInteractorHighLightActor()
  {
    LastPickedProperty->Delete();
  }
  virtual void OnMouseMove() override
  {
    int* clickPos = this->GetInteractor()->GetEventPosition();

    // Pick from this location.
    vtkSmartPointer<vtkPropPicker>  picker =
      vtkSmartPointer<vtkPropPicker>::New();
    picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

    // If we picked something before, reset its property
    if (this->LastPickedActor)
      {
      this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
      }
    this->LastPickedActor = picker->GetActor();
    if (this->LastPickedActor)
      {
      // Save the property of the picked actor so that we can
      // restore it next time
      this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
      // Highlight the picked actor by changing its properties
      this->LastPickedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
      this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
      this->LastPickedActor->GetProperty()->SetSpecular(0.0);
      }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnMouseMove();
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
  }

private:
  vtkActor    *LastPickedActor;
  vtkProperty *LastPickedProperty;
};

vtkStandardNewMacro(MouseInteractorHighLightActor);

int main(int argc, char** argv)
{
    int numberOfSpheres = 100;
      if (argc > 1)
        {
        numberOfSpheres = atoi(argv[1]);
        }

    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);

    // A render window
    QVTKOpenGLNativeWidget widget;

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
      vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();

    widget.SetRenderWindow(renderWindow);
    widget.resize( 1024, 1024 );

    // Set the custom type to use for interaction.
    vtkSmartPointer<MouseInteractorHighLightActor> style =
      vtkSmartPointer<MouseInteractorHighLightActor>::New();
    style->SetDefaultRenderer(renderer);

    widget.GetInteractor()->SetInteractorStyle( style );

    for (int i = 0; i < numberOfSpheres; ++i)
      {
      vtkSmartPointer<vtkSphereSource> source =
        vtkSmartPointer<vtkSphereSource>::New();
      double x, y, z, radius;
      x = vtkMath::Random(-5,5);
      y = vtkMath::Random(-5,5);
      z = vtkMath::Random(-5,5);
      radius = vtkMath::Random(.5, 1.0);
      source->SetRadius(radius);
      source->SetCenter(x, y, z);
      source->SetPhiResolution(11);
      source->SetThetaResolution(21);
      vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
      mapper->SetInputConnection ( source->GetOutputPort());
      vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();
      actor->SetMapper ( mapper );
      double r, g, b;
      r = vtkMath::Random(.4, 1.0);
      g = vtkMath::Random(.4, 1.0);
      b = vtkMath::Random(.4, 1.0);
      actor->GetProperty()->SetDiffuseColor(r, g, b);
      actor->GetProperty()->SetDiffuse(.8);
      actor->GetProperty()->SetSpecular(.5);
      actor->GetProperty()->SetSpecularColor(1.0,1.0,1.0);
      actor->GetProperty()->SetSpecularPower(30.0);
      renderer->AddActor ( actor );
      }

    renderer->SetBackground ( .3, .4, .5 );

    widget.GetRenderWindow()->Render();
    widget.GetInteractor()->Initialize();
    widget.GetRenderWindow()->Render();
    widget.GetRenderWindow()->AddRenderer( renderer );
    widget.show();
    app.exec();

    return EXIT_SUCCESS;
}
