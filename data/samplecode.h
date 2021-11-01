//http://www.jma.go.jp/bosai/forecast/data/forecast/270000.json

// char* input;
// size_t inputLength; (optional)
StaticJsonDocument<224> filter;

JsonObject filter_0          = filter.createNestedObject();
filter_0["publishingOffice"] = true;
filter_0["reportDatetime"]   = true;

JsonObject filter_0_timeSeries_0        = filter_0["timeSeries"].createNestedObject();
filter_0_timeSeries_0["timeDefines"][0] = true;

JsonObject filter_0_timeSeries_0_areas_0 = filter_0_timeSeries_0["areas"].createNestedObject();

JsonObject filter_0_timeSeries_0_areas_0_area    = filter_0_timeSeries_0_areas_0.createNestedObject("area");
filter_0_timeSeries_0_areas_0_area["name"]       = true;
filter_0_timeSeries_0_areas_0_area["code"]       = true;
filter_0_timeSeries_0_areas_0["weatherCodes"][0] = true;

StaticJsonDocument<1536> doc;

DeserializationError error = deserializeJson(doc, input, inputLength, DeserializationOption::Filter(filter));

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

JsonObject  root_0                  = doc[0];
const char* root_0_publishingOffice = root_0["publishingOffice"];  // "大阪管区気象台"
const char* root_0_reportDatetime   = root_0["reportDatetime"];    // "2021-10-31T11:00:00+09:00"

JsonArray root_0_timeSeries = root_0["timeSeries"];

JsonArray   root_0_timeSeries_0_timeDefines   = root_0_timeSeries[0]["timeDefines"];
const char* root_0_timeSeries_0_timeDefines_0 = root_0_timeSeries_0_timeDefines[0];
const char* root_0_timeSeries_0_timeDefines_1 = root_0_timeSeries_0_timeDefines[1];
const char* root_0_timeSeries_0_timeDefines_2 = root_0_timeSeries_0_timeDefines[2];

const char* root_0_timeSeries_0_areas_0_area_name = root_0_timeSeries[0]["areas"][0]["area"]["name"];
const char* root_0_timeSeries_0_areas_0_area_code = root_0_timeSeries[0]["areas"][0]["area"]["code"];

JsonArray   root_0_timeSeries_0_areas_0_weatherCodes   = root_0_timeSeries[0]["areas"][0]["weatherCodes"];
const char* root_0_timeSeries_0_areas_0_weatherCodes_0 = root_0_timeSeries_0_areas_0_weatherCodes[0];
const char* root_0_timeSeries_0_areas_0_weatherCodes_1 = root_0_timeSeries_0_areas_0_weatherCodes[1];
const char* root_0_timeSeries_0_areas_0_weatherCodes_2 = root_0_timeSeries_0_areas_0_weatherCodes[2];
