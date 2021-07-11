#include "SystemStyles.hpp"

const SystemStyles * SystemStyles::m_styles = nullptr;


SystemStyles::SystemStyles() {

  API_ASSERT(m_styles == nullptr);
  m_styles = this;

  m_no_padding = lv::Style().set_left_padding(0).set_bottom_padding(0);
}
