#include "ModelML.h"

using namespace Eigen;

[[nodiscard]] VectorXf ModelML::getParams() const {
    return _params;
}

void ModelML::setParams(const VectorXf &params) {
    _params = params;
}