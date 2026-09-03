#include "Id.hh"

namespace sl {

u32 IdPool::acquire() {
    std::unique_lock lk{m_mutex};

    if (not m_freeIds.empty()) {
        u32 id = m_freeIds.front();
        m_freeIds.pop_front();
        return id;
    }
    return m_counter++;
}

void IdPool::release(u32 id) {
    std::unique_lock lk{m_mutex};
    m_freeIds.push_back(id);
}

}  // namespace sl
