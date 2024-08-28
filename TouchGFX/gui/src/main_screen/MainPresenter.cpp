#include <gui/main_screen/MainView.hpp>
#include <gui/main_screen/MainPresenter.hpp>

MainPresenter::MainPresenter(MainView& v)
    : view(v)
{
}

void MainPresenter::activate()
{
    view.setPoolLight(model->getPoolLight());
}

void MainPresenter::deactivate()
{
}

void MainPresenter::setPoolLight(bool state)
{
    model->userSetPoolLight(state);
}
