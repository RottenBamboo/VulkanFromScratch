#pragma once

#include <string>
#include "uuid.h"

namespace RottenBamboo 
{
    struct RBData 
    {
        uuids::uuid m_GUID;
        
        inline virtual uuids::uuid GetGUID() {return m_GUID;};
        void GenerateGUID()
        {
            std::random_device rd;
            auto seed_data = std::array<int, std::mt19937::state_size> {};
            std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            std::mt19937 generator(seq);
            uuids::uuid_random_generator gen{generator};
            m_GUID = gen();
        }

        inline bool IsGUIDNil() const
        {
            return m_GUID.is_nil();
        }

        inline bool GUIDToString(std::string& id) const
        {
            if(m_GUID.is_nil())
            {
                return false;
            }
            id = uuids::to_string(m_GUID);
            return true;
        }
        
        inline virtual bool GenerateGUIDFromString(const std::string& uuidString)
        {
            std::optional<uuids::uuid> id = uuids::uuid::from_string(uuidString);
            if(id.has_value())
            {
                m_GUID = id.value();
                return true;
            }
            return false;
        };
    };
}