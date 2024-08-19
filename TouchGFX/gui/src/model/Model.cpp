#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model()
    : modelListener(0), poolLightState(false), poolTemperature(18)
{
}

void Model::tick()
{
}

void Model::userSetPoolLight(bool state)
{
    poolLightState = state;
}

void Model::userSetTemperature(uint32_t temp)
{
    poolTemperature = temp;
}
