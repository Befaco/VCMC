// Copyright 2019 Sergio Retamero.
//
// Author: Sergio Retamero (sergio.retamero@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//

boolean checkJSONItem(JSONObject jObj, String theItem)
{
  if (jObj.isNull(theItem))
  {
    println("JSON file format not found:" + theItem);
    return false;
  }
  return true;
}

void FillJSONConfig(JSONObject newConfig)
{

  theGlobalConfig.FillJSON(newConfig);

  JSONArray ConfigArr;
  ConfigArr = new JSONArray();
  for (int i = 0; i < 9; i++)
  {
    JSONObject newInputCtrlConfig = new JSONObject();
    InputControlsCFG[i].FillJSON(newInputCtrlConfig);
    newInputCtrlConfig.setInt("Bank id", i + 1);
    ConfigArr.setJSONObject(i, newInputCtrlConfig);
  }

  // Create a new JSON config object

  newConfig.setJSONArray("CV Config", ConfigArr);
  //  saveJSONArray(ConfigArr, "data/new.json");
  //  saveJSONObject(newConfig, "data/newconfig.json");
}

void SaveJSONConfig()
{
  JSONObject newConfig = new JSONObject();
  FillJSONConfig(newConfig);
  saveJSONObject(newConfig, sCurrentFile);
}

void LoadJSONConfig()
{
  // A JSON object
  JSONObject json;
  json = loadJSONObject(sCurrentFile); //("data/newconfig.json");

  theGlobalConfig.ReadJSON(json);

  if (json.isNull("CV Config"))
  {
    println("JSON file format incorrect; CV Config");
    return;
  }

  JSONArray ConfigArr;
  ConfigArr = json.getJSONArray("CV Config");
  for (int i = 0; i < 9; i++)
  {
    JSONObject newInputCtrlConfig = ConfigArr.getJSONObject(i);
    InputControlsCFG[i].ReadJSON(newInputCtrlConfig);
  }

  DataText.setText(json.toString());
}
