#include <gui/pooltemp_screen/PoolTempView.hpp>

PoolTempView::PoolTempView()
{
}

void PoolTempView::setupScreen()
{
}

void PoolTempView::tearDownScreen()
{
}

void PoolTempView::setPoolTemp(uint32_t temp)
{
    //update slider and temperature text
    slider.setValue(temp);
    Unicode::snprintf(tempTextBuffer, 3, "%d", temp);
    tempText.invalidate();
}

void PoolTempView::sliderMoved(int value)
{
    //report to presenter and update text
    presenter->userSetTemperature(value);
    Unicode::snprintf(tempTextBuffer, 3, "%d", value);
    tempText.invalidate();
}
