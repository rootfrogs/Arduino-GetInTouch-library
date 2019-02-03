/*
  GetInTouch.cpp - Library for GetInTouch extension on twitch.
  Created by rootfrogs UG
*/

#include "GetInTouch.h"

GITAction::GITAction(String _name)
{
	//length of name will be checked if action is added to GetInTouch Object
	name = _name;
	triggered = false;
	inputFieldCnt = 0;
}

bool GITAction::addTextbox(String label)
{
	if(label.length() > 40)
	{
		label = label.substring(0,40);
	}
	
	if(inputFieldCnt < MAX_NUMBER_OF_INPUTS)
	{
		inputFields += "%TEXT~" + label;
		parameterName[inputFieldCnt] = label;
		inputFieldCnt++;
		return true;
	}
	else
	{
		return false;
	}
}

bool GITAction::addCombobox(String label, String values)
{
	if(label.length() > 40)
	{
		label = label.substring(0,40);
	}
	
	if(values.length() > 200)
	{
		values = values.substring(0,200);
	}
	
	if(inputFieldCnt < MAX_NUMBER_OF_INPUTS)
	{
		  values.replace(";", ",");
		  values.replace("%", "_");
		  values.replace("~", "-");
		  values.replace("\r", "");
		  values.replace("\n", "");
		  
		  inputFields += "%COMBO~" + label + "~"  + values;
		parameterName[inputFieldCnt] = label;
		inputFieldCnt++;
		
		return true;
	}  
	else
	{
		return false;
	}
}

bool GITAction::addSlider(String label, int min, int max, int start)
{
	if(label.length() > 40)
	{
		label = label.substring(0,40);
	}
	
	if(min > max)
	{
		int tmp = min;
		min = max;
		max = tmp;
	}
	
	if(start < min) start = min;
	if(start > max) start = max;
  
	if(inputFieldCnt < MAX_NUMBER_OF_INPUTS)
	{
		inputFields +=  "%SLIDER~" + label + "~" + String(min) + "|" + String(max) + "|" + String(start);
		parameterName[inputFieldCnt] = label;
		inputFieldCnt++;
		
		return true;
	}  		
	else
	{
		return false;
	}
}

bool GITAction::addPixelMatrix(uint8_t cols, uint8_t rows)
{
	if(cols > 32) cols = 32;
    if(rows > 32) rows = 32;
  
	if(inputFieldCnt < MAX_NUMBER_OF_INPUTS)
	{
		inputFields +=  "%MATRIX~~" + String(cols) + "|" + String(rows);
		parameterName[inputFieldCnt] = label;
		inputFieldCnt++;
		
		return true;
	}  		
	else
	{
		return false;
	}
}

bool GITAction::isTriggered()
{
	return triggered;
}

String GITAction::getParameter(String name)
{
	for(uint8_t i = 0; i < MAX_NUMBER_OF_INPUTS; i++)
	{
		if(name.equals(parameterName[i]))
		{
			return getParameter(i);
		}
	}
	
	return "";
}

String GITAction::getParameter(uint8_t index)
{
	if(index < MAX_NUMBER_OF_INPUTS)
	{
		return parameterValue[index];
	}
	else
	{
		return "";
	}
}

int GITAction::getIntParameter(String name)
{
	for(uint8_t i = 0; i < MAX_NUMBER_OF_INPUTS; i++)
	{
		if(name.equals(parameterName[i]))
		{
			return getIntParameter(i);
		}
	}
	
	return 0;
}

int GITAction::getIntParameter(uint8_t index)
{
	if(index < MAX_NUMBER_OF_INPUTS)
	{
		return parameterValue[index-1].toInt();
	}
	else
	{
		return 0;
	}
}

void GITAction::ended()
{
	if(triggered==true)		//prevent multiple sending of actionEnded
	{
		Serial.print("actionEnded:");
		Serial.println(name);
		triggered = false;
	}
}

String GITAction::getUsername()
{
	return username;
}

GetInTouch::GetInTouch()
{
}

void GetInTouch::init()
{
  Serial.begin(9600);
  numberOfActions = 0;
}

bool GetInTouch::addAction(GITAction* action)
{
  if(numberOfActions >= MAX_NUMBER_OF_ACTIONS)
  {
    return false;
  }

  if(action->name.length() < 5)
  {
    return false;
  }

  if(action->name.length() > 40)
  {
    action->name = action->name.substring(0,40);
  }

  action->name.replace("|", "/");
  action->name.replace(";", ",");
  action->name.replace("%", "_");
  action->name.replace("~", "-");
  action->name.replace("\r", "");
  action->name.replace("\n", "");

  for(uint8_t i = 0; i < numberOfActions; i++)
  {
    if(actions[i]->name.equals(action->name))
      return false;   //name already exists
  }
  
  actions[numberOfActions] = action;
  numberOfActions++;

  return true;
}

void GetInTouch::run()
{
  while (Serial.available()) 
  {
    if (Serial.available() > 0) 
    {
      char c = Serial.read();  //gets one byte from serial buffer

      if(c == 13)
        analyzeString();
      else if(c > 31)    //accept text characters only.
        readString += c;
    }
  }
}

void GetInTouch::analyzeString()
{
  //if(readString.equals("getActions"))
  if(readString.indexOf("getActions") >= 0)
  {
    sendActions();
  }
  else if(readString.startsWith("triggerAction:"))
  {
    String actionStr = readString.substring(14);
	
	for(uint8_t i = 0; i < numberOfActions; i++)
	{
		if(actionStr.startsWith(actions[i]->name))
		{
			actions[i]->triggered = true;
			actionStr = actionStr.substring(actionStr.indexOf("%")+1);
			actions[i]->username = actionStr.substring(0,actionStr.indexOf("%"));
			actionStr = actionStr.substring(actionStr.indexOf("%")+1);
			
			for(uint8_t j = 0; j < MAX_NUMBER_OF_INPUTS; j++)
			{
				if(actionStr.length() > 0)
				{
					if(actionStr.indexOf("|") > 0)
					{
						actions[i]->parameterValue[j] = actionStr.substring(0, actionStr.indexOf("|"));
						actionStr = actionStr.substring(actionStr.indexOf("|")+1);
					}
					else
					{
						actions[i]->parameterValue[j] = actionStr;
						actionStr = "";
					}
				}
				else
				{
					actions[i]->parameterValue[j] = "";
				}
			}	
			
			Serial.print("actionTriggered:");
			Serial.println(actions[i]->name);
			
			break;
		}
	}
  }
  
  readString = "";
}

void GetInTouch::sendActions()
{
  Serial.print("myActions:");
  for(uint8_t i = 0; i < numberOfActions; i++)
  {
    if(i!=0) Serial.print(";");
    Serial.print(actions[i]->name);
	Serial.print(actions[i]->inputFields);
  }
  Serial.write('\r');
}
