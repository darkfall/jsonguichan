//
//  JsonGui - json parsing class for guichan (http://guichan.sourceforge.net), use jsoncpp for parse json script (http://jsoncpp.sourceforge.net).
//  author - darkfall (darkfall3@gmail.com)
//  website - https://github.com/darkfall/jsonguichan or http://code.google.com/p/jsonguichan/
//  blog - http://www.gamemastercn.com
//  example:
//     gcn::JsonGui* pGuiParser = new gcn::JsonGui;
//     pGuiParser->parse("myjsongui.json");
//     global_gui->add(pGuiParser->getWidget("myWindow"))
//     
//  see example.json for jsongui format example
//
//  jsongui.h
//  Sora
//
//  Created by Griffin Bu on 3/28/11.
//  Copyright 2011 Ruiwei Bu. All rights reserved.
//

#ifndef JSON_GUI_H_
#define JSON_GUI_H_

#include "jsoncpp/json.h"
#include "guichan.hpp"

#include <map>

namespace gcn {
    class JsonGui;
    typedef void (*JsonGuiParseFunc)(const Json::Value& val, gcn::Widget* parent, JsonGui* pCaller);
    
    class JsonGui {
    public:
        typedef Json::Value::Members VALUE_NAMES;
        JsonGui();
        
        /* 
         parse a json file
         @param filePath: the path of the file
         @retval: parse succeed or not
         */
        bool parse(const std::string& filePath);
        /* 
         parse a json file
         @param strData: the memory block that contains the file data
         @param size: the size of the block
         @retval: parse succeed or not
         */
        bool parse(void* strData, unsigned long size);
        
        /*
         get a widget from the parser
         @param name: the name of the widget, specified in "name" value
         @retval: the widget, return NULL if it not exists
         */
        gcn::Widget* getWidget(const std::string& name) const;

        /*
         register a external parse function to parse extra widgets
            when the parser met a unknown widget, it will try to find a func suitable for it with the name
         @param name: the name of the widget, such as "myButton"
         @param func: the external parse func
         */
        void registerParseFunc(const std::string& name, JsonGuiParseFunc func);

        
        inline void addToParent(gcn::Widget* widget, gcn::Widget* parent);

        /* 
           specific parse functions
        */
        void parseWidget(const Json::Value& val, gcn::Widget* parent);
        void parseDefaults(const Json::Value& val, gcn::Widget* widget);
                
        void parseContainer(const Json::Value& val, gcn::Widget* parent);
        void parseWindow(const Json::Value& val, gcn::Widget* parent);
        void parseButton(const Json::Value& val, gcn::Widget* parent);
        void parseSlider(const Json::Value& val, gcn::Widget* parent);
        void parseLabel(const Json::Value& val, gcn::Widget* parent);
        void parseIcon(const Json::Value& val, gcn::Widget* parent);
        void parseCheckbox(const Json::Value& val, gcn::Widget* parent);
        void parseTextBox(const Json::Value& val, gcn::Widget* parent);
        void parseTextField(const Json::Value& val, gcn::Widget* parent);
        void parseRadioButton(const Json::Value& val, gcn::Widget* parent);
        void parseImageButton(const Json::Value& val, gcn::Widget* parent);
        void parseListBox(const Json::Value& val, gcn::Widget* parent);
		void parseDropDown(const Json::Value& val, gcn::Widget* parent);
        
    private:
		inline void _parseWidget(const char* name, const Json::Value& val, gcn::Widget* parent);
		
        Json::Reader reader;
        Json::Value rootValue;
            
        typedef std::map<std::string, JsonGuiParseFunc> EXTENSION_FUNC_MAP;
        EXTENSION_FUNC_MAP extensionFuncs;
        
        typedef std::map<std::string, gcn::Widget*> WIDGET_MAP;
        WIDGET_MAP widgets;
    };
	
    /*
     a simple listmobel for with use listbox and dropdown
     add pushElement and removeElement
     */
	class JsonListModel : public gcn::ListModel {
	public:
		virtual int getNumberOfElements() { return (int)items.size(); }
		virtual std::string getElementAt(int i) { 
			if(i < items.size())
				return items[i];
			return "\0";
		}
        virtual void removeElementAt(int i) {
            if(i < items.size()) {
                items.erase(items.begin() + i);
            }
        }
		virtual void pushElement(const std::string& str) { items.push_back(str); }
		
    private:
		std::vector<std::string> items;
	};
    
} // namespace sora


#endif