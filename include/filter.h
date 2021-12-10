
#pragma once
// R"()" = Raw String Literals(C++)
constexpr char codeFilter[] PROGMEM = R"(
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
