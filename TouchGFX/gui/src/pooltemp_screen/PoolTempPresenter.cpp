#include <gui/pooltemp_screen/PoolTempView.hpp>
#include <gui/pooltemp_screen/PoolTempPresenter.hpp>

PoolTempPresenter::PoolTempPresenter(PoolTempView& v)
    : view(v)
{
}

void PoolTempPresenter::activate()
{
    //Set view to match pool temperature in Model
    view.setPoolTemp(model->getPoolTemperature());
}

void PoolTempPresenter::deactivate()
{
}

void PoolTempPresenter::poolTemperatureChanged()
{
    //forward new temperature to view
    view.setPoolTemp(model->getPoolTemperature());
}
