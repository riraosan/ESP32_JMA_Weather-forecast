
#pragma once
// R = Raw String Literals(C++)
const char PROGMEM filter[] = R"(
[
  {
    "publishingOffice": true,
    "reportDatetime": true,
    "timeSeries": [
      {
        "timeDefines": [
          true
        ],
        "areas": [
          {
            "area": {
              "name": true,
              "code": true
            },
            "weatherCodes": [
              true
            ],
            "weathers": [
              true
            ]
          }
        ]
      }
    ]
  }
])";
