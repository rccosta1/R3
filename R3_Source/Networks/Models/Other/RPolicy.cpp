#include "RPolicy.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include "RPolygonalAreaSourceModel.h"
#include "RLocation.h"
#include <math.h>


RPolicy::RPolicy(QObject *parent, QString name)
    : RObject(parent, name)
{

}


RPolicy::~RPolicy()
{

}


int RPolicy::resetTime()
{

    return 0;
}

