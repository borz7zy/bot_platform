#pragma once

#include <easy_json.hxx>
#include <string>
#include <logprint.hxx>

namespace configController
{
    void testEasyJson()
    {
        std::string s;
        if (easy_json::file_get_value_string("./test.json", "configVersion", s) == 0)
        {
            LOGI("CONFIG DRIVER", "%s", s.c_str());
        }
    }
};