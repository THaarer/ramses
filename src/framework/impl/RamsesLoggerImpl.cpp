//  -------------------------------------------------------------------------
//  Copyright (C) 2023 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "impl/RamsesLoggerImpl.h"

namespace ramses::internal
{
    RamsesLogger& GetRamsesLogger()
    {
        static RamsesLogger logger;
        return logger;
    }
}
