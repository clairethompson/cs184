#include "transformation.h"
#include "matrix.h"

Transformation::Transformation(Matrix m) {
    this->trans = m;
};

Transformation::Transformation() {
    this->trans = Matrix();
};