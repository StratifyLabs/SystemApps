#include "Model.hpp"

Model * Model::m_model = nullptr;

Model::Model(){
  API_ASSERT(m_model == nullptr);
  m_model = this;
}
