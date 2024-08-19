#include <gui/main_screen/MainView.hpp>

MainView::MainView()
{
}

void MainView::setupScreen()
{
}

void MainView::tearDownScreen()
{
}

void MainView::updatePoolLight()
{
    //report change to presenter
    presenter->setPoolLight(onOffButton.getState());
}

void MainView::setPoolLight(bool state)
{
    onOffButton.forceState(state);
    bulbYellow.setVisible(state);

    onOffButton.invalidate();
    bulbYellow.invalidate();
}
