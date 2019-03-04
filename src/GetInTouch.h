/*
  GetInTouch.h - Library for GetInTouch extension on twitch.
  Created by rootfrogs UG
*/

#ifndef GETINTOUCH_H
#define GETINTOUCH_H

#include "Arduino.h"

#define MAX_NUMBER_OF_ACTIONS 10
#define MAX_NUMBER_OF_INPUTS 3

/** This struct is a bundle of data of an action.
 *   
 *  @author rootfrogs UG
 *  @date 05.07.2018 
 */
class GITAction 
{
    public:
	/** constructor for an action
	* @param[in] name name of the action
	*/
		GITAction(String _name);
	
	/** will add a textbox to your action. The viewer will get displayed this textbox if he try to trigger this action.
    *  This function should be called in your setup.
    *  @param[in] label will be displayed together with the textbox.
    *  @param[in] min minimum textlength
    *  @param[in] max maximum textlength
    *  @return will return false when the parameters are wrong or invalid. Otherwise it will return true.
    */
    bool addTextbox(String label);
    bool addTextbox(String label, uint8_t min, uint8_t max);
    
	
	/** will add a combobox selection to your action. The viewer will get displayed this combobox if he try to trigger this action.
    *  This function should be called in your setup.
    *  @param[in] label will be displayed together with the combobox.
    *  @param[in] values all options seperated with an | charactar
    *  @return will return false when the parameters are wrong or invalid. Otherwise it will return true.
    */    
	bool addCombobox(String label, String values);
	
	/** will add a slider to your action. The viewer will get displayed this slider if he try to trigger this action.
    *  This function should be called in your setup.
    *  @param[in] label will be displayed together with the slider value.
    *  @param[in] min minimum value of the slider
    *  @param[in] max maximum value of the slider
    *  @param[in] start sets default value of the slider
    *  @return will return false when the parameters are wrong or invalid. Otherwise it will return true.
    */    	
	bool addSlider(String label, int min, int max, int start);
    
    /** will add a Matrix to your action. The viewer will get displayed this Pixel Matrix if he try to trigger this action.
    *  This function should be called in your setup.
    *  @param[in] label will be displayed together with the slider value.
    *  @param[in] cols number of cols of the matrix
    *  @param[in] rows number of rows of the matrix
    *  @return will return false when the parameters are wrong or invalid. Otherwise it will return true.
    */    	
    bool addPixelMatrix(String label, uint8_t cols, uint8_t rows);
	
	/** When your action is finished you have to call this method to release it in your extension.
    *  As long as you don't call this function, your action will be disabled for the viewers in the extension.
    */   
	void ended();
	
	/** If a viewer has triggered this action, this function will return true.
	*  You should call this function frequently.
    *  @return true if the action was triggered (will stay true until ended() method is called)
    */  
	bool isTriggered();
	
	/** If you have added input fields to your action, you can get the appropriate parameter which the viewer has selected or inserted.
	*  @param[in] index The index of the parameter (order as you have added the fields, beginning with 0)
    *  @return parameter value as String
    */  
	String getParameter(uint8_t index);
	
	/** If you have added input fields to your action, you can get the appropriate parameter which the viewer has  selected or inserted.
	*  @param[in] name the label you have given the parameter in the add method.
    *  @return parameter value as String
    */  
	String getParameter(String name);
	
	/** If you have added input fields to your action, you can get the appropriate parameter which the viewer has  selected or inserted.
	*  This method is particular for the case that you expect an integer value. It trys to convert the userinput into int.
	*  If this is not possible, it returns 0
	*  @param[in] index The index of the parameter (order as you have added the fields, beginning with 0)
    *  @return value of userinput as integer
    */  
	int getIntParameter(uint8_t index);
	
	/** If you have added input fields to your action, you can get the appropriate parameter which the viewer has selected or inserted.
	*  This method is particular for the case that you expect an integer value. It trys to convert the userinput into int.
	*  If this is not possible, it returns 0
	*  @param[in] name the label you have given the parameter in the add method.
    *  @return value of userinput as integer
    */  
	int getIntParameter(String name);
	
	/** Returns the username of the viewer who has triggered the action.
    *  @return username
    */  
	String getUsername();

    private:
		/** twitch username of the person who has triggered the action */
		String username;

		/** parameter which the user has choosen or inserted */
		String parameterName[MAX_NUMBER_OF_INPUTS];
		String parameterValue[MAX_NUMBER_OF_INPUTS];
	
		/** name of the action */
		String name;
		
		/** internal inputField String */
		String inputFields;
		
		/** number of added input fields */
		uint8_t inputFieldCnt;
		
		/** flag which shows if the action was triggered */
		bool triggered;

		/** GetInTouch is friend class, this class can access the private members and they have not to be public */
		friend class GetInTouch;
};


/** Main class of GetInTouch library. 
 *  This class will handle all the communication between your Arduino and the GetInTouchApp on your computer.
 *  Therefore it uses the Serial Port (USB connection to your computer)
 *   
 *  @author rootfrogs UG
 *  @date 24.06.2018 
 */
class GetInTouch
{
  public:
    /** constructor (empty)
    */
    GetInTouch();

    /** initialized the Serial port and internal members.
    */
    void init();

    /** will add an action to your extension.
    *  This function should be called in your setup().
    *  @param[in] action a reference to your (completly initialized) GITAction.
    *  @return will return false when the parameters are wrong or invalid. Otherwise it will return true.
    */
    bool addAction(GITAction* action);

    /** Will handle the Serial inputs and communication to your GetInTouch App
    *  This function have to be called frequently. 
    */
    void run();
    
  private:
    String readString;
    GITAction* actions[MAX_NUMBER_OF_ACTIONS];
    int numberOfActions;
    void analyzeString();
    void sendActions();
    bool addAction(String name, String inputFields);
};

#endif
