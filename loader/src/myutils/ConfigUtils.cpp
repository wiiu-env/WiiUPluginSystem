/****************************************************************************
 * Copyright (C) 2018 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include <vector>
#include <map>
#include <wups/config/WUPSConfig.h>
#include <dynamic_libs/vpad_functions.h>
#include <utils/logger.h>

#include "common/common.h"
#include "common/retain_vars.h"

#include "overlay_helper.h"
#include "ScreenUtils.h"
#include "ConfigUtils.h"
#include <language/gettext.h>

#include "ConfigInformation.h"

void ConfigUtils::configMenuOpenedCallback(wups_overlay_options_type_t screen, void * args) {
    // We close the configuration menu with the home button, but this would also
    // trigger the home menu.
    // To avoid this (and restore the GX2 buffers properly) we disable the home menu button
    // as long we're in the configuration menu.
    OSEnableHomeButtonMenu(false);

    std::vector<WUPSConfig*> * configList = (std::vector<WUPSConfig*> *) args;

    if(configList == NULL) {
        DEBUG_FUNCTION_LINE("configList was NULL.\n");
        // Re-enable home button. TODO: Only enable when it was enabled before.
        OSEnableHomeButtonMenu(true);
        return;
    }

    int32_t i = 0;
    int32_t x = 0;
    int32_t screen_y_pos = 0;
    int32_t visible_rows_start = 2;
    int32_t visible_rows_end = 16;
    int32_t visible_rows_range = visible_rows_end - visible_rows_start;

    WUPSConfig* curConfig = NULL;

    VPADData vpad_data;
    int32_t error;

    int32_t maxSelect = 0;
    int32_t curSelect = 0;

    int32_t curScreenOffset = 0;
    bool firstRun = true;
    while(true) {
        VPADRead(0, &vpad_data, 1, &error);
        if(vpad_data.btns_d == VPAD_BUTTON_HOME) {
            break;
        }

        int32_t newSelect = curSelect;
        bool changed = false;

        if(curConfig == NULL) {
            maxSelect = configList->size();
            if(maxSelect > 0) {
                maxSelect--;
            }

            bool selected = false;

            if(vpad_data.btns_r & VPAD_BUTTON_DOWN) {
                newSelect++;
            }
            if(vpad_data.btns_r & VPAD_BUTTON_UP) {
                newSelect--;
            }
            if(vpad_data.btns_r & VPAD_BUTTON_A) {
                selected = true;
                changed = true;
            }

            if(newSelect < 0) {
                newSelect = maxSelect;
            } else if(newSelect > maxSelect) {
                newSelect = 0;
            }

            if(newSelect != curSelect) {
                curSelect = newSelect;
                changed = true;
            }
            if(!firstRun) {
                if(!changed) {
                    continue;
                }
            }
            firstRun = false;

            ScreenUtils::OSScreenClear(screen);
            screen_y_pos = 0;
            ScreenUtils::printTextOnScreen(screen, 0, visible_rows_end + 1, gettext("Press HOME to return to the running application."));
            ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,gettext("Configurable running plugins:"));
            ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"==========================");

            if(configList->size() == 0) {
                ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,gettext("No configurable running plugins found."));
                ScreenUtils::flipBuffers(screen);
                continue;
            }

            int32_t index = 0;
            for (auto & curConfig_ : *configList) {
                if(index == curSelect) {
                    ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"> %s",curConfig_->getName().c_str());
                    if(selected) {
                        curConfig = curConfig_;
                        curSelect = 0;
                        curScreenOffset = 0;
                        firstRun = true;
                        maxSelect = 0;
                        for (auto & curCat : curConfig_->getCategories()) {
                            maxSelect += curCat->getItems().size();
                        }
                        if(maxSelect > 0) {
                            maxSelect--;
                        }
                    }

                } else {
                    ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"  %s",curConfig_->getName().c_str());
                }
                index++;
            }
            ScreenUtils::flipBuffers(screen);
            continue;
        }

        WUPSConfigButtons pressedButtons = WUPS_CONFIG_BUTTON_NONE;

        /*
        if(vpad_data.btns_r & VPAD_BUTTON_DOWN) {
            pressedButtons |= WUPS_CONFIG_BUTTON_DOWN;
        }
        if(vpad_data.btns_r & VPAD_BUTTON_UP) {
            pressedButtons |= WUPS_CONFIG_BUTTON_UP;
        }*/
        if(vpad_data.btns_d & VPAD_BUTTON_A) {
            pressedButtons |= WUPS_CONFIG_BUTTON_A;
        }
        if(vpad_data.btns_d & VPAD_BUTTON_B) {
            pressedButtons |= WUPS_CONFIG_BUTTON_B;
        }
        if(vpad_data.btns_d & VPAD_BUTTON_LEFT) {
            pressedButtons |= WUPS_CONFIG_BUTTON_LEFT;
        }
        if(vpad_data.btns_d & VPAD_BUTTON_RIGHT) {
            pressedButtons |= WUPS_CONFIG_BUTTON_RIGHT;
        }

        if(vpad_data.btns_r & VPAD_BUTTON_DOWN) {
            newSelect++;
        }
        if(vpad_data.btns_r & VPAD_BUTTON_UP) {
            newSelect--;
        }

        if(vpad_data.btns_d & VPAD_BUTTON_B) {
            curConfig = NULL;
            curSelect = 0;
            curScreenOffset = 0;
            firstRun = true;
            continue;
        }
        if(newSelect < 0) {
            newSelect = maxSelect;
        } else if(newSelect > maxSelect) {
            newSelect = 0;
        }

        if(newSelect != curSelect) {
            curSelect = newSelect;
            changed = true;
        }
        if(pressedButtons != WUPS_CONFIG_BUTTON_NONE) {
            changed = true;
        }

        int32_t cur_visible_rows = 0;
        if(!firstRun) {
            if(!changed) {
                continue;
            }
        }
        firstRun = false;

        ScreenUtils::OSScreenClear(screen);

        int32_t inSelect = 0;

        bool ignore = false;
        for (auto & curCat : curConfig->getCategories()) {
            cur_visible_rows += 2;
            for (auto & curItem : curCat->getItems()) {
                cur_visible_rows++;
                if(!ignore && curSelect == inSelect) {
                    if(curSelect == 0) {
                        curScreenOffset = 0;
                    }else if(cur_visible_rows + curScreenOffset >= visible_rows_range) {
                        curScreenOffset -= (cur_visible_rows + curScreenOffset) - visible_rows_range;
                    }else if(cur_visible_rows + curScreenOffset < visible_rows_range/2 && cur_visible_rows >= visible_rows_range/2) {
                        curScreenOffset -= (cur_visible_rows + curScreenOffset) - visible_rows_range/2;
                    }
                    ignore = true;
                }
                inSelect++;
            }
        }

        ScreenUtils::printTextOnScreen(screen, 0, 0, "Configuration for %s:", curConfig->getName().c_str());

        int32_t arrow_x_offset = 60;
        if(curScreenOffset < 0) {
            ScreenUtils::printTextOnScreen(screen, arrow_x_offset, visible_rows_start + 1, " ^");
            ScreenUtils::printTextOnScreen(screen, arrow_x_offset, visible_rows_start + 2, " |");
        }
        if(curScreenOffset + cur_visible_rows > visible_rows_range) {
            ScreenUtils::printTextOnScreen(screen, arrow_x_offset, visible_rows_end - 2, " | ");
            ScreenUtils::printTextOnScreen(screen, arrow_x_offset, visible_rows_end - 1, " v");
        }

        ScreenUtils::printTextOnScreen(screen, 0, visible_rows_end + 1, gettext("B = plugin overview, HOME  = back to running application."));

        screen_y_pos = curScreenOffset + visible_rows_start;

        inSelect = 0;

        for (auto & curCat : curConfig->getCategories()) {
            if((screen_y_pos + 2) >= visible_rows_end) {
                break;
            }
            if((screen_y_pos - visible_rows_start) < -1) {
                screen_y_pos += 2;
            } else if((screen_y_pos - visible_rows_start) < 0) {
                screen_y_pos++;
                ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"==========================");
            } else {
                ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"%s",curCat->getName().c_str());
                ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"==========================");
            }
            for (auto & curItem : curCat->getItems()) {
                if(screen_y_pos >= visible_rows_end) {
                    break;
                }
                if(curSelect == inSelect) {
                    if(pressedButtons != WUPS_CONFIG_BUTTON_NONE) {
                        curItem->onButtonPressed(pressedButtons);
                        firstRun = true;
                    }
                    ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"> %-36s %s\n",curItem->getDisplayName().c_str(),curItem->getCurrentValueSelectedDisplay().c_str());
                } else if((screen_y_pos - visible_rows_start) >= 0) {
                    ScreenUtils::printTextOnScreen(screen, x, screen_y_pos++,"  %-36s %s\n",curItem->getDisplayName().c_str(),curItem->getCurrentValueDisplay().c_str());
                } else {
                    screen_y_pos++;
                }
                inSelect++;
            }

        }

        ScreenUtils::flipBuffers(screen);
        i++;
    }
    // Re-enable home button. TODO: Only enable when it was enabled before.
    // Currently we can be sure it was enabled, because it's an requirement to enter the configuration menu.
    // This might change in the future.
    OSEnableHomeButtonMenu(true);
}

void ConfigUtils::openConfigMenu(std::vector<WUPSConfig*> configs) {
    overlay_helper(WUPS_OVERLAY_DRC_AND_TV_WITH_DRC_PRIO, configMenuOpenedCallback, (void*) &configs);
}

std::vector<ConfigInformation *> ConfigUtils::getConfigInformation() {
    std::vector<ConfigInformation *> configs;
    for(int32_t plugin_index=0; plugin_index<gbl_replacement_data.number_used_plugins; plugin_index++) {
        replacement_data_plugin_t * plugin_data = &gbl_replacement_data.plugin_data[plugin_index];
        if(plugin_data == NULL) {
            continue;
        }
        for(int32_t j=0; j<plugin_data->number_used_hooks; j++) {
            replacement_data_hook_t * hook_data = &plugin_data->hooks[j];
            if(hook_data->type == WUPS_LOADER_HOOK_GET_CONFIG) {
                if(hook_data->func_pointer == NULL) {
                    break;
                }
                void * func_ptr = hook_data->func_pointer;
                WUPSConfig * cur_config = ((WUPSConfig* (*)(void))((uint32_t*)func_ptr) )();
                DCFlushRange(cur_config, sizeof(WUPSConfig));
                if(cur_config != NULL) {
                    std::string fullPath(plugin_data->path);
                    std::string path = fullPath.substr(0, fullPath.find_last_of("/"));
                    size_t filenamePos = fullPath.find_last_of("/")+1;
                    std::string filename = fullPath.substr(filenamePos, fullPath.find_last_of(".") - filenamePos);
                    ConfigInformation * configInfo = new ConfigInformation(cur_config, path, filename.append(PLUGIN_CONFIG_FILE_EXT));
                    configs.push_back(configInfo);
                }
            }
        }
    }
    return configs;
}

void ConfigUtils::deleteConfigInformation(std::vector<ConfigInformation *> configs) {
    for (auto & curConfig : configs) {
        delete curConfig;
    }
}

void ConfigUtils::loadConfigFromSD() {
    deleteConfigInformation(getConfigInformation());
}

void ConfigUtils::openConfigMenu() {
    std::vector<ConfigInformation *> configInfos = getConfigInformation();

    std::vector<WUPSConfig *> configs;

    for (auto & curConfig : configInfos) {
        configs.push_back(curConfig->getConfig());
    }

    for (auto & curConfig : configs) {
        DCFlushRange(curConfig, sizeof(WUPSConfig));
    }

    ConfigUtils::openConfigMenu(configs);

    for (auto & curConfig : configs) {
        DCFlushRange(curConfig, sizeof(WUPSConfig));
    }

    deleteConfigInformation(configInfos);
}
