//  -------------------------------------------------------------------------
//  Copyright (C) 2014 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "Ramsh/RamshStandardSetup.h"
#include "Ramsh/RamshCommunicationChannelDLT.h"
#include "Ramsh/RamshCommunicationChannelConsole.h"

namespace ramses_internal
{
    RamshStandardSetup::RamshStandardSetup(ramses::ERamsesShellType type, String prompt)
        : m_type(type)
        , m_prompt(std::move(prompt))
    {
    }

    RamshStandardSetup::~RamshStandardSetup()
    {
        RamshCommunicationChannelDLT::GetInstance().unregisterRamsh(*this);
    }

    bool RamshStandardSetup::start()
    {
        if (m_started)
            return false;
        m_started = true;

        if (m_type == ramses::ERamsesShellType_Console)
            m_consoleChannel = RamshCommunicationChannelConsole::Construct(*this, m_prompt);
        if (m_type == ramses::ERamsesShellType_Console || m_type == ramses::ERamsesShellType_Default)
            RamshCommunicationChannelDLT::GetInstance().registerRamsh(*this);
        return true;
    }

    bool RamshStandardSetup::stop()
    {
        if (!m_started)
            return false;
        m_consoleChannel.reset();
        RamshCommunicationChannelDLT::GetInstance().unregisterRamsh(*this);
        m_started  = false;
        return true;
    }
}
