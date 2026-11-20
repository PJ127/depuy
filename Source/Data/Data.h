#pragma once

namespace dm {
class Mesh;
}

// -----------------------------------------------------------------------------
namespace dm
// -----------------------------------------------------------------------------
{

//! \brief The data.
class Data {
  public:
    static Data *instance();

    Data();
    ~Data() {}

    dm::Mesh *mesh() { return currentMesh; }

    Data &operator=(const Data &) = delete;
    Data(const Data &) = delete;

  private:
    dm::Mesh *currentMesh;
};

// -----------------------------------------------------------------------------
}; // namespace dm
// -----------------------------------------------------------------------------
