#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(MainConfig,
    CONFIG_VALUE(isEnabled, bool, "Enable Mod", true);
    CONFIG_VALUE(megaThicc, bool, "THICC AS FUCK", false);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(isEnabled);
        CONFIG_INIT_VALUE(megaThicc);
    )
)
