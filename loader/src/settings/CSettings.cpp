/****************************************************************************
 * Copyright (C) 2015 Dimok
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/common.h"
#include "CSettings.h"
#include "SettingsEnums.h"
#include <fs/CFile.hpp>
#include <fs/FSUtils.h>
#include <utils/StringTools.h>
#include <utils/logger.h>
#include <language/gettext.h>

#define VERSION_LINE        "# WiiUPluginSystem - Main settings file v"
#define VALID_VERSION       1
#define CONFIG_FILENAME     "/wups.cfg"

CSettings *CSettings::settingsInstance = NULL;

CSettings::CSettings(){
    bChanged = false;
    memset(&nullValue, 0, sizeof(nullValue));
    nullValue.strValue = new std::string();
    configPath = DEFAULT_WUPSLOADER_PATH;
	this->SetDefault();
}

CSettings::~CSettings(){
    for(u32 i = 0; i < settingsValues.size(); i++)
    {
        if(settingsValues[i].dataType == TypeString)
            delete settingsValues[i].strValue;
    }
    delete nullValue.strValue;
}

void CSettings::SetDefault()
{
    for(u32 i = 0; i < settingsValues.size(); i++)
    {
        if(settingsValues[i].dataType == TypeString)
            delete settingsValues[i].strValue;
    }

    settingsNames.resize(MAX_VALUE);
    settingsValues.resize(MAX_VALUE);

	settingsNames[AppLanguage] = "AppLanguage";
    settingsValues[AppLanguage].dataType = TypeString;
    settingsValues[AppLanguage].strValue = new std::string();

}

bool CSettings::Load(){
	//! Reset default path variables to the right device
	SetDefault();

	std::string filepath = configPath;
	filepath += CONFIG_FILENAME;

    log_printf("CSettings::Load(line %d): Loading Configuration from %s\n",__LINE__,filepath.c_str());

	CFile file(filepath, CFile::ReadOnly);
	if (!file.isOpen())
        return false;


    std::string strBuffer;
    strBuffer.resize(file.size());
    file.read((u8 *) &strBuffer[0], strBuffer.size());
    file.close();


    //! remove all windows crap signs
    size_t position;
    while(1 && !strBuffer.empty())
    {
        position = strBuffer.find('\r');
        if(position == std::string::npos)
            break;

        strBuffer.erase(position, 1);
    }

	std::vector<std::string> lines = StringTools::stringSplit(strBuffer, "\n");


	if(lines.empty() || !ValidVersion(lines[0]))
		return false;

	for(u32 i = 0; i < lines.size(); ++i)
    {
        std::vector<std::string> valueSplit = StringTools::stringSplit(lines[i], "=");
        if(valueSplit.size() != 2)
            continue;

        while((valueSplit[0].size() > 0) && valueSplit[0][0] == ' ')
            valueSplit[0].erase(0, 1);

        while((valueSplit[1].size() > 0) && valueSplit[1][ valueSplit[1].size() - 1 ] == ' ')
            valueSplit[1].resize(valueSplit[1].size() - 1);

        for(u32 n = 0; n < settingsNames.size(); n++)
        {
            if(!settingsNames[n])
                continue;

            if(valueSplit[0] == settingsNames[n])
            {
                switch(settingsValues[n].dataType)
                {
                    case TypeBool:
                        settingsValues[n].bValue = atoi(valueSplit[1].c_str());
                        break;
                    case TypeS8:
                        settingsValues[n].cValue = atoi(valueSplit[1].c_str());
                        break;
                    case TypeU8:
                        settingsValues[n].ucValue = atoi(valueSplit[1].c_str());
                        break;
                    case TypeS16:
                        settingsValues[n].sValue = atoi(valueSplit[1].c_str());
                        break;
                    case TypeU16:
                        settingsValues[n].usValue = atoi(valueSplit[1].c_str());
                        break;
                    case TypeS32:
                        settingsValues[n].iValue = atoi(valueSplit[1].c_str());
                        break;
                    case TypeU32:
                        settingsValues[n].uiValue = strtoul(valueSplit[1].c_str(), 0, 10);
                        break;
                    case TypeF32:
                        settingsValues[n].fValue = atof(valueSplit[1].c_str());
                        break;
                    case TypeString:
                        if(settingsValues[n].strValue == NULL)
                            settingsValues[n].strValue = new std::string();

                        *settingsValues[n].strValue = valueSplit[1];
                        break;
                    default:
                        break;
                }
            }
        }
    }

	return true;
}

bool CSettings::ValidVersion(const std::string & versionString){
	int version = 0;

    if(versionString.find(VERSION_LINE) != 0)
        return false;

    version = atoi(versionString.c_str() + strlen(VERSION_LINE));

	return version == VALID_VERSION;
}

bool CSettings::Reset(){
	this->SetDefault();
	bChanged = true;

	if (this->Save())
        return true;

	return false;
}

bool CSettings::Save(){
    if(!bChanged)
        return true;

    FSUtils::CreateSubfolder(configPath.c_str());

	std::string filepath = configPath;
	filepath += CONFIG_FILENAME;

	CFile file(filepath, CFile::WriteOnly);
	if (!file.isOpen())
        return false;

	file.fwrite("%s%i\n", VERSION_LINE, VALID_VERSION);

	for(u32 i = 0; i < settingsValues.size(); i++)
    {
        switch(settingsValues[i].dataType)
        {
            case TypeBool:
                file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].bValue);
                break;
            case TypeS8:
                file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].cValue);
                break;
            case TypeU8:
                file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].ucValue);
                break;
            case TypeS16:
                file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].sValue);
                break;
            case TypeU16:
                file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].usValue);
                break;
            case TypeS32:
                file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].iValue);
                break;
            case TypeU32:
                file.fwrite("%s=%u\n", settingsNames[i], settingsValues[i].uiValue);
                break;
            case TypeF32:
                file.fwrite("%s=%f\n", settingsNames[i], settingsValues[i].fValue);
                break;
            case TypeString:
                if(settingsValues[i].strValue != NULL)
                    file.fwrite("%s=%s\n", settingsNames[i], settingsValues[i].strValue->c_str());
                break;
            default:
                break;
        }
    }

    file.close();
    bChanged = false;

	return true;
}
