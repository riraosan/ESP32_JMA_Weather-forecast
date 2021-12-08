
#pragma once
// R = Raw String Literals(C++)
char filter[] = R"(
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
