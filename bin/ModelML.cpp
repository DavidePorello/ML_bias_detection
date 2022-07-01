#include "ModelML.h"

using namespace Eigen;

const VectorXf ModelML::getParams() {
    return _params;
}

void ModelML::setParams(const VectorXf &params) {
    _params = params;
}